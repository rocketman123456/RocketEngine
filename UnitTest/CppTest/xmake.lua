target("aos_vs_soa")
    set_kind("binary")
    add_files("aos_vs_soa.cpp")
target_end()

target("enum_2_string")
    set_kind("binary")
    add_files("enum_2_string.cpp")
target_end()

target("sort_compare")
    set_kind("binary")
    add_files("sort_compare.cpp")
target_end()

-- target("factory_test")
--     set_kind("binary")
--     add_files("factory_test.cpp")
-- target_end()

target("L1_cache")
    set_kind("binary")
    add_files("L1_cache.cpp")
    if is_plat("macosx") then
        add_frameworks("Cocoa", "IOKit", "CoreVideo")
    end
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end
target_end()

target("raii")
    set_kind("binary")
    add_files("raii.cpp")
target_end()

target("serializer")
    set_kind("binary")
    add_files("serializer.cpp")
    add_includedirs(".")
target_end()

-- target("singleton")
--     set_kind("binary")
--     add_files("singleton.cpp")
--     add_includedirs(".")
-- target_end()
