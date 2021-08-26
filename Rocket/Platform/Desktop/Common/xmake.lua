target("RocketPlatform")
    set_kind("static")
    add_files(
        "Window/*.cpp"
    )
    add_packages(
        "glfw", 
        {public = true}
    )
target_end()