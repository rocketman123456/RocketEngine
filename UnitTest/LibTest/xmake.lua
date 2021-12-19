target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("glfw", "glad", "stb")
    add_deps("RocketLog")
    add_deps("RocketUtils")
    add_options("console_log")
    add_options("memory_check")
target_end()
