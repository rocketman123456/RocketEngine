includes("Common")
if is_plat("windows", "macosx", "linux") then
    includes("Desktop")
elseif is_plat("android", "iphoneos") then
    includes("Mobile")
end