-- Rocket Engine Config File

--
-- Set Project Basic
--
set_project("Rocket")
set_version("0.0.1")
add_rules(
    "mode.debug", 
    "mode.check", 
    "mode.profile",
    "mode.valgrind",
    "mode.release", 
    "mode.minsizerel", 
    "mode.releasedbg"
)
set_languages("c99", "c++20")
--set_warnings("all", "error")

--
-- Add Required Modules
--
add_requireconfs("*", {configs = {shared = false}})
add_requires(
    "vcpkg::spdlog",
    "vcpkg::fmt",
    "vcpkg::taskflow",
    "vcpkg::entt",
    "vcpkg::mimalloc",
    "vcpkg::zlib",
    "vcpkg::zstd",
    "vcpkg::bzip2",
    "vcpkg::liblzma",
    "vcpkg::lz4",
    "vcpkg::xxhash",
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
    if is_plat("windows") then
        add_includedirs("$(env VULKAN_SDK)/Include")
        add_linkdirs("$(env VULKAN_SDK)/Bin")
    elseif is_plat("macosx") then
        add_includedirs("$(env VULKAN_SDK)/Include")
        add_linkdirs("$(env VULKAN_SDK)/Bin")
    elseif is_plat("linux") then
        add_includedirs("$(env VULKAN_SDK)/Include")
        add_linkdirs("$(env VULKAN_SDK)/Bin")
    end
    add_requires("vcpkg::volk")
    --add_requires("vcpkg::vulkan-memory-allocator")
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
else
    add_defines("RK_OPENGL")
    printf("Use Deault OpenGL Render API\n")
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
    "Rocket/RHI",
    "External/glad/include"
    --"UnitTest"
)

--
-- Add ISPC Build Rule
--
--rule("ispc")
--    set_extensions(".ispc")
--    on_build_file(function (target, sourcefile, opt)
--        printf("Build ISPC\n")
--    end)

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
if is_mode("debug", "valgrind", "check") then
    -- 添加DEBUG编译宏
    add_defines("RK_DEBUG")
-- 如果是release模式
elseif is_mode("release", "minsizerel", "releasedbg") then
    -- 添加RELEASE编译宏
    add_defines("RK_RELEASE")
-- 如果是profile模式
elseif is_mode("profile") then
    -- 添加DEBUG编译宏
    add_defines("RK_PROFILE")
end

--
-- Add Sub Module
--
includes(
    "Rocket", 
    "External",
    "UnitTest"
)
