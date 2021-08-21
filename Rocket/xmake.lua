target("RocketEngine")
    set_kind("static")
    add_files(
        "Engine/AudioSystem/*.cpp",
        "Engine/Config/*.cpp",
        "Engine/EventSystem/*.cpp",
        "Engine/Exception/*.cpp",
        "Engine/FileSystem/*.cpp",
        "Engine/Log/*.cpp", 
        "Engine/Parser/*.cpp",
        "Engine/Memory/*.cpp",
        "Engine/Utils/*.cpp"
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
