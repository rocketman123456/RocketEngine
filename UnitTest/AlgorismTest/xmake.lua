target("search_test")
    set_kind("binary")
    add_files("search_test.cpp")
target_end()

target("sort_test")
    set_kind("binary")
    add_files("sort_test.cpp")
    add_deps("RocketEngineLog")
    add_options("console_log")
    add_options("memory_check")
target_end()

target("vertex_delaunay_test")
    set_kind("binary")
    add_files("vertex_delaunay_test.cpp")
    add_deps("RocketEngineLog")
    add_deps("RocketEngineUtils")
    add_deps("RocketEngineGeometry")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "glfw", "glad",
        {public = true}
    )
target_end()

target("sphere_test")
    set_kind("binary")
    add_files("sphere_test.cpp")
    add_deps("RocketEngineLog")
    add_deps("RocketEngineUtils")
    add_deps("RocketEngineGeometry")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "glfw", "glad",
        {public = true}
    )
target_end()

target("curve_test")
    set_kind("binary")
    add_files("curve_test.cpp")
    add_deps("RocketEngineLog")
    add_deps("RocketEngineUtils")
    add_deps("RocketEngineGeometry")
    add_deps("RocketPlatformDesktop")
    add_deps("RocketRenderCommon")
    add_deps("RocketRenderSoft")
    add_deps("soft_render_util")
    add_options("console_log")
    add_options("memory_check")
    add_packages(
        "glfw", "glad",
        {public = true}
    )
target_end()
