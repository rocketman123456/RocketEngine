target("RocketEngine")
    set_kind("static")
    -- Add Files
    add_files(
        "Core/*.cpp",
        "Debug/*.cpp",
        "Module/*.cpp"
    )
    -- Add Packages
    add_packages("vcpkg::spdlog", "vcpkg::fmt", {public = true})
