-- Rocket Engine Config File

add_rules("mode.debug", "mode.release")
set_languages("c99", "c++17")
--set_warnings("all", "error")
--add_includedirs("/usr/include", "/usr/local/include")
--add_linkdirs("/usr/local/lib", "/usr/lib")
--add_links("tbox")
--add_syslinks("z", "pthread")
--add_cxflags("-stdnolib", "-fno-strict-aliasing")
--add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})

add_includedirs("RocketEngine")

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

includes("RocketEngine")

--
-- How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
