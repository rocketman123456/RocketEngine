if is_plat("windows", "macosx", "linux") then
    includes("Windows")
elseif is_plat("linux") then
    includes("Linux")
elseif is_plat("macosx") then
    includes("MacOS")
end

target("RocketPlatformDesktop")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("render")
    add_options("profile_settings")
    add_packages("glfw", {public = true})
    add_deps("RocketLog", {public = true})
target_end()