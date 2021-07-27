target("zip_test")
    set_kind("binary")
    add_files("zip_test.cpp")
    add_packages(
        "minizip", "zlib", {public = true}
    )
target_end()

target("unzip_test")
    set_kind("binary")
    add_files("unzip_test.cpp")
    add_packages(
        "minizip", "zlib", {public = true}
    )
target_end()
