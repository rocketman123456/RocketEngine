target("portaudio")
    set_kind("static")
    add_includedirs(
        "src/common"
    )
    add_files(
        "src/common/*.c"
    )
    if is_plat("windows") then
        add_files(
            "src/os/win/*.c",
            "src/hostapi/dsound/*.c",
            "src/hostapi/wmme/*.c",
            "src/hostapi/wasapi/*.c",
            "src/hostapi/wdmks/*.c"
        )
    else
        add_files(
            "src/os/unix/*.c"
        )
        if is_plat("macosx") then
            add_files(
                "src/hostapi/coreaudio/*.c"
            )
        end
    end
target_end()
