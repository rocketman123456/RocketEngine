target("RocketRenderCommon")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("render")
    add_options("profile_settings")
    add_deps("RocketLog", {public = true})
    if is_config("render", "vulkan") then
        add_deps("RocketVulkan", {public = true})
    end
target_end()
