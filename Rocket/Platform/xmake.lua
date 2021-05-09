target("RocketPlatform")
    set_kind("static")
    if is_plat("linux", "macosx", "windows") then
        -- Add Packages
        add_packages(
            "vcpkg::glfw3"
        )
        -- Add Files
        add_files(
            "Desktop/Window/*.cpp"
        )
    end
