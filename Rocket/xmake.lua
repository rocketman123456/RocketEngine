target("engine_main")
    set_kind("binary")
    add_files("Engine/*.cpp")
    add_packages(
        "glfw", "glad", {public = true}
    )
