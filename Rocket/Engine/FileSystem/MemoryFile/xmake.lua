target("RocketMemoryFile")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages("gsl", {public = true})
    add_deps(
        "RocketLog",
        "RocketMemory",
        {public = true}
    )
    set_license("LGPL-2.0")
target_end()
