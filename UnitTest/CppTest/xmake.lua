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

target("L1_cache")
    set_kind("binary")
    add_files("L1_cache.cpp")
target_end()

target("raii")
    set_kind("binary")
    add_files("raii.cpp")
target_end()
