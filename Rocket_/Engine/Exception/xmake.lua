target("RocketEngineExecption")
    set_kind("static")
    add_files("*.cpp")
    add_deps("RocketEngineLog")
    add_deps("RocketEngineMemory")
    add_options("console_log")
    add_options("memory_check")
target_end()