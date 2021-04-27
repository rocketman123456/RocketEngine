target("memory_leak_test")
    set_kind("binary")
    add_files("memory_leak_test.cpp")
    add_deps("RocketEngine", {inherit = true})

target("glfw_test")
    set_kind("binary")
    add_files("glfw_test.cpp")
    add_packages("vcpkg::glfw3")
    -- Add Render Dependent Libs
    if is_config("render_api", "opengl") then
        add_packages("vcpkg::glad")
    elseif is_config("render_api", "vulkan") then
        add_requires("vcpkg::vulkan", "vcpkg::volk")
    end
    add_deps("RocketEngine", {inherit = true})

