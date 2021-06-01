-- Rocket Engine Config File

--
-- Set Project Basic
--
set_project("Rocket")
add_rules("mode.debug", "mode.release")
set_languages("c99", "c++20")
set_warnings("all", "error")

--
-- Add Required Modules
--
add_requires(
    "vcpkg::spdlog",
    "vcpkg::fmt",
    "vcpkg::taskflow",
    "vcpkg::entt",
    "vcpkg::mimalloc",
    "vcpkg::minizip-ng",
    "vcpkg::openal-soft",
    "vcpkg::eigen3",
    "vcpkg::yaml-cpp"
)
if is_plat("linux", "macosx", "windows") then
    add_requires(
        "vcpkg::glfw3",
        "vcpkg::sdl2"
    )
end

--
-- Set Render Options
--
option("render_api")
    set_showmenu(true)
    set_description("The Render API config option")
option_end()

if is_config("render_api", "opengl") then
    add_defines("RK_OPENGL")
    printf("OpenGL Render API\n")
elseif is_config("render_api", "opengl_es") then
    add_defines("RK_OPENGL_ES")
    printf("OpenGL ES Render API\n")
elseif is_config("render_api", "vulkan") then
    add_defines("RK_VULKAN")
    add_requires("vcpkg::volk", "vcpkg::vulkan-headers", "vcpkg::vulkan-memory-allocator")
    printf("Vulkan Render API\n")
elseif is_config("render_api", "soft_render") then 
    add_defines("RK_SOFT_RENDER")
    printf("Soft Render API\n")
elseif is_config("render_api", "metal") then
    add_defines("RK_METAL")
    printf("Metal Render API\n")
elseif is_config("render_api", "dx12") then
    add_defines("RK_DX12")
    printf("DirectX 12 Render API\n")
elseif is_config("render_api", "dx11") then
    add_defines("RK_DX11")
    printf("DirectX 11 Render API\n")
end

--
-- Set Profile Options
--
option("profile")
    set_showmenu(true)
    set_description("The Profile config option")
option_end()
if is_config("profile", "on") then
    add_defines("RK_PROFILE")
end

--
-- Set mem_check Options
--
option("mem_check")
    set_showmenu(true)
    set_description("The mem_check config option")
option_end()
if is_config("mem_check", "on") then
    add_defines("RK_MEM_CHECK")
end

--
-- Set Include Dirs
--
add_includedirs(
    "Rocket/Engine",
    "Rocket/Platform",
    "Rocket/RHI"
)

--
-- Set Platform Defines
--
if is_plat("linux", "macosx", "windows") then
    add_defines("RK_DESKTOP", "GLFW_INCLUDE_NONE")
    if is_plat("linux") then
        add_defines("RK_LINUX")
    elseif is_plat("macosx") then
        add_defines("RK_MACOS")
    elseif is_plat("windows") then
        add_defines("RK_WINDOWS", "_CRT_SECURE_NO_WARNINGS")
    end
end

--
-- Set Compile Defines
--
-- 如果当前编译模式是debug
if is_mode("debug") then
    -- 添加DEBUG编译宏
    add_defines("RK_DEBUG")
    -- 启用调试符号
    set_symbols("debug")
    -- 禁用优化
    set_optimize("none")
-- 如果是release或者profile模式
elseif is_mode("release", "profile") then
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

--
-- Add Sub Module
--
includes(
    "Rocket",  
    "UnitTest"
)

--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
