target("RocketEngine")
    set_kind("static")
    add_files(
        "Engine/Config/*.cpp",
        "Engine/EventSystem/*.cpp",
        "Engine/Exception/*.cpp",
        "Engine/FileSystem/*.cpp",
        "Engine/Log/*.cpp", 
        "Engine/Memory/*.cpp",
        "Engine/Utils/*.cpp"
    )
    add_packages(
        "glfw", "glad", "minizip", "zlib", "yaml-cpp", "spdlog", "fmt", {public = true}
    )
    add_options("console_log")
    add_options("memory_check")
target_end()
