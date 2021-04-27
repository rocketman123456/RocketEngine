-- 如果当前编译模式是debug
if is_mode("debug") then
    -- 添加DEBUG编译宏
    --add_defines("NEW_OVERLOAD_IMPLEMENTATION_")
end

target("RocketEngine")
    set_kind("shared")
    --set_kind("binary")
    add_files("Debug/*.cpp")
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl", {public = true})
    end
    if is_plat("macosx") then
        add_frameworks("Cocoa", "IOKit", "CoreVideo")
    end
