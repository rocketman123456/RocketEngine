//========================================================================
// ZipFile.cpp : API to use Zip files
//
// Part of the GameCode4 Application
//
// GameCode4 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 4th Edition" by Mike McShaffry and David
// "Rez" Graham, published by Charles River Media. 
// ISBN-10: 1133776574 | ISBN-13: 978-1133776574
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the authors a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1133776574/ref=olp_product_details?ie=UTF8&me=&seller=
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code: 
//    http://code.google.com/p/gamecode4/
//
// (c) Copyright 2012 Michael L. McShaffry and David Graham
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser GPL v3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// http://www.gnu.org/licenses/lgpl-3.0.txt for more details.
//
// You should have received a copy of the GNU Lesser GPL v3
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// --------------------------------------------------------------------------
// File:        ZipFile.cpp
//
// Purpose:     The implementation of a quick'n dirty ZIP file reader class.
//              Original code written by Javier Arevalo. 
//              Get zlib from http://www.cdrom.com/pub/infozip/zlib/
// --------------------------------------------------------------------------

//#include "GameCodeStd.h"
#include "FileSystem/ZipFile.h"
#include "Core/Core.h"

#include <cctype> // for std::tolower
#include <zlib.h>
#include <string>
#include <algorithm>
#include <cstdint>

namespace Rocket {
    namespace FileSystem {
        // --------------------------------------------------------------------------
        // Basic types.
        // --------------------------------------------------------------------------
        using dword = uint32_t;
        using word = uint16_t;
        using byte = uint8_t;

        // --------------------------------------------------------------------------
        // ZIP file structures. Note these have to be packed.
        // --------------------------------------------------------------------------

#pragma pack(1)
        // --------------------------------------------------------------------------
        // struct ZipFile::TZipLocalHeader					- Chapter 8, page 215
        // --------------------------------------------------------------------------
        struct ZipFile::TZipLocalHeader {
            enum {
                SIGNATURE = 0x04034b50,
            };
            dword   sig;
            word    version;
            word    flag;
            word    compression;      // Z_NO_COMPRESSION or Z_DEFLATED
            word    mod_time;
            word    mod_date;
            dword   crc32;
            dword   compressd_size;
            dword   uncompressd_size;
            word    file_name_len;         // Filename string follows header.
            word    extra_name_len;          // Extra field follows filename.
        };

        // --------------------------------------------------------------------------
        // struct ZipFile::TZipDirHeader					- Chapter 8, page 215
        // --------------------------------------------------------------------------
        struct ZipFile::TZipDirHeader {
            enum {
                SIGNATURE = 0x06054b50
            };
            dword   sig;
            word    nDisk;
            word    start_disk;
            word    dir_entries;
            word    total_dir_entries;
            dword   dir_size;
            dword   dir_offset;
            word    comment_len;
        };

        // --------------------------------------------------------------------------
        // struct ZipFile::TZipDirFileHeader					- Chapter 8, page 215
        // --------------------------------------------------------------------------
        struct ZipFile::TZipDirFileHeader {
            enum {
                SIGNATURE   = 0x02014b50
            };
            dword   sig;
            word    ver_made;
            word    ver_needed;
            word    flag;
            word    compression;      // COMP_xxxx
            word    mod_time;
            word    mod_date;
            dword   crc32;
            dword   compressd_size;            // Compressed size
            dword   uncompressd_size;           // Uncompressed size
            word    file_name_len;         // Filename string follows header.
            word    extra_name_len;          // Extra field follows filename.
            word    comment_len;          // Comment field follows extra field.
            word    disk_start;
            word    int_attr;
            dword   ext_attr;
            dword   hdr_offset;

            char* GetName   () const { return (char *)(this + 1);   }
            char* GetExtra  () const { return GetName() + file_name_len; }
            char* GetComment() const { return GetExtra() + extra_name_len; }
        };
#pragma pack()

