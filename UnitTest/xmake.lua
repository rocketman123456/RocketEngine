target("memory_leak_test")
    --set_kind("static")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("RocketEngine")
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    end
target_end()