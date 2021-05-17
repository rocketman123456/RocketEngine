if is_config("render_api", "opengl") then
    includes("OpenGL")
elseif is_config("render_api", "vulkan") then
    includes("Vulkan")
elseif is_config("render_api", "soft_render") then 
    includes("SoftRender")
elseif is_config("render_api", "metal") then
    includes("Metal")
end
