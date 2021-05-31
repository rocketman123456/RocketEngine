target("RocketRender")
    add_packages(
        "vcpkg::spdlog", 
        "vcpkg::fmt", 
        "vcpkg::glad",
        {public = true}
    )
    set_kind("static")
    add_files("*.cpp")
