target("RocketEngineFileSystem")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "minizip", "zlib", 
        "openal-soft", "libsndfile",
        "mimalloc", 
        {public = true}
    )
    add_deps(
        "RocketEngineLog",
        "RocketEngineMemory",
        {public = true}
    )
    set_license("LGPL-2.0")
target_end()
