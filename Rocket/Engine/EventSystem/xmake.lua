target("RocketEngineEventSystem")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "eigen", 
        {public = true}
    )
    add_deps(
        "RocketEngineUtils",
        {public = true}
    )
target_end()