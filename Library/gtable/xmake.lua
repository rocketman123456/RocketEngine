target("gtable")
    set_kind("static")
    add_deps("wave", {public = true})
    add_files("*.c")
target_end()
