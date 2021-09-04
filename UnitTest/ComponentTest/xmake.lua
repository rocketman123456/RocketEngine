target("memory_test")
    set_kind("binary")
    add_files("memory_test.cpp")
    add_deps("RocketEngine")
    add_options("memory_check")
target_end()

target("hash_test")
    set_kind("binary")
    add_files("hash_test.cpp")
target_end()

target("log_test")
    set_kind("binary")
    add_files("log_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("window_test")
    set_kind("binary")
    add_files("window_test.cpp")
    add_deps("RocketEngine")
    add_deps("RocketPlatformDesktop")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("soft_render_test")
    set_kind("binary")
    add_files("soft_render_test.cpp")
    add_deps("RocketEngine")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("json_test")
    set_kind("binary")
    add_files("json_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("event_test")
    set_kind("binary")
    add_files("event_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("find_root_test")
    set_kind("binary")
    add_files("find_root_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("file_test")
    set_kind("binary")
    add_files("file_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("memory_pool_test")
    set_kind("binary")
    add_files("memory_pool_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("engine_test")
    set_kind("binary")
    add_files("engine_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()
