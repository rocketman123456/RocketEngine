target("memory_test")
    set_kind("binary")
    add_files("memory_test.cpp")

target("search_test")
    set_kind("binary")
    add_files("search_test.cpp")

target("list_test")
    set_kind("binary")
    add_files("list_test.cpp")

target("stack_test")
    set_kind("binary")
    add_files("stack_test.cpp")

target("bag_test")
    set_kind("binary")
    add_files("bag_test.cpp")

target("sort_test")
    set_kind("binary")
    add_files("sort_test.cpp")

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("glfw", "glad")

target("log_test")
    set_kind("binary")
    add_files("log_test.cpp", "../Rocket/Engine/Log/Log.cpp")
    add_packages("spdlog", "fmt")
