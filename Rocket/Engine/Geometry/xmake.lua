target("RocketEngineGeometry")
    set_kind("static")
    add_files("*.cpp", "MeshOperation/*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "eigen", 
        {public = true}
    )
    add_deps(
        "RocketEngineLog",
        "RocketEngineMemory",
        {public = true}
    )
target_end()
