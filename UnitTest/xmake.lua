target("memory_leak_test")
    set_kind("binary")
    add_files("memory_leak_test.cpp")
    add_files("../Rocket/Engine/Debug/MemLeak.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl", {public = true})
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo", {public = true})
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32", {public = true})
    end

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("vcpkg::glfw3")
    -- Add Render Dependent Libs
    if is_config("render_api", "opengl") then
        add_packages("vcpkg::glad")
    elseif is_config("render_api", "vulkan") then
        add_requires("vcpkg::vulkan", "vcpkg::volk")
    end
    -- Add Platform Dependent Libs
    if is_plat("macosx") then
        add_frameworks("Cocoa", "IOKit", "CoreVideo", {public = true})
        add_links("pthread", "m", "dl", {public = true})
    elseif is_plat("linux") then
        add_links("GL", "X11", "pthread", "m", "dl", {public = true})
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32", {public = true})
    end
    add_deps("RocketEngine", {inherit = true})


