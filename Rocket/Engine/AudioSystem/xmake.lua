target("RocketEngineAudioSystem")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "openal-soft", "libsndfile",
        {public = true}
    )
    add_deps(
        "RocketEngineLog",
        "RocketEngineEventSystem",
        {public = true}
    )
    set_license("LGPL-2.0")
target_end()