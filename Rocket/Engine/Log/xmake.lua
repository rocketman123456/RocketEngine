target("RocketLog")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("profile_settings")
    add_packages("spdlog", {public = true})
    if is_plat("windows") then
        add_packages("fmt", {public = true})
    end
    add_deps("RocketCore", {public = true})
target_end()
