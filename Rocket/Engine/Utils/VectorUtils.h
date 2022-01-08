#pragma once
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

namespace Rocket {
    // e.g., eraseSelected({1, 2, 3, 4, 5}, {1, 3})  ->   {1, 3, 5}
    //                         ^     ^    2 and 4 get deleted
    template <class T, class Index = int> inline void EraseSelected(
        std::vector<T>& v, const std::vector<Index>& selection) {
        // cut off the elements moved to the end of the vector by std::stable_partition
        v.resize(std::distance(
                v.begin(),
                // the stable_partition moves any element whose index is in 'selection' to the end
                std::stable_partition(v.begin(), v.end(),
                    [&selection, &v](const T& item) {
                        return !std::binary_search(
                            selection.begin(), selection.end(),
                            /* std::distance(std::find(v.begin(), v.end(), item), v.begin()) - if you don't like the pointer arithmetic below */
                            static_cast<Index>(static_cast<const T*>(&item) - &v[0]));
                    }
                )
            )
        );
    }

    template <typename T>
    inline void MergeVectors(std::vector<T>& v1, const std::vector<T>& v2) {
        v1.insert( v1.end(), v2.begin(), v2.end() );
    }

    inline int AddUnique(std::vector<std::string>& files, const std::string& file) {
        if (file.empty()) {
            return -1;
        }
        auto i = std::find(std::begin(files), std::end(files), file);
        if (i == files.end()) {
            files.push_back(file);
            return (int)files.size() - 1;
        }
        return (int)std::distance(files.begin(), i);
    }
}