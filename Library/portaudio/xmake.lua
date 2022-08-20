target("portaudio")
    set_kind("static")
    add_includedirs("src/common")
    add_files("src/common/*.c")
    -- add_defines(
    --     "PA_LOG_API_CALLS",
    --     "PA_ENABLE_DEBUG_OUTPUT"
    -- )
    if is_plat("windows") then
        add_files(
            "src/os/win/*.c",
            -- "src/hostapi/dsound/*.c",
            "src/hostapi/wmme/*.c"
            -- "src/hostapi/wasapi/*.c"
            -- "src/hostapi/wdmks/*.c"
        )
        add_packages("winmm", "ole32", "uuid")
        add_defines("PA_USE_WMME")
    else
        add_files("src/os/unix/*.c")
        if is_plat("macosx") then
            add_files("src/hostapi/coreaudio/*.c")
            add_defines("PA_USE_COREAUDIO")
        else
            add_files("src/hostapi/alsa/*.c")
            add_packages("alsa")
            add_defines("PA_USE_ALSA")
        end
    end
target_end()
