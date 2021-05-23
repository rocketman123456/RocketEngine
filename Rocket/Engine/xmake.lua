target("RocketEngine")
    set_kind("static")
    -- Add Files
    add_files(
        "Core/*.cpp",
        "Debug/*.cpp",
        "Module/*.cpp",
        "Geometry/*.cpp",
        "Reflection/*.cpp"
    )
    -- Add Packages
    add_packages(
        "vcpkg::spdlog",
        "vcpkg::fmt",
        {public = true}
    )
