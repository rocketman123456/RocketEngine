target("RocketRender")
    set_kind("static")
    -- soft renderer will use default opengl renderer
    if is_config("render", "soft") then
        --print("soft")
    elseif is_config("render", "opengl") then
        --print("opengl")
    elseif is_config("render", "vulkan") then
        --print("vulkan")
    elseif is_config("render", "metal") then
        --print("metal")
    end
target_end()