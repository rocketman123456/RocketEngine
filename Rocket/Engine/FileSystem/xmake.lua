includes(
    "Basic",
    "NativeFile",
    "MemoryFile",
    "ZipFile"
)

target("RocketFileSystem")
    set_kind("static")
    add_files("*.cpp")
    add_options("console_log")
    add_options("memory_check")
    add_options("profile_settings")
    add_packages("gsl", {public = true})
    add_deps(
        "RocketLog",
        "RocketFileBasic",
        "RocketNativeFile",
        "RocketMemoryFile",
        "RocketZipFile",
        {public = true}
    )
target_end()
