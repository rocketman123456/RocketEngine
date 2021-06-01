target("RocketEngine")
    set_kind("static")
    -- Add Packages
    add_packages(
        "vcpkg::spdlog",
        "vcpkg::fmt",
        "vcpkg::yaml-cpp",
        --"vcpkg::openal-soft",
        {public = true}
    )
    -- Add Files
    add_files(
        "Core/*.cpp",
        "Debug/*.cpp",
        "Module/*.cpp",
        "Parser/*.cpp",
        "Reflection/*.cpp",
        "Resource/*.cpp",
        "Utils/*.cpp"
    )
    
