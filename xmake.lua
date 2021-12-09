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
    "mode.release", 
    "mode.minsizerel", 
    "mode.releasedbg"
)
set_languages("c99", "c++17")
--set_warnings("all", "error")

--
-- Add Required Modules
--
add_requires("spdlog v1.9.2", {system = false, configs = {shared = false, debug = true, fmt_external = false, cxflags = "-fPIC"}})
add_requires("gsl v3.1.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
add_requires("glad v0.1.34", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
add_requires("mimalloc 2.0.2", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
add_requires("libzip 1.8.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
--add_requires("miniz 2.1.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("fmt 8.0.1", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("minizip 1.2.11", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("zlib 1.2.11", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("yaml-cpp 0.7.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("openal-soft 1.21.1", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("libsndfile 1.0.30", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("stb", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("taskflow v3.2.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("eigen 3.4.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("glfw 3.3.4", {system = false, configs = {glfw_include = "none", shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("libsdl 2.0.16", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("bullet3 3.09", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("tinygltf v2.5.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("lua v5.4.2", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("luajit", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})

-- if is_plat("macosx", "linux", "windows") then
--     add_requires("libtorch v1.8.1", {system = false, configs = {shared = true, debug = true, cxflags = "-fPIC"}})
-- end

--
-- Set Render Options
--
option("render")
    set_default("soft")
    set_showmenu(true)
    set_values("soft", "opengl", "vulkan", "metal", "dx12", "dx11")
    set_description("The Render config option")
option_end()
if is_config("render", "soft") then
    --print("soft")
    add_defines("RK_SOFT")
elseif is_config("render", "opengl") then
    --print("opengl")
    add_defines("RK_OPENGL")
elseif is_config("render", "vulkan") then
    --print("vulkan")
    add_defines("RK_VULKAN")
elseif is_config("render", "metal") then
    --print("metal")
    add_defines("RK_METAL")
elseif is_config("render", "dx12") then
    --print("metal")
    add_defines("RK_DX12")
elseif is_config("render", "dx11") then
    --print("metal")
    add_defines("RK_DX11")
end

--
-- Set Log Options
--
option("console_log")
    set_default(true)
    set_showmenu(true)
    set_values(false, true)
    set_description("The Console Log config option")
    add_defines("RK_CONSOLE_LOG")
option_end()

--
-- Set Profile Options
--
option("profile_settings")
    set_default(false)
    set_showmenu(true)
    set_values(false, true)
    set_description("Profile Settings")
    add_defines("RK_PROFILE_SETTINGS")
option_end()

--
-- Set memory_check Options
--
option("memory_check")
    set_default(false)
    set_showmenu(true)
    set_values(false, true)
    set_description("The Memory Leak Check config option")
    add_defines("RK_MEMORY_CHECK")
option_end()

--
-- Set Include Dirs
--
add_includedirs(
    "Rocket/Engine",
    "Rocket/Render",
    "Rocket/Platform"
    -- "Rocket/Render/SoftRender",
)
-- if is_plat("windows", "macosx", "linux") then
--     add_includedirs("Rocket/Platform/Desktop/Common")
--     if is_plat("windows") then
--         add_includedirs("Rocket/Platform/Desktop/Windows")
--     elseif is_plat("macosx") then
--         add_includedirs("Rocket/Platform/Desktop/MacOS")
--     elseif is_plat("linux") then
--         add_includedirs("Rocket/Platform/Desktop/Linux")
--     end
-- elseif is_plat("android", "iphoneos") then
--     add_includedirs("Rocket/Platform/Mobile/Common")
--     if is_plat("android") then
--         add_includedirs("Rocket/Platform/Mobile/Android")
--     elseif is_plat("iphoneos") then
--         add_includedirs("Rocket/Platform/Mobile/iOS")
--     end
-- end

--
-- Set Platform Defines
--
if is_plat("linux", "macosx", "windows") then
    add_defines("RK_DESKTOP")
    --add_defines("GLFW_INCLUDE_NONE")
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
if is_mode("debug", "check") then
    -- 添加DEBUG编译宏
    add_defines("RK_DEBUG_INFO")
-- 如果是release模式
elseif is_mode("release", "minsizerel", "releasedbg") then
    -- 添加RELEASE编译宏
    add_defines("RK_RELEASE_INFO")
-- 如果是profile模式
elseif is_mode("profile") then
    -- 添加PROFILE编译宏
    add_defines("RK_PROFILE_INFO")
end

--
-- Add Sub Module
--
includes(
    "Rocket"
    --"Sandbox",
    -- "UnitTest"
)
