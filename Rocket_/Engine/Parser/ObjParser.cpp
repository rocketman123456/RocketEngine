#include "Parser/ObjParser.h"
#include "FileSystem/FileSystem.h"
#include "Log/Log.h"

#include <fstream>
#include <iostream>

// #define OBJL_CONSOLE_OUTPUT

namespace Rocket {
    int32_t ObjParser::Initialize() {
        if (full_path_.substr(full_path_.size() - 4, 4) != ".obj")
            return 1;

        loaded_meshes.clear();
        loaded_vertices.clear();
        loaded_indices.clear();
        loaded_materials.clear();

        FileBuffer buffer;
        FileSystem::LoadSync(path_, name_, FileOperateMode::READ_TEXT, buffer);
        content_ = std::string((char*)buffer.buffer);

        return 0;
    }

    void ObjParser::Finalize() {
        loaded_meshes.clear();
        loaded_vertices.clear();
        loaded_indices.clear();
        loaded_materials.clear();
    }

    bool ObjParser::Parse() {
        std::vector<Eigen::Vector3f> positions;
        std::vector<Eigen::Vector2f> tcoords;
        std::vector<Eigen::Vector3f> normals;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::vector<std::string> mesh_mat_names;

        bool listening = false;
        std::string meshname;

        Mesh temp_mesh;

#ifdef OBJL_CONSOLE_OUTPUT
        constexpr unsigned int output_every_nth = 1000;
        unsigned int output_indicator = output_every_nth;
#endif

        std::vector<std::string> result;
        Algorithm::Split(content_, result, "\n");
        
        for(auto curline : result) {
#ifdef OBJL_CONSOLE_OUTPUT
            std::cout << "curline: " << curline << std::endl;
            if ((output_indicator = ((output_indicator + 1) % output_every_nth)) == 1) {
                if (!meshname.empty()) {
                    std::cout << "\r- " << meshname;
                    std::cout << "\t| vertices > " << positions.size();
                    std::cout << "\t| texcoords > " << tcoords.size();
                    std::cout << "\t| normals > " << normals.size();
                    std::cout << "\t| triangles > " << (vertices.size() / 3);
                    std::cout << (!mesh_mat_names.empty() ? "\t| material: " + mesh_mat_names.back() : "");
                    std::cout << "\n";
                }
            }
#endif

            // Generate a Mesh Object or Prepare for an object to be created
            if (Algorithm::FirstToken(curline) == "o" || Algorithm::FirstToken(curline) == "g" || curline[0] == 'g') {
                if (!listening) {
                    listening = true;
                    if (Algorithm::FirstToken(curline) == "o" || Algorithm::FirstToken(curline) == "g")
                        meshname = Algorithm::Tail(curline);
                    else
                        meshname = "unnamed";
                } else {
                    // Generate the mesh to put into the array
                    if (!indices.empty() && !vertices.empty()) {
                        // Create Mesh
                        temp_mesh = Mesh(vertices, indices);
                        temp_mesh.mesh_name = meshname;

                        // Insert Mesh
                        loaded_meshes.push_back(temp_mesh);

                        // Cleanup
                        vertices.clear();
                        indices.clear();
                        meshname.clear();

                        meshname = Algorithm::Tail(curline);
                    } else {
                        if (Algorithm::FirstToken(curline) == "o" || Algorithm::FirstToken(curline) == "g")
                            meshname = Algorithm::Tail(curline);
                        else
                            meshname = "unnamed";
                    }
                }
#ifdef OBJL_CONSOLE_OUTPUT
                std::cout << std::endl;
                output_indicator = 0;
#endif
            }
            // Generate a Vertex Position
            if (Algorithm::FirstToken(curline) == "v") {
                std::vector<std::string> spos;
                Eigen::Vector3f vpos;
                Algorithm::Split(Algorithm::Tail(curline), spos, " ");

                vpos[0] = std::stof(spos[0]);
                vpos[1] = std::stof(spos[1]);
                vpos[2] = std::stof(spos[2]);

                positions.push_back(vpos);
            }
            // Generate a Vertex Texture Coordinate
            if (Algorithm::FirstToken(curline) == "vt") {
                std::vector<std::string> stex;
                Eigen::Vector2f vtex;
                Algorithm::Split(Algorithm::Tail(curline), stex, " ");

                vtex[0] = std::stof(stex[0]);
                vtex[1] = std::stof(stex[1]);

                tcoords.push_back(vtex);
            }
            // Generate a Vertex Normal;
            if (Algorithm::FirstToken(curline) == "vn") {
                std::vector<std::string> snor;
                Eigen::Vector3f vnor;
                Algorithm::Split(Algorithm::Tail(curline), snor, " ");

                vnor[0] = std::stof(snor[0]);
                vnor[1] = std::stof(snor[1]);
                vnor[2] = std::stof(snor[2]);

                normals.push_back(vnor);
            }
            // Generate a Face (vertices & indices)
            if (Algorithm::FirstToken(curline) == "f") {
                // Generate the vertices
                std::vector<Vertex> vVerts;
                GenVerticesFromRawOBJ(vVerts, positions, tcoords, normals, curline);

                // Add Vertices
                for (int i = 0; i < int(vVerts.size()); i++) {
                    vertices.push_back(vVerts[i]);
                    loaded_vertices.push_back(vVerts[i]);
                }

                std::vector<unsigned int> iIndices;

                VertexTriangluation(iIndices, vVerts);

                // Add Indices
                for (int i = 0; i < int(iIndices.size()); i++) {
                    unsigned int indnum = (unsigned int)((vertices.size()) - vVerts.size()) + iIndices[i];
                    indices.push_back(indnum);
                    indnum = (unsigned int)((loaded_vertices.size()) - vVerts.size()) + iIndices[i];
                    loaded_indices.push_back(indnum);

                }
            }
            // Get Mesh Material Name
            if (Algorithm::FirstToken(curline) == "usemtl") {
                mesh_mat_names.push_back(Algorithm::Tail(curline));

                // Create new Mesh, if Material changes within a group
                if (!indices.empty() && !vertices.empty()) {
                    // Create Mesh
                    temp_mesh = Mesh(vertices, indices);
                    temp_mesh.mesh_name = meshname;
                    int i = 2;
                    while(1) {
                        temp_mesh.mesh_name = meshname + "_" + std::to_string(i);

                        for (auto &m : loaded_meshes)
                            if (m.mesh_name == temp_mesh.mesh_name)
                                continue;
                        break;
                    }

                    // Insert Mesh
                    loaded_meshes.push_back(temp_mesh);

                    // Cleanup
                    vertices.clear();
                    indices.clear();
                }

#ifdef OBJL_CONSOLE_OUTPUT
                output_indicator = 0;
#endif
            }
            // Load Materials
            if (Algorithm::FirstToken(curline) == "mtllib") {
                // Generate LoadedMaterial

                // Generate a path to the material file
                std::vector<std::string> temp;
                Algorithm::Split(full_path_, temp, "/");

                std::string pathtomat = "";

                if (temp.size() != 1) {
                    for (int i = 0; i < temp.size() - 1; i++) {
                        pathtomat += temp[i] + "/";
                    }
                }

                pathtomat += Algorithm::Tail(curline);

#ifdef OBJL_CONSOLE_OUTPUT
                std::cout << std::endl << "- find materials in: " << pathtomat << std::endl;
#endif

                // Load Materials
                LoadMaterials(pathtomat);
            }
        }

#ifdef OBJL_CONSOLE_OUTPUT
        std::cout << std::endl;
#endif

        // Deal with last mesh
        if (!indices.empty() && !vertices.empty()) {
            // Create Mesh
            temp_mesh = Mesh(vertices, indices);
            temp_mesh.mesh_name = meshname;

            // Insert Mesh
            loaded_meshes.push_back(temp_mesh);
        }

        // Set Materials for each Mesh
        for (int i = 0; i < mesh_mat_names.size(); i++) {
            std::string matname = mesh_mat_names[i];

            // Find corresponding material name in loaded materials
            // when found copy material variables into mesh material
            for (int j = 0; j < loaded_materials.size(); j++) {
                if (loaded_materials[j].name == matname) {
                    loaded_meshes[i].mesh_material = loaded_materials[j];
                    break;
                }
            }
        }

        RK_INFO(File, "Mesh Size: {}", loaded_meshes.size());
        RK_INFO(File, "Vertices Size: {}", loaded_vertices.size());
        RK_INFO(File, "Indices Size: {}", loaded_indices.size());

        if (loaded_meshes.empty() && loaded_vertices.empty() && loaded_indices.empty())
            return false;
        else
            return true;
    }

