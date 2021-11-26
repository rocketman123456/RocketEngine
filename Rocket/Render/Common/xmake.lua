target("RocketRenderCommon")
    set_kind("static")
    add_files(
        "RayTrace/*.cpp", 
        "Render/*.cpp"
    )
    add_packages("eigen", "spdlog", {public = true})
    add_options("console_log")
    add_options("memory_check")
target_end()