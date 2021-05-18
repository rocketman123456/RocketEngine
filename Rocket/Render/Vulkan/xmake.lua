target("RocketRender")
    set_kind("static")
    add_files("*.cpp")
    add_packages(
        "vcpkg::mimalloc", 
        "vcpkg::spdlog", 
        "vcpkg::fmt", 
        "vcpkg::volk",
        {public = true}
    )