-- Rocket Engine Config File

set_project("Rocket")

add_rules("mode.debug", "mode.release")
set_languages("c99", "c++17")
set_warnings("all", "error")
--add_includedirs("/usr/include", "/usr/local/include")
--add_linkdirs("/usr/local/lib", "/usr/lib")
--add_links("tbox")
--add_syslinks("z", "pthread")
--add_cxflags("-stdnolib", "-fno-strict-aliasing")
--add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})

add_requires(
    "vcpkg::glfw3",
    "vcpkg::spdlog",
    "vcpkg::fmt",
    "vcpkg::taskflow",
    "vcpkg::entt"
)

option("render_api")
    set_showmenu(true)
    set_description("The Render API config option")
option_end()

if is_config("render_api", "opengl") then
    add_requires(
        "vcpkg::glad"
    )
end
if is_config("render_api", "vulkan") then
    add_requires(
        "vcpkg::vulkan",
        "vcpkg::volk"
    )
end

option("profile")
    set_showmenu(true)
    set_description("The Profile config option")
option_end()

if is_config("profile", "on") then
    add_defines("RK_PROFILE")
end

add_includedirs(
    "Rocket/Engine"
)

if is_plat("linux") then
    add_defines("RK_LINUX")
end
if is_plat("macosx") then
    add_defines("RK_MACOS")
end
if is_plat("windows") then
    add_defines("RK_WINDOWS")
end

-- 如果当前编译模式是debug
if is_mode("debug") then
    -- 添加DEBUG编译宏
    add_defines("RK_DEBUG")
    -- 启用调试符号
    set_symbols("debug")
    -- 禁用优化
    set_optimize("none")
end

-- 如果是release或者profile模式
if is_mode("release", "profile") then
    -- 如果是release模式
    if is_mode("release") then
        -- 添加DEBUG编译宏
        add_defines("RK_RELEASE")
        -- 隐藏符号
        set_symbols("hidden")
        -- strip所有符号
        set_strip("all")
        -- 忽略帧指针
        add_cxflags("-fomit-frame-pointer")
        add_mxflags("-fomit-frame-pointer")
        -- 优化
        set_optimize("fastest")
    -- 如果是profile模式
    else
        -- 添加DEBUG编译宏
        add_defines("RK_PROFILE")
        -- 启用调试符号
        set_symbols("debug")
        -- 优化
        set_optimize("fastest")
    end
    -- 添加扩展指令集
    add_vectorexts("sse2", "sse3", "ssse3", "mmx")
end

task("hello")
    -- 设置运行脚本
    on_run(function ()
        print("hello xmake!")
    end)

target("test")
    on_load(function (target)
        print("on load test")
    end)
    on_build(function (target)
        print("on build test")
    end)
    on_link(function (target)
        print("on link test")
    end)
    after_build(function (target)
        -- 导入task模块
        import("core.project.task")
        -- 运行hello任务
        task.run("hello")
    end)

includes("Rocket", "UnitTest")

--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
