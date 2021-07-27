target("RocketEngine")
    set_kind("static")
    add_files(
        "Engine/Log/*.cpp", 
        "Engine/Utils/*.cpp",
        "Engine/FileSystem/*.cpp",
        "Engine/Memory/*.cpp"
    )
    add_packages(
        "glfw", "glad", "minizip", "zlib", "spdlog", "fmt", {public = true}
    )
    add_options("console_log")
    add_options("memory_check")
target_end()
