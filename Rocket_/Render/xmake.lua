includes("Common")

-- TODO : maybe reduce options
if is_config("render", "soft") then
    includes("SoftRender")
    includes("OpenGLRender")
elseif is_config("render", "opengl") then
    --includes("SoftRender")
    includes("OpenGLRender")
elseif is_config("render", "vulkan") then
    --includes("SoftRender")
    includes("VulkanRender")
elseif is_config("render", "metal") then
    --includes("SoftRender")
    includes("MetalRender")
elseif is_config("render", "dx11") then
    --includes("SoftRender")
    includes("DX11Render")
elseif is_config("render", "dx12") then
    --includes("SoftRender")
    includes("DX12Render")
end
