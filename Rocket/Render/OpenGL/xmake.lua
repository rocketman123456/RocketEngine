target("RocketRender")
    set_kind("static")
    add_files("*.cpp")
    add_packages(
        "vcpkg::spdlog", 
        "vcpkg::fmt", 
        {public = true}
    )
