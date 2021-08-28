includes("Common")

if is_plat("windows") then
    includes("Windows")
elseif is_plat("macosx") then
    includes("MacOS")
elseif is_plat("linux") then
    includes("Linux")
end
