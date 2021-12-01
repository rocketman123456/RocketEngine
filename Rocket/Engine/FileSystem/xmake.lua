includes("BasicFile")

target("RocketFileSystem")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_deps(
        "RocketLog",
        "RocketMemory",
        "RocketNativeFile",
        "RocketZipFile",
        "RocketMemoryFile",
        "RocketSpecialFile",
        {public = true}
    )
target_end()
