target("RocketMemory")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("profile_settings")
    add_deps(
        "RocketCore",
        "RocketLog",
        {public = true}
    )
target_end()
