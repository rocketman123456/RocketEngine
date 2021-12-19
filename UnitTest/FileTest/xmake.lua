target("file_system_test")
    set_kind("binary")
    add_files("file_system_test.cpp")
    add_deps("RocketFileSystem")
    add_options("console_log")
    add_options("memory_check")
    add_options("profile_settings")
target_end()

-- target("file_test")
--     set_kind("binary")
--     add_files("file_test.cpp")
--     add_deps("RocketFileSystem")
--     add_options("console_log")
--     add_options("memory_check")
-- target_end()

-- target("zip_file_test")
--     set_kind("binary")
--     add_files("zip_file_test.cpp")
--     add_deps("RocketFileSystem")
--     add_options("console_log")
--     add_options("memory_check")
-- target_end()