    void ObjParser::GenVerticesFromRawOBJ(
        std::vector<Vertex>& oVerts,
        const std::vector<Eigen::Vector3f>& iPositions,
        const std::vector<Eigen::Vector2f>& iTCoords,
        const std::vector<Eigen::Vector3f>& iNormals,
        std::string icurline) {
        // Init variables
        std::vector<std::string> sface, svert;
        Vertex vVert;
        Algorithm::Split(Algorithm::Tail(icurline), sface, " ");

        bool noNormal = false;

        // For every given vertex do this
        for (int i = 0; i < int(sface.size()); i++) {
            // See What type the vertex is.
            int vtype;

            Algorithm::Split(sface[i], svert, "/");

            // Check for just position - v1
            if (svert.size() == 1) {
                // Only position
                vtype = 1;
            }

            // Check for position & texture - v1/vt1
            if (svert.size() == 2) {
                // Position & Texture
                vtype = 2;
            }

            // Check for Position, Texture and Normal - v1/vt1/vn1
            // or if Position and Normal - v1//vn1
            if (svert.size() == 3) {
                if (svert[1] != "") {
                    // Position, Texture, and Normal
                    vtype = 4;
                } else {
                    // Position & Normal
                    vtype = 3;
                }
            }

            // Calculate and store the vertex
            switch (vtype) {
                case 1: { // P
                    vVert.position = Algorithm::GetElement(iPositions, svert[0]);
                    vVert.texture_coordinate = Eigen::Vector2f(0, 0);
                    noNormal = true;
                    oVerts.push_back(vVert);
                    break;
                }
                case 2: { // P/T
                    vVert.position = Algorithm::GetElement(iPositions, svert[0]);
                    vVert.texture_coordinate = Algorithm::GetElement(iTCoords, svert[1]);
                    noNormal = true;
                    oVerts.push_back(vVert);
                    break;
                }
                case 3: { // P//N
                    vVert.position = Algorithm::GetElement(iPositions, svert[0]);
                    vVert.texture_coordinate = Eigen::Vector2f(0, 0);
                    vVert.normal = Algorithm::GetElement(iNormals, svert[2]);
                    oVerts.push_back(vVert);
                    break;
                }
                case 4: { // P/T/N
                    vVert.position = Algorithm::GetElement(iPositions, svert[0]);
                    vVert.texture_coordinate = Algorithm::GetElement(iTCoords, svert[1]);
                    vVert.normal = Algorithm::GetElement(iNormals, svert[2]);
                    oVerts.push_back(vVert);
                    break;
                }
                default: {
                    break;
                }
            }
        }

        // take care of missing normals
        // these may not be truly acurate but it is the
        // best they get for not compiling a mesh with normals
        if (noNormal) {
            Eigen::Vector3f A = oVerts[0].position - oVerts[1].position;
            Eigen::Vector3f B = oVerts[2].position - oVerts[1].position;
            Eigen::Vector3f normal = A.cross(B);

            for (int i = 0; i < int(oVerts.size()); i++) {
                oVerts[i].normal = normal;
            }
        }
    }

