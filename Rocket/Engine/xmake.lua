target("RocketEngine")
    set_kind("static")
    add_files(
        "Algorism/UnionFind/*.cpp",
        "AudioSystem/*.cpp",
        "Application/*.cpp",
        "Config/*.cpp",
        "EventSystem/*.cpp",
        "Exception/*.cpp",
        "FileSystem/*.cpp",
        "Geometry/*.cpp",
        --"Geometry/Mesh/*.cpp",
        "Geometry/MeshOperation/*.cpp",
        "Log/*.cpp", 
        "Math/*.cpp", 
        "Parser/*.cpp",
        "Physics/*.cpp",
        "Memory/*.cpp",
        "Scene/*.cpp",
        "Utils/*.cpp"
    )
    add_packages(
        "glfw", 
        "glad", 
        "eigen", 
        "minizip", "zlib", 
        "yaml-cpp", 
        "spdlog", "fmt", 
        "openal-soft", "libsndfile",
        "nlohmann_json",
        "tinygltf",
        {public = true}
    )
    add_options("console_log")
    add_options("memory_check")
    set_license("LGPL-2.0")
target_end()