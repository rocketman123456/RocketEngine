target("search_test")
    set_kind("binary")
    add_files("search_test.cpp")
target_end()

target("sort_test")
    set_kind("binary")
    add_files("sort_test.cpp")
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("vertex_delaunay_test")
    set_kind("binary")
    add_files("vertex_delaunay_test.cpp")
    add_deps("RocketEngine")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("sphere_test")
    set_kind("binary")
    add_files("sphere_test.cpp")
    add_deps("RocketEngine")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
target_end()
