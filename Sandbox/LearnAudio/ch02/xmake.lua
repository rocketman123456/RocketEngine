target("sf2float")
    set_kind("binary")
    add_deps("portsf")
    add_files("sf2float.c")
target_end()

target("sfgain")
    set_kind("binary")
    add_deps("portsf")
    add_files("sfgain.c")
target_end()
