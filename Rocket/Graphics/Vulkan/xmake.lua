target("RocketVulkan")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("render")
    add_options("profile_settings")
    add_packages(
        "vulkan-headers",
        "volk",
        "glfw",
        -- "shaderc",
        {public = true}
    )
    add_deps("RocketLog", {public = true})
target_end()
