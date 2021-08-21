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
add_requires("spdlog", {system = false, shared = false, debug = true, configs = {fmt_external = false, cxflags = "-fPIC"}})
add_requires("fmt", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
add_requires("glad", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
add_requires("minizip", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
add_requires("zlib", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
add_requires("yaml-cpp", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
add_requires("openal-soft", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
add_requires("libsndfile", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
add_requires("nlohmann_json", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})

if is_plat("linux", "macosx", "windows") then
    add_requires("glfw", {system = false, shared = false, debug = true, glfw_include = "none", configs = {cxflags = "-fPIC"}})
    add_requires("libsdl", {system = false, shared = false, debug = true, configs = {cxflags = "-fPIC"}})
end

--
-- Set Render Options
--
option("render")
    set_default("vulkan")
    set_showmenu(true)
    set_values("soft", "opengl", "vulkan", "metal")
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

--
-- Set memory_check Options
--
option("memory_check")
    set_default(true)
    set_showmenu(true)
    set_values(false, true)
    set_description("The Memory Leak Check config option")
    add_defines("RK_MEMORY_CHECK")
option_end()

--
-- Set Include Dirs
--
add_includedirs(
    "Rocket/Engine"
)

--
-- Add ISPC Build Rule
--

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
    add_defines("RK_DEBUG")
-- 如果是release模式
elseif is_mode("release", "minsizerel", "releasedbg") then
    -- 添加RELEASE编译宏
    add_defines("RK_RELEASE")
-- 如果是profile模式
elseif is_mode("profile") then
    -- 添加PROFILE编译宏
    add_defines("RK_PROFILE")
end

--
-- Add Sub Module
--
includes(
    "Rocket", 
    "Sandbox",
    "UnitTest"
)