        // --------------------------------------------------------------------------
        // Function:      Init
        // Purpose:       Initialize the object and read the zip file directory.
        // Parameters:    A stdio FILE* used for reading.
        // --------------------------------------------------------------------------
        bool ZipFile::Init(const std::string &file_name) {
            End();

            file_ = fopen(file_name.c_str(), "rb");
            if (!file_)
                return false;

            // Assuming no extra comment at the end, read the whole end record.
            TZipDirHeader dh;

            fseek(file_, -(int)sizeof(dh), SEEK_END);
            int32_t dhOffset = ftell(file_);
            memset(&dh, 0, sizeof(dh));
            fread(&dh, sizeof(dh), 1, file_);

            // Check
            if (dh.sig != TZipDirHeader::SIGNATURE)
                return false;

            // Go to the beginning of the directory.
            fseek(file_, dhOffset - dh.dir_size, SEEK_SET);

            // Allocate the data buffer, and read the whole thing.
            raw_data_ = new char[dh.dir_size + dh.dir_entries*sizeof(*m_papDir)];
            if (!raw_data_)
                return false;
            memset(raw_data_, 0, dh.dir_size + dh.dir_entries*sizeof(*m_papDir));
            fread(raw_data_, dh.dir_size, 1, file_);

            // Now process each entry.
            char *pfh = raw_data_;
            m_papDir = (const TZipDirFileHeader **)(raw_data_ + dh.dir_size);

            bool success = true;

            for (int i = 0; i < dh.dir_entries && success; i++) {
                TZipDirFileHeader& fh = *(TZipDirFileHeader*)pfh;

                // Store the address of nth file for quicker access.
                m_papDir[i] = &fh;

                // Check the directory entry integrity.
                if (fh.sig != TZipDirFileHeader::SIGNATURE) {
                    success = false;
                }
                else {
                    pfh += sizeof(fh);

                    // Convert UNIX slashes to DOS backlashes.
                    for (int j = 0; j < fh.file_name_len; j++)
                        if (pfh[j] == '/')
                        pfh[j] = '\\';

                    char fileName[_MAX_PATH];
                    memcpy(fileName, pfh, fh.file_name_len);
                    fileName[fh.file_name_len]=0;
                    strlwr(fileName);
                    std::string spath = fileName;
                    m_ZipContentsMap[spath] = i;

                    // Skip name, extra and comment fields.
                    pfh += fh.file_name_len + fh.extra_name_len + fh.comment_len;
                }
            }
            if (!success) {
                delete[] raw_data_;
            }
            else {
                entries_num_ = dh.dir_entries;
            }

            return success;
        }

        int ZipFile::Find(const std::string &path) const {
            std::string lowerCase = path;
            std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
            ZipContentsMap::const_iterator i = m_ZipContentsMap.find(lowerCase);
            if (i==m_ZipContentsMap.end())
                return -1;

            return i->second;
        }

        // --------------------------------------------------------------------------
        // Function:      End
        // Purpose:       Finish the object
        // Parameters:    
        // --------------------------------------------------------------------------
        void ZipFile::End() {
            m_ZipContentsMap.clear();
            //SAFE_DELETE_ARRAY(m_pDirData);
            delete[] raw_data_;
            entries_num_ = 0;
        }

        // --------------------------------------------------------------------------
        // Function:      GetFilename
        // Purpose:       Return the name of a file
        // Parameters:    The file index and the buffer where to store the filename
        // --------------------------------------------------------------------------
        std::string ZipFile::GetFilename(int i) const {
            std::string fileName = "";
            if (i >=0 && i < entries_num_) {
                char pszDest[_MAX_PATH];
                memcpy(pszDest, m_papDir[i]->GetName(), m_papDir[i]->file_name_len);
                pszDest[m_papDir[i]->file_name_len] = '\0';
                fileName = pszDest;
            }
            return fileName;
        }


        // --------------------------------------------------------------------------
        // Function:      GetFileLen
        // Purpose:       Return the length of a file so a buffer can be allocated
        // Parameters:    The file index.
        // --------------------------------------------------------------------------
        int ZipFile::GetFileLen(int i) const {
            if (i < 0 || i >= entries_num_)
                return -1;
            else
                return m_papDir[i]->uncompressd_size;
        }

        // --------------------------------------------------------------------------
        // Function:      ReadFile
        // Purpose:       Uncompress a complete file
        // Parameters:    The file index and the pre-allocated buffer
        // --------------------------------------------------------------------------
        bool ZipFile::ReadFile(int i, void* buffer) {
            if (buffer == NULL || i < 0 || i >= entries_num_) {
                return false;
            }

            // Quick'n dirty read, the whole file at once.
            // Ungood if the ZIP has huge files inside

            // Go to the actual file and read the local header.
            fseek(file_, m_papDir[i]->hdr_offset, SEEK_SET);
            TZipLocalHeader h;

            memset(&h, 0, sizeof(h));
            fread(&h, sizeof(h), 1, file_);
            if (h.sig != TZipLocalHeader::SIGNATURE) {
                return false;
            }

            // Skip extra fields
            fseek(file_, h.file_name_len + h.extra_name_len, SEEK_CUR);

            if (h.compression == Z_NO_COMPRESSION) {
                // Simply read in raw stored data.
                fread(buffer, h.compressd_size, 1, file_);
                return true;
            }
            else if (h.compression != Z_DEFLATED) {
                return false;
            }

            // Alloc compressed data buffer and read the whole stream
            char* pcData = new char[h.compressd_size];
            if (!pcData)
                return false;

            memset(pcData, 0, h.compressd_size);
            fread(pcData, h.compressd_size, 1, file_);

            bool ret = true;

            // Setup the inflate stream.
            z_stream stream;
            int err;

            stream.next_in = (Bytef*)pcData;
            stream.avail_in = (uInt)h.compressd_size;
            stream.next_out = (Bytef*)buffer;
            stream.avail_out = h.uncompressd_size;
            stream.zalloc = (alloc_func)0;
            stream.zfree = (free_func)0;

            // Perform inflation. wbits < 0 indicates no zlib header inside the data.
            err = inflateInit2(&stream, -MAX_WBITS);
            if (err == Z_OK) {
                err = inflate(&stream, Z_FINISH);
                inflateEnd(&stream);
                if (err == Z_STREAM_END) {
                    err = Z_OK;
                }
                inflateEnd(&stream);
            }
            if (err != Z_OK) {
                ret = false;
            }

            delete[] pcData;
            return ret;
        }

