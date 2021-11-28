target("RocketEngineParser")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "eigen", "stb",
        {public = true}
    )
    add_deps(
        "RocketEngineLog",
        "RocketEngineMemory",
        "RocketEngineAlgorithm",
        "RocketEngineFileSystem",
        "RocketEngineMath",
        {public = true}
    )
target_end()
