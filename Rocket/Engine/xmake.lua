if is_mode("debug") then
    --add_defines("__NEW_OVERLOAD_IMPLEMENTATION__")
end

target("RocketEngine")
    set_kind("shared")
    add_files("Debug/*.cpp")
    add_packages("vcpkg::spdlog", "vcpkg::fmt", {public = true})
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl", {public = true})
    end
    if is_plat("macosx") then
        add_frameworks("Cocoa", "IOKit", "CoreVideo", {public = true})
    end
