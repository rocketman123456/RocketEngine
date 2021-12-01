if is_plat("windows", "macosx", "linux") then
    includes("Windows")
elseif is_plat("linux") then
    includes("Linux")
elseif is_plat("macosx") then
    includes("MacOS")
end