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
set_languages("c99", "c++20")
--set_warnings("all", "error")

--
-- Add Required Modules
--
add_requireconfs("*", {configs = {shared = false}})

--
-- Set Render Options
--

--
-- Set Profile Options
--

--
-- Set mem_check Options
--

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
    -- 添加DEBUG编译宏
    add_defines("RK_PROFILE")
end

--
-- Add Sub Module
--
includes(
    "Rocket", 
    "UnitTest"
)
