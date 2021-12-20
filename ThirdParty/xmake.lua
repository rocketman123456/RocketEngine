target("easy_profiler")
    set_kind("static")
    add_defines(
        "EASY_PROFILER_STATIC=1", 
        "EASY_PROFILER_VERSION_MAJOR=2",
        "EASY_PROFILER_VERSION_MINOR=1",
        "EASY_PROFILER_VERSION_PATCH=0",
        "BUILD_WITH_EASY_PROFILER",
        "EASY_DEFAULT_PORT=28077",
        "EASY_OPTION_PRETTY_PRINT_FUNCTIONS",
        "EASY_OPTION_BUILTIN_COLORS",
        "EASY_OPTION_LOG_ENABLED",
        "EASY_CHRONO_HIGHRES_CLOCK"
    )
    if is_plat("windows") then 
        add_defines(
            "EASY_OPTION_EVENT_TRACING_ENABLED",
            "EASY_OPTION_LOW_PRIORITY_EVENT_TRACING"
        )
    end
    add_includedirs("easy_profiler/easy_profiler_core/include")
    add_files("easy_profiler/easy_profiler_core/*.cpp")
target_end()

target("easy_profiler_converter")
    set_kind("binary")
    add_defines(
        "EASY_PROFILER_STATIC=1", 
        "EASY_PROFILER_VERSION_MAJOR=2",
        "EASY_PROFILER_VERSION_MINOR=1",
        "EASY_PROFILER_VERSION_PATCH=0",
        "BUILD_WITH_EASY_PROFILER",
        "EASY_DEFAULT_PORT=28077",
        "EASY_OPTION_PRETTY_PRINT_FUNCTIONS",
        "EASY_OPTION_BUILTIN_COLORS",
        "EASY_OPTION_LOG_ENABLED",
        "EASY_CHRONO_HIGHRES_CLOCK"
    )
    if is_plat("windows") then 
        add_defines(
            "EASY_OPTION_EVENT_TRACING_ENABLED",
            "EASY_OPTION_LOW_PRIORITY_EVENT_TRACING"
        )
    end
    add_includedirs("easy_profiler/easy_profiler_core/include")
    add_includedirs("easy_profiler/easy_profiler_converter/include")
    add_files("easy_profiler/easy_profiler_converter/*.cpp")
    add_deps("easy_profiler")
target_end()

target("OptickCore")
    set_kind("static")
    add_defines(
        "OPTICK_EXPORTS=0",
        "OPTICK_ENABLE_GPU=0",
        "OPTICK_ENABLE_GPU_VULKAN=0",
        "OPTICK_ENABLE_GPU_D3D12=0"
    )
    add_includedirs("optick/src")
    add_files("optick/src/*.cpp")
target_end()