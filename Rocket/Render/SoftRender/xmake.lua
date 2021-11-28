target("RocketRenderSoft")
    set_kind("static")
    add_files("Render/*.cpp")
    add_packages("eigen", "spdlog", {public = true})
    add_options("console_log")
    add_options("memory_check")
    add_deps(
        "RocketEngineLog",
        "RocketEngineMemory",
        "RocketEngineParser",
        {public = true}
    )
target_end()