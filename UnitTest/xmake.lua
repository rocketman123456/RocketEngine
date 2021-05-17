target("taskflow_test_01")
    set_kind("binary")
    add_files("taskflow_test_01.cpp")
    add_packages("vcpkg::taskflow")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("engine_test")
    set_kind("binary")
    add_files("engine_test.cpp")
    add_packages("vcpkg::taskflow", "vcpkg::mimalloc")
    add_deps("RocketEngine", "RocketPlatform")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
        add_ldflags("/subsystem:console")
    end

--target("memory_leak_test")
--    set_kind("binary")
--    add_files("memory_leak_test.cpp")
--    add_packages("vcpkg::mimalloc")
--    -- Add Platform Dependent Libs
--    if is_plat("linux", "macosx") then
--        add_links("pthread", "m", "dl")
--        if is_plat("macosx") then
--            add_frameworks("Cocoa", "IOKit", "CoreVideo")
--        end
--    elseif is_plat("windows") then
--        add_links("user32", "gdi32", "shell32", "kernel32")
--    end

target("hook_test")
    set_kind("binary")
    add_files("hook_test.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end
    --add_deps("subhook")

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("vcpkg::glfw3")
    -- Add Render Dependent Libs
    if is_config("render_api", "opengl") then
        add_packages("vcpkg::glad")
    elseif is_config("render_api", "vulkan") then
        add_packages("vcpkg::volk")
    end
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end
