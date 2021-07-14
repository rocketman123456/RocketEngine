target("memory_test")
    set_kind("binary")
    add_files("memory_test.cpp")

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("glfw", "glad")