target("RocketMemory")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_deps(
        "RocketCore",
        "RocketLog",
        {public = true}
    )
target_end()
