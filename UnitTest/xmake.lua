add_includedirs(".")

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

target("sdl_test")
    set_kind("binary")
    add_deps("glad")
    add_packages("vcpkg::sdl2")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_links("iconv")
            add_frameworks("Cocoa", "IOKit", "CoreVideo", "CoreAudio", "CoreFoundation", "CoreAudioKit", "ForceFeedback", "Metal", "AudioToolbox", "Carbon")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32", "advapi32", "winmm", "ole32", "version", "setupapi", "oleaut32", "shell32", "uuid", "imm32")
        add_ldflags("/subsystem:console")
    end
    add_files("sdl_test.cpp")

--target("sdl_vulkan_test")
--    set_kind("binary")
--    add_packages("vcpkg::sdl2", "vcpkg::volk", "vulkan")
--    -- Add Platform Dependent Libs
--    if is_plat("linux", "macosx") then
--        add_links("pthread", "m", "dl")
--        if is_plat("macosx") then
--            add_links("iconv")
--            add_frameworks("Cocoa", "IOKit", "CoreVideo", "CoreAudio", "CoreFoundation", "CoreAudioKit", "ForceFeedback", "Metal", "AudioToolbox", "Carbon")
--        end
--    elseif is_plat("windows") then
--        add_links("user32", "gdi32", "shell32", "kernel32", "advapi32")
--        add_ldflags("/subsystem:console")
--    end
--    add_files("sdl_vulkan_test.cpp")

--target("engine_test")
--    set_kind("binary")
--    add_deps("RocketEngine", "RocketPlatform", "RocketRender")
--    -- Add Platform Dependent Libs
--    if is_plat("linux", "macosx") then
--        add_links("pthread", "m", "dl")
--        if is_plat("macosx") then
--            add_links("iconv")
--            add_frameworks("Cocoa", "IOKit", "CoreVideo", "CoreAudio", "CoreFoundation", "CoreAudioKit", "ForceFeedback", "Metal", "AudioToolbox", "Carbon")
--        end
--    elseif is_plat("windows") then
--        add_links("user32", "gdi32", "shell32", "kernel32", "advapi32")
--        add_ldflags("/subsystem:console")
--    end
--    add_files("engine_test.cpp")

--target("memory_leak_test")
--    set_kind("binary")
--    add_cxflags("-fsanitize=address", "-ftrapv")
--    add_mxflags("-fsanitize=address", "-ftrapv")
--    add_ldflags("-fsanitize=address")
--    add_files("memory_leak_test.cpp")
--    --add_packages("vcpkg::mimalloc")
--    -- Add Platform Dependent Libs
--    if is_plat("linux", "macosx") then
--        add_links("pthread", "m", "dl")
--        if is_plat("macosx") then
--            add_frameworks("Cocoa", "IOKit", "CoreVideo")
--        end
--    elseif is_plat("windows") then
--        add_links("user32", "gdi32", "shell32", "kernel32")
--    end

--target("hook_test")
--    set_kind("binary")
--    add_files("hook_test.cpp")
--    -- Add Platform Dependent Libs
--    if is_plat("linux", "macosx") then
--        add_links("pthread", "m", "dl")
--        if is_plat("macosx") then
--            add_frameworks("Cocoa", "IOKit", "CoreVideo")
--        end
--    elseif is_plat("windows") then
--        add_links("user32", "gdi32", "shell32", "kernel32")
--    end
--    --add_deps("subhook")

--target("glfw_test")
--    set_kind("binary")
--    add_files("glfw_test.cpp")
--    add_packages("vcpkg::glfw3")
--    -- Add Render Dependent Libs
--    if is_config("render_api", "opengl") then
--        add_packages("vcpkg::glad")
--    elseif is_config("render_api", "vulkan") then
--        add_packages("vcpkg::volk")
--    end
--    -- Add Platform Dependent Libs
--    if is_plat("linux", "macosx") then
--        add_links("pthread", "m", "dl")
--        if is_plat("macosx") then
--            add_frameworks("Cocoa", "IOKit", "CoreVideo")
--        end
--    elseif is_plat("windows") then
--        add_links("user32", "gdi32", "shell32", "kernel32")
--    end

includes("CPP")
