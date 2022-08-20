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
        add_defines("PA_USE_WMME")
        add_packages("winmm", "ole32", "uuid")
    else
        add_files("src/os/unix/*.c")
        if is_plat("macosx") then
            add_files("src/hostapi/coreaudio/*.c")
            add_defines("PA_USE_COREAUDIO")
            add_frameworks(
                "CoreAudio", 
                "AudioToolbox", 
                "AudioUnit", 
                "CoreFoundation", 
                "CoreServices",
                {public = true}
            )
        else
            add_files("src/hostapi/alsa/*.c")
            add_defines("PA_USE_ALSA")
            add_packages("alsa")
        end
    end
target_end()
