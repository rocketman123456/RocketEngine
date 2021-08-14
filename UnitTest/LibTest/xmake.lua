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

target("yaml_test")
    set_kind("binary")
    add_files("yaml_test.cpp")
    add_packages("yaml-cpp", {public = true})
    add_deps("RocketEngine")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_deps("RocketEngine")
    add_packages("glfw", "glad")
    add_options("console_log")
    add_options("memory_check")
target_end()
