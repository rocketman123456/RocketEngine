target("RocketRenderSoft")
    set_kind("static")
    add_files("Render/*.cpp")
    add_packages("eigen", "spdlog", {public = true})
    add_options("console_log")
    add_options("memory_check")
    add_deps("RocketEngineParser")
target_end()