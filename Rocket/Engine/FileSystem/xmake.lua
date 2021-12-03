includes(
    "NativeFile",
    "MemoryFile",
    "ZipFile"
)

target("RocketFileSystem")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_deps(
        "RocketLog",
        "RocketMemory",
        "RocketNativeFile",
        "RocketMemoryFile",
        "RocketZipFile",
        -- "RocketSpecialFile",
        {public = true}
    )
target_end()
