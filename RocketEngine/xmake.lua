-- 如果当前编译模式是debug
if is_mode("debug") then
    -- 添加DEBUG编译宏
    --add_defines("NEW_OVERLOAD_IMPLEMENTATION_")
end

target("RocketEngine")
    --set_kind("static")
    set_kind("binary")
    add_files("*.cpp", "Debug/*.cpp")
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    end
