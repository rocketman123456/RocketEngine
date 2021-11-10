target("zip_test")
    set_kind("binary")
    add_files("zip_test.cpp")
    add_packages("minizip", "zlib", {public = true})
target_end()

target("unzip_test")
    set_kind("binary")
    add_files("unzip_test.cpp")
    add_packages("minizip", "zlib", {public = true})
target_end()

target("torch_test")
    set_kind("binary")
    add_files("torch_test.cpp")
    add_packages("libtorch", {public = true})
target_end()

target("yaml_test")
    set_kind("binary")
    add_files("yaml_test.cpp")
    add_deps("RocketEngineLog")
    add_deps("RocketEngineUtils")
    add_deps("RocketEngineConfig")
    add_deps("RocketEngineFileSystem")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("audio_test")
    set_kind("binary")
    add_files("audio_test.cpp")
    add_deps("RocketEngineUtils")
    add_deps("RocketEngineLog")
    add_deps("RocketEngineUtils")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_deps("RocketEngineLog")
    add_deps("RocketEngineUtils")
    --add_deps("RocketEngineParser")
    add_packages("glfw", "glad")
    add_options("console_log")
    add_options("memory_check")
    add_packages("stb", {public = true})
target_end()

target("sdl_test")
    set_kind("binary")
    add_files("sdl_test.cpp")
    add_packages("libsdl", "glad")
    add_options("console_log")
    add_options("memory_check")
target_end()