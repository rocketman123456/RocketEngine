-- Rocket Engine XMake Config File
set_project("Rocket")
set_version("0.0.1")

--
-- Set Project Basic
--
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
-- Set Render Options
--
option("render")
    set_default("vulkan")
    set_showmenu(true)
    set_values("soft", "opengl", "vulkan", "metal", "dx12", "dx11")
    set_description("The Render config option")
    set_category("Rocket")
option_end()
if is_config("render", "soft") then
    add_defines("RK_SOFT")
elseif is_config("render", "opengl") then
    add_defines("RK_OPENGL")
elseif is_config("render", "vulkan") then
    add_defines("RK_VULKAN")
elseif is_config("render", "metal") then
    add_defines("RK_METAL")
elseif is_config("render", "dx12") then
    add_defines("RK_DX12")
elseif is_config("render", "dx11") then
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
    set_category("Rocket")
    add_defines("RK_CONSOLE_LOG")
option_end()

--
-- Set Profile Options
--
option("profile_settings")
    set_default(true)
    set_showmenu(true)
    set_values(false, true)
    set_description("Profile Settings")
    set_category("Rocket")
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
    set_category("Rocket")
    add_defines("RK_MEMORY_CHECK")
option_end()

--
-- Add Required Modules
--
add_requires("spdlog v1.10.0", {system = false, configs = {shared = false, debug = true, fmt_external = false, cxflags = "-fPIC"}})
add_requires("mimalloc 2.0.6", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
add_requires("gsl v4.0.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
add_requires("lz4 v1.9.3", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
add_requires("eigen 3.4.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("libzip 1.8.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("yaml-cpp 0.7.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("stb 2021.09.10", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("tinygltf v2.5.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("glfw 3.3.6", {system = false, configs = {glfw_include = "none", shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("taskflow v3.2.0", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("concurrentqueue", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("lua v5.4.4", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})

-- For spdlog compile error
if is_plat("windows") then
    add_requires("fmt 8.1.1", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
end

if is_config("render", "soft") then
    -- add_requires("glad v0.1.36", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
    add_defines("RK_SOFT")
elseif is_config("render", "opengl") then
    -- add_requires("glad v0.1.36", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
    add_defines("RK_OPENGL")
elseif is_config("render", "vulkan") then
    add_defines("VK_NO_PROTOTYPES")
    -- add_requires("glad v0.1.36", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
    -- add_requires("vulkan-headers 1.2.189", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
    -- add_requires("volk 1.3.204", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
    -- add_requires("shaderc 2021.11.22", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
    add_defines("RK_VULKAN")
elseif is_config("render", "metal") then
    add_defines("RK_METAL")
elseif is_config("render", "dx12") then
    add_defines("RK_DX12")
elseif is_config("render", "dx11") then
    add_defines("RK_DX11")
end

-- add_requires("imgui v1.85", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("imguizmo 1.83", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("openal-soft 1.21.1", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("libsndfile 1.0.30", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("libsdl 2.0.16", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("bullet3 3.09", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- add_requires("luajit", {system = false, configs = {shared = false, debug = true, cxflags = "-fPIC"}})
-- if is_plat("macosx", "linux", "windows") then
--     add_requires("libtorch v1.8.1", {system = false, configs = {shared = true, debug = true, cxflags = "-fPIC"}})
-- end

--
-- Set Include Dirs
--
-- add_includedirs(
--    "Rocket/Engine",
--    "Rocket/Graphics",
--    "Rocket/Platform"
-- )

--
-- Set Platform Defines
--
if is_plat("linux", "macosx", "windows") then
    add_defines("RK_DESKTOP")
    if is_plat("linux") then
        add_defines("RK_LINUX")
    elseif is_plat("macosx") then
        add_defines("RK_MACOS")
    elseif is_plat("windows") then
        add_defines("RK_WINDOWS", "_CRT_SECURE_NO_WARNINGS")
    end
elseif is_plat("android", "iphoneos") then
    add_defines("RK_MOBILE")
    if is_plat("android") then
        add_defines("RK_ANDROID")
    elseif is_plat("iphoneos") then
        add_defines("RK_IPHONE")
    end
end

--
-- Set Compile Defines
--
if is_mode("debug", "check") then
    add_defines("RK_DEBUG_INFO")
elseif is_mode("release", "minsizerel", "releasedbg") then
    add_defines("RK_RELEASE_INFO")
elseif is_mode("profile") then
    add_defines("RK_PROFILE_INFO")
end

--
-- Add Sub Module
--
includes(
    "Rocket",
    "Sandbox",
    "UnitTest"
)
