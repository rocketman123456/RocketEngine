target("memory_leak_test")
    set_kind("binary")
    add_files("memory_leak_test.cpp")
    add_deps("RocketEngine")
    --if is_plat("linux", "macosx") then
    --    add_links("pthread", "m", "dl")
    --end

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("vcpkg::glfw3", "vcpkg::glad")
    if is_plat("macosx") then
        add_frameworks("Cocoa", "IOKit", "CoreVideo")
    end
