# 1. 第一个简单项目<br>
第一个项目就从经典的Hello World开始吧！
## 1.1. xmake与cmake的项目配置<br>
写一个项目，总归需要进行项目配置管理。Windows平台可以选择VS来管理与编写代码，但是为了跨平台运行程序的考虑，需要选择别的方式进行管理。大多数跨平台的开源项目会使用CMAKE进行管理，但是CMAKE的语法复杂，学习成本高，在以前做项目时，也遇到使用CMAKE配置项目出现奇怪BUG，需要花很长时间解决的问题，分散了开发的注意力，反而降低效率。<br>
现在有国产的[xmake](http://xmake.io/)可以用于项目管理，语法简单很多，对初学者友好，所以本项目采用xmake进行项目配置。
xmake使用lua作为脚本语言，本项目的第一个配置文件如下所示：<br>
`xmake.lua`
```
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
-- Set Include Dirs
--
add_includedirs(
    "Rocket/Engine"
)

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
    "UnitTest"
)
```

## 1.2. 第一个hello world<br>
第一个简单的Hello World如下所示：<br>
`Rocket/xmake.lua`
```
target("hello_world")
    set_kind("binary")
    add_files("Engine/*.cpp")
```
`Rocket/Engine/main.cpp`
```
#include <iostream>

int main() {
    std::cout << "Hello World" << std::endl;

    return 0;
}
```

## 1.3. Git进行版本管理<br>
最后为了管理项目不同分支代码，选择git进行项目管理。
本项目的仓库地址为[RocketEngine](https://github.com/rocketman123456/RocketEngine)
