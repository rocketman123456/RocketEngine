target("subhook")
    add_defines("SUBHOOK_STATIC")
    if is_plat("linux", "macos") then
        add_defines("SUBHOOK_UNIX")
    elseif is_plat("macosx") then
        add_defines("SUBHOOK_UNIX")
    elseif is_plat("windows") then
        add_defines("SUBHOOK_WINDOWS")
    end
    if is_arch("x86") then 
        add_defines("SUBHOOK_X86")
    elseif is_arch("x86_64") then 
        add_defines("SUBHOOK_X86_64")
    end
    add_files("subhook/subhook.c")

