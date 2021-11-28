target("RocketRenderCommon")
    set_kind("static")
    add_files(
        "RayTrace/*.cpp", 
        "Render/*.cpp"
    )
    add_packages("eigen", "spdlog", {public = true})
    add_options("console_log")
    add_options("memory_check")
    add_deps(
        "RocketEngineLog",
        "RocketEngineMemory",
        {public = true}
    )
target_end()