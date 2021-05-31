target("RocketRender")
    set_kind("static")
    add_packages(
        "vcpkg::spdlog", 
        "vcpkg::fmt", 
        {public = true}
    )
    add_files("*.cpp")
    