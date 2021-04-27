-- 如果当前编译模式是debug
if is_mode("debug") then
    -- 添加DEBUG编译宏
    --add_defines("__NEW_OVERLOAD_IMPLEMENTATION__")
end

target("RocketEngine")
    set_kind("static")
    add_files("Debug/*.cpp")
    add_packages("vcpkg::spdlog", "vcpkg::fmt", {public = true})
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl", {public = true})
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo", {public = true})
        end
    end
    if is_plat("windows") then
        add_links("user32", "gdi32", "opengl32", "shell32", "kernel32", {public = true})
    end
