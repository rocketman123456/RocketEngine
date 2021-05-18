target("RocketRender")
    set_kind("static")
    add_files("*.cpp")
    add_packages(
        "vcpkg::spdlog", 
        "vcpkg::fmt", 
        "vcpkg::volk",
        {public = true}
    )