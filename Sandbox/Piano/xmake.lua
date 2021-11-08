target("piano")
    set_kind("binary")
    add_files("piano.cpp")
    add_deps(
        "RocketEngineLog",
        "RocketEngineEventSystem",
        "RocketEngineFileSystem",
        "RocketEngineAudioSystem"
    )
    add_deps("RocketEngineParser")
    add_deps("RocketEngineUtils")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "glfw", "glad",
        {public = true}
    )
target_end()