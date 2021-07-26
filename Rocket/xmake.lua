target("RocketEngine")
    set_kind("static")
    add_files(
        "Engine/Log/*.cpp", 
        "Engine/Utils/*.cpp"
    )
    add_packages(
        "glfw", "glad", "spdlog", "fmt", {public = true}
    )
    add_options("console_log")
    add_options("memory_check")
target_end()
