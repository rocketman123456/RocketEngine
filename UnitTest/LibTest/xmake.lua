target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("glfw", "glad", "stb")
    add_deps("RocketLog")
    add_deps("RocketUtils")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("taskflow_test")
    set_kind("binary")
    add_files("taskflow_test.cpp")
    add_packages("taskflow")
    add_deps("RocketLog")
    add_deps("RocketUtils")
    add_options("console_log")
    add_options("memory_check")
    if is_plat("macosx") then
        add_frameworks("Cocoa", "IOKit", "CoreVideo")
    end
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end
target_end()
