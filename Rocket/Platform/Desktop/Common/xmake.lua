target("RocketPlatformDesktop")
    set_kind("static")
    add_files(
        "Window/*.cpp"
    )
    add_packages(
        "glfw", 
        {public = true}
    )
    add_deps(
        "RocketEngineLog",
        "RocketEngineMemory",
        {public = true}
    )
target_end()