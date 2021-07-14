target("engine_test")
    set_kind("binary")
    add_files("Engine/*.cpp", "Engine/Log/*.cpp")
    add_packages(
        "glfw", "glad", "spdlog", "fmt", {public = true}
    )
