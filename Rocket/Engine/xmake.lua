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
        "Log/*.cpp", 
        "Parser/*.cpp",
        "Physics/*.cpp",
        "Physics/BasicElement/*.cpp",
        "Physics/MeshOperation/*.cpp",
        "Memory/*.cpp",
        "Utils/*.cpp"
    )
    add_packages(
        "glfw", 
        "glad", 
        "minizip", "zlib", 
        "yaml-cpp", 
        "spdlog", "fmt", 
        "openal-soft", "libsndfile",
        "nlohmann_json",
        {public = true}
    )
    add_options("console_log")
    add_options("memory_check")
    set_license("LGPL-2.0")
target_end()