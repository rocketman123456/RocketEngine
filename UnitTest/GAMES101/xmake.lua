target("hw1")
    set_kind("binary")
    add_files("hw1.cpp")
    add_deps("RocketEngine")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("hw2")
    set_kind("binary")
    add_files("hw2.cpp")
    add_deps("RocketEngine")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("hw3")
    set_kind("binary")
    add_files("hw3.cpp")
    add_deps("RocketEngine")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
target_end()