        // --------------------------------------------------------------------------
        // Function:      ReadLargeFile
        // Purpose:       Uncompress a complete file with callbacks.
        // Parameters:    The file index and the pre-allocated buffer
        // --------------------------------------------------------------------------
        bool ZipFile::ReadLargeFile(int i, void *buffer, void(*progressCallback)(int, bool &))
        {
            if (buffer == NULL || i < 0 || i >= entries_num_)
                return false;

            // Quick'n dirty read, the whole file at once.
            // Ungood if the ZIP has huge files inside

            // Go to the actual file and read the local header.
            fseek(file_, m_papDir[i]->hdr_offset, SEEK_SET);
            TZipLocalHeader h;

            memset(&h, 0, sizeof(h));
            fread(&h, sizeof(h), 1, file_);
            if (h.sig != TZipLocalHeader::SIGNATURE)
                return false;

            // Skip extra fields
            fseek(file_, h.file_name_len + h.extra_name_len, SEEK_CUR);

            if (h.compression == Z_NO_COMPRESSION) {
                // Simply read in raw stored data.
                fread(buffer, h.compressd_size, 1, file_);
                return true;
            }
            else if (h.compression != Z_DEFLATED)
                return false;

            // Alloc compressed data buffer and read the whole stream
            char *pcData = new char[h.compressd_size];
            if (!pcData)
                return false;

            memset(pcData, 0, h.compressd_size);
            fread(pcData, h.compressd_size, 1, file_);

            bool ret = true;

            // Setup the inflate stream.
            z_stream stream;
            int err;

            stream.next_in = (Bytef*)pcData;
            stream.avail_in = (uInt)h.compressd_size;
            stream.next_out = (Bytef*)buffer;
            stream.avail_out = (128 * 1024); //  read 128k at a time h.ucSize;
            stream.zalloc = (alloc_func)0;
            stream.zfree = (free_func)0;

            // Perform inflation. wbits < 0 indicates no zlib header inside the data.
            err = inflateInit2(&stream, -MAX_WBITS);
            if (err == Z_OK) {
                uInt count = 0;
                bool cancel = false;
                while (stream.total_in < (uInt)h.compressd_size && !cancel) {
                    err = inflate(&stream, Z_SYNC_FLUSH);
                    if (err == Z_STREAM_END) {
                        err = Z_OK;
                        break;
                    }
                    else if (err != Z_OK) {
                        RK_CORE_ASSERT(false, "Something happened.");
                        break;
                    }

                    stream.avail_out = (128 * 1024); 
                    stream.next_out += stream.total_out;

                    progressCallback(count * 100 / h.compressd_size, cancel);
                }
                inflateEnd(&stream);
            }
            if (err != Z_OK)
                ret = false;

            delete[] pcData;
            return ret;
        }


        /*******************************************************
        Example useage:

        void MakePath(const char *pszPath) {
            if (pszPath[0] == '\0')
                return;

            char buf[1000];
            const char *p = pszPath;

            //printf("MakePath(\"%s\")\n", pszPath);

            // Skip machine name in network paths like \\MyMachine\blah...
            if (p[0] == '\\' && p[1] == '\\')
                p = strchr(p+2, '\\');

            while (p != NULL && *p != '\0') {
                p = strchr(p, '\\');

                if (p) {
                    memcpy(buf, pszPath, p - pszPath);
                    buf[p - pszPath] = 0;
                    p++;
                }
                else
                    strcpy(buf, pszPath);

                if (buf[0] != '\0' && strcmp(buf, ".") && strcmp(buf, "..")) {
                    //printf("  Making path: \"%s\"\n", buf);
                    mkdir(buf);
                }
            }
        }

        void main(int argc, const char *argv[])
        {
            if (argc > 1) {
                FILE *f = fopen(argv[1], "rb");
                if (f) {
                    ZipFile zip;

                    if (true != zip.Init(f))
                        printf("Bad Zip file: \"%s\"\n", argv[1]);
                    else {
                        for (int i = 0; i < zip.GetNumFiles(); i++) {
                        int len = zip.GetFileLen(i);
                        char fname[1000];

                        zip.GetFilename(i, fname);

                        printf("File \"%s\" (%d bytes): ", fname, len);

                        char *pData = GCC_NEW char[len];
                        if (!pData)
                            printf("OUT OF MEMORY\n");
                        else if (true == zip.ReadFile(i, pData)) {
                            printf("OK\n");
                            char dpath[1000];

                            sprintf(dpath, "Data\\%s", fname);
                            char *p = strrchr(dpath, '\\');
                            if (p) {
                                *p = '\0';
                                MakePath(dpath);
                                *p = '\\';
                            }
                            FILE *fo = fopen(dpath, "wb");
                            if (fo) {
                                fwrite(pData, len, 1, fo);
                                fclose(fo);
                            }
                        }
                        else
                            printf("ERROR\n");
                        delete[] pData;
                    }
                    zip.End();
                }

                fclose(f);
            }
        }
        ******************************************************/
    }
}
