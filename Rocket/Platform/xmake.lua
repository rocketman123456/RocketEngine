includes("Common")

if is_plat("windows", "macosx", "linux") then
    includes("Desktop")
end
if is_plat("android", "iphoneos") then
    includes("Mobile")
end