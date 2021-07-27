target("list_test")
    set_kind("binary")
    add_files("list_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("stack_test")
    set_kind("binary")
    add_files("stack_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("bag_test")
    set_kind("binary")
    add_files("bag_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("queue_test")
    set_kind("binary")
    add_files("queue_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()