target("RocketEngine")
    set_kind("static")
    -- Add Files
    add_files(
        "Core/*.cpp",
        "Debug/*.cpp",
        "Module/*.cpp",
        "Parser/*.cpp",
        "Reflection/*.cpp",
        "Resource/*.cpp"
    )
    -- Add Packages
    add_packages(
        "vcpkg::spdlog",
        "vcpkg::fmt",
        "vcpkg::yaml-cpp",
        {public = true}
    )
