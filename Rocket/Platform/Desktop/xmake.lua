if is_plat("windows") then
    includes("Windows")
elseif is_plat("macosx") then
    includes("MacOS")
elseif is_plat("linux") then
    includes("Linux")
end
includes("Common")

target("RocketPlatform")
    set_kind("static")
    add_files(
        "Common/Window/*.cpp"
    )
    add_packages(
        "glfw", 
        {public = true}
    )
target_end()