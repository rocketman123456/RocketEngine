target("RocketEngine")
    --set_kind("static")
    set_kind("binary")
    add_files("*.cpp", "Debug/*.cpp")
    if is_plat("linux") then
        add_links("pthread", "m", "dl")
    end
    if is_plat("macosx") then
        add_links("pthread", "m", "dl")
    end
    if is_plat("windows") then
    end