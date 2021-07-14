add_includedirs(".")

target("new_trace")
    set_kind("binary")
    add_files("new_trace.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("membars")
    set_kind("binary")
    add_files("membars.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("raii")
    set_kind("binary")
    add_files("raii.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("atomic_sync")
    set_kind("binary")
    add_files("atomic_sync.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("dekker")
    set_kind("binary")
    add_files("dekker.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("dekker_n")
    set_kind("binary")
    add_files("dekker_n.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("deep_ptr")
    set_kind("binary")
    add_files("deep_ptr.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("unique")
    set_kind("binary")
    add_files("unique.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("memory_pool_implement")
    set_kind("binary")
    add_files("memory_pool_implement.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("producer_consumer")
    set_kind("binary")
    add_files("producer_consumer.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("thread_pool")
    set_kind("binary")
    add_files("thread_pool.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("thread_pool_1")
    set_kind("binary")
    add_files("thread_pool_1.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("timer")
    set_kind("binary")
    add_files("timer.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("singleton")
    set_kind("binary")
    add_files("singleton.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("compression")
    set_kind("binary")
    add_files("compression.cpp")
    add_packages("vcpkg::lz4", "vcpkg::xxhash")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32")
    end

target("serializer")
    set_kind("binary")
    add_files("serializer.cpp")
    -- Add Platform Dependent Libs
    if is_plat("linux", "macosx") then
        add_links("pthread", "m", "dl")
        if is_plat("macosx") then
            add_frameworks("Cocoa", "IOKit", "CoreVideo")
        end
    elseif is_plat("windows") then
        add_links("user32", "gdi32", "shell32", "kernel32", "ws2_32")
    end
