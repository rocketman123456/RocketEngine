if is_config("render", "vulkan") then
    target("window_test")
        set_kind("binary")
        add_files("window_test.cpp")
        add_deps("RocketLog")
        add_deps("RocketPlatformCommon")
        add_deps("RocketPlatformDesktop")
        add_deps("RocketRenderCommon")
        add_options("console_log")
        add_options("memory_check")
        add_options("profile_settings")
    target_end()
end