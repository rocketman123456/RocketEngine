target("RocketUtils")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages("gsl", {public = true})
    add_deps(
        "RocketCore",
        "RocketLog",
        -- "RocketMemory",
        {public = true}
    )
target_end()
