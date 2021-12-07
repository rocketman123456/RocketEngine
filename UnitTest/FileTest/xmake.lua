target("file_system_test")
    set_kind("binary")
    add_files("file_system_test.cpp")
    add_deps("RocketLog")
    add_deps("RocketFileSystem")
    add_options("console_log")
    add_options("memory_check")
target_end()