    // Triangulate a list of vertices into a face by printing
    //	inducies corresponding with triangles within it
    void ObjParser::VertexTriangluation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts) {
        // If there are 2 or less verts,
        // no triangle can be created,
        // so exit
        if (iVerts.size() < 3) {
            return;
        }
        // If it is a triangle no need to calculate it
        if (iVerts.size() == 3) {
            oIndices.push_back(0);
            oIndices.push_back(1);
            oIndices.push_back(2);
            return;
        }

        // Create a list of vertices
        std::vector<Vertex> tVerts = iVerts;

        while (true) {
            // For every vertex
            for (int i = 0; i < int(tVerts.size()); i++) {
                // pPrev = the previous vertex in the list
                Vertex pPrev;
                if (i == 0)
                    pPrev = tVerts[tVerts.size() - 1];
                else
                    pPrev = tVerts[i - 1];

                // pCur = the current vertex;
                Vertex pCur = tVerts[i];

                // pNext = the next vertex in the list
                Vertex pNext;
                if (i == tVerts.size() - 1)
                    pNext = tVerts[0];
                else
                    pNext = tVerts[i + 1];

                // Check to see if there are only 3 verts left
                // if so this is the last triangle
                if (tVerts.size() == 3) {
                    // Create a triangle from pCur, pPrev, pNext
                    for (int j = 0; j < int(tVerts.size()); j++) {
                        if (iVerts[j].position == pCur.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pPrev.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pNext.position)
                            oIndices.push_back(j);
                    }

                    tVerts.clear();
                    break;
                }
                if (tVerts.size() == 4) {
                    // Create a triangle from pCur, pPrev, pNext
                    for (int j = 0; j < int(iVerts.size()); j++) {
                        if (iVerts[j].position == pCur.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pPrev.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pNext.position)
                            oIndices.push_back(j);
                    }

                    Eigen::Vector3f tempVec;
                    for (int j = 0; j < int(tVerts.size()); j++) {
                        if (tVerts[j].position != pCur.position
                            && tVerts[j].position != pPrev.position
                            && tVerts[j].position != pNext.position) {
                            tempVec = tVerts[j].position;
                            break;
                        }
                    }

                    // Create a triangle from pCur, pPrev, pNext
                    for (int j = 0; j < int(iVerts.size()); j++) {
                        if (iVerts[j].position == pPrev.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pNext.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == tempVec)
                            oIndices.push_back(j);
                    }

                    tVerts.clear();
                    break;
                }

                // If Vertex is not an interior vertex
                float angle = Math::AngleBetweenV3(pPrev.position - pCur.position, pNext.position - pCur.position) * (180 / 3.14159265359);
                if (angle <= 0 && angle >= 180)
                    continue;

                // If any vertices are within this triangle
                bool inTri = false;
                for (int j = 0; j < int(iVerts.size()); j++) {
                    if (Math::InTriangle(iVerts[j].position, pPrev.position, pCur.position, pNext.position)
                        && iVerts[j].position != pPrev.position
                        && iVerts[j].position != pCur.position
                        && iVerts[j].position != pNext.position) {
                        inTri = true;
                        break;
                    }
                }
                if (inTri)
                    continue;

                // Create a triangle from pCur, pPrev, pNext
                for (int j = 0; j < int(iVerts.size()); j++) {
                    if (iVerts[j].position == pCur.position)
                        oIndices.push_back(j);
                    if (iVerts[j].position == pPrev.position)
                        oIndices.push_back(j);
                    if (iVerts[j].position == pNext.position)
                        oIndices.push_back(j);
                }

                // Delete pCur from the list
                for (int j = 0; j < int(tVerts.size()); j++) {
                    if (tVerts[j].position == pCur.position) {
                        tVerts.erase(tVerts.begin() + j);
                        break;
                    }
                }

                // reset i to the start
                // -1 since loop will add 1 to it
                i = -1;
            }

            // if no triangles were created
            if (oIndices.size() == 0)
                break;

            // if no more vertices
            if (tVerts.size() == 0)
                break;
        }
    }

    // Load Materials from .mtl file
    bool ObjParser::LoadMaterials(std::string path) {
        // If the file is not a material file return false
        if (path.substr(path.size() - 4, path.size()) != ".mtl")
            return false;

        auto file = FileSystem::OpenSync(path, FileOperateMode::READ_TEXT);
        FileBuffer buffer;
        file->ReadAll(buffer);
        auto content = std::string((char*)buffer.buffer);
        FileSystem::CloseSync(std::move(file));

        material_content_.push_back(content);

        // If the file is not found return false
        if (!content.empty())
            return false;

        Material tempMaterial;

        bool listening = false;

        // Go through each line looking for material variables
        std::vector<std::string> result;
        Algorithm::Split(content_, result, "\n");
        // std::string curline;
        for(auto curline : result) {
        // while (std::getline(file, curline))
        // {
            // new material and material name
            if (Algorithm::FirstToken(curline) == "newmtl") {
                if (!listening) {
                    listening = true;

                    if (curline.size() > 7)
                        tempMaterial.name = Algorithm::Tail(curline);
                    else
                        tempMaterial.name = "none";
                } else {
                    // Generate the material

                    // Push Back loaded Material
                    loaded_materials.push_back(tempMaterial);

                    // Clear Loaded Material
                    tempMaterial = Material();

                    if (curline.size() > 7)
                        tempMaterial.name = Algorithm::Tail(curline);
                    else
                        tempMaterial.name = "none";
                }
            }
            // Ambient Color
            if (Algorithm::FirstToken(curline) == "Ka") {
                std::vector<std::string> temp;
                Algorithm::Split(Algorithm::Tail(curline), temp, " ");

                if (temp.size() != 3)
                    continue;

                tempMaterial.ka[0] = std::stof(temp[0]);
                tempMaterial.ka[1] = std::stof(temp[1]);
                tempMaterial.ka[2] = std::stof(temp[2]);
            }
            // Diffuse Color
            if (Algorithm::FirstToken(curline) == "Kd") {
                std::vector<std::string> temp;
                Algorithm::Split(Algorithm::Tail(curline), temp, " ");

                if (temp.size() != 3)
                    continue;

                tempMaterial.kd[0] = std::stof(temp[0]);
                tempMaterial.kd[1] = std::stof(temp[1]);
                tempMaterial.kd[2] = std::stof(temp[2]);
            }
            // Specular Color
            if (Algorithm::FirstToken(curline) == "Ks") {
                std::vector<std::string> temp;
                Algorithm::Split(Algorithm::Tail(curline), temp, " ");

                if (temp.size() != 3)
                    continue;

                tempMaterial.ks[0] = std::stof(temp[0]);
                tempMaterial.ks[1] = std::stof(temp[1]);
                tempMaterial.ks[2] = std::stof(temp[2]);
            }
            // Specular Exponent
            if (Algorithm::FirstToken(curline) == "Ns")
                tempMaterial.ns = std::stof(Algorithm::Tail(curline));
            // Optical Density
            if (Algorithm::FirstToken(curline) == "Ni")
                tempMaterial.ni = std::stof(Algorithm::Tail(curline));
            // Dissolve
            if (Algorithm::FirstToken(curline) == "d")
                tempMaterial.d = std::stof(Algorithm::Tail(curline));
            // Illumination
            if (Algorithm::FirstToken(curline) == "illum")
                tempMaterial.illum = std::stoi(Algorithm::Tail(curline));
            // Ambient Texture Map
            if (Algorithm::FirstToken(curline) == "map_Ka")
                tempMaterial.map_ka = Algorithm::Tail(curline);
            // Diffuse Texture Map
            if (Algorithm::FirstToken(curline) == "map_Kd")
                tempMaterial.map_kd = Algorithm::Tail(curline);
            // Specular Texture Map
            if (Algorithm::FirstToken(curline) == "map_Ks")
                tempMaterial.map_ks = Algorithm::Tail(curline);
            // Specular Hightlight Map
            if (Algorithm::FirstToken(curline) == "map_Ns")
                tempMaterial.map_ns = Algorithm::Tail(curline);
            // Alpha Texture Map
            if (Algorithm::FirstToken(curline) == "map_d")
                tempMaterial.map_d = Algorithm::Tail(curline);
            // Bump Map
            if (Algorithm::FirstToken(curline) == "map_Bump" || 
                Algorithm::FirstToken(curline) == "map_bump" || 
                Algorithm::FirstToken(curline) == "bump") {
                tempMaterial.map_bump = Algorithm::Tail(curline);
            }
        }

        // Deal with last material

        // Push Back loaded Material
        loaded_materials.push_back(tempMaterial);

        // Test to see if anything was loaded
        // If not return false
        if (loaded_materials.empty())
            return false;
        // If so return true
        else
            return true;
    }
}
