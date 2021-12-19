target("RocketCore")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("profile_settings")
    add_packages("mimalloc", {public = true})
target_end()
