add_includedirs(
    "Source"
)

target("nexus_ecs")
    set_kind("binary")
    add_files("Source/*.cpp")
    add_files("Source/Graphics/*.cpp")
    add_files("Source/Systems/*.cpp")
    add_packages("glad", "glfw")
target_end()
