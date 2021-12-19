target("RocketPlatformCommon")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("render")
    add_options("profile_settings")
    if is_plat("windows", "macosx", "linux") then
        add_deps("RocketPlatformDesktop", {public = true})
    elseif is_plat("android", "iphoneos") then
        print("Mobile Not Support Now")
    end
    add_deps("RocketLog", {public = true})
target_end()
