target("RocketEngine")
    set_kind("static")
    -- Add Files
    add_files(
        "Core/*.cpp",
        "Debug/*.cpp",
        "Module/*.cpp",
        "Geometry/*.cpp"
    )
    -- Add Packages
    add_packages(
        "vcpkg::mimalloc", 
        "vcpkg::spdlog", 
        "vcpkg::fmt", 
        {public = true}
    )
