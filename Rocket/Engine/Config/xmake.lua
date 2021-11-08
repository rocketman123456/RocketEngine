target("RocketEngineConfig")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "yaml-cpp", 
        {public = true}
    )
    add_deps(
        "RocketEngineFileSystem",
        {public = true}
    )
target_end()
