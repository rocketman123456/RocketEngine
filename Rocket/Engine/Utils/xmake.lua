target("RocketEngineUtils")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "eigen", 
        "openal-soft", "libsndfile",
        {public = true}
    )
    add_deps(
        "RocketEngineLog",
        {public = true}
    )
    set_license("LGPL-2.0")
target_end()
