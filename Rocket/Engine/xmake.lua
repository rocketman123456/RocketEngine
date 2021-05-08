target("RocketEngine")
    set_kind("static")
    -- Add Files
    add_files(
        "Core/*.cpp",
        "Debug/*.cpp",
        "Module/*cpp"
    )
    -- Add Packages
    add_packages("vcpkg::spdlog", "vcpkg::fmt", {public = true})
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl", {public = true})
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo", {public = true})
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32", {public = true})
    end
