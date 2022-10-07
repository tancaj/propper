workspace "propper"
    architecture "x64"
    startproject "sandbox"

    newoption{
        trigger = "std",
        value = "20",
        description = "C++ version",
        allowed ={
            {"20", "c++20"},
            {"2a", "c++2a"}
        },
        default= "20"
    }

    configurations
    {
        "debug",
        "release",
        "dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "sandbox"
        kind "ConsoleApp"
        language "C++"
        targetdir("bin/" .. outputdir .."/%{prj.name}")
        objdir("bin-int/" .. outputdir .."/%{prj.name}")

    files
    {
        "include/*.hpp",
        "%{prj.name}/*.cpp"
    }

    includedirs
    {
        "include",
        "include/vendor/fmt"

    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        symbols "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"

    filter "options:std=20"
        cppdialect "c++20"

    filter "options:std=2a"
        cppdialect "c++2a"

    project "tests"
        kind "ConsoleApp"
        language "C++"
        targetdir("bin/" .. outputdir .."/%{prj.name}")
        objdir("bin-int/" .. outputdir .."/%{prj.name}")

    files
    {
        "%{prj.name}/*.cpp",
        "%{prj.name}/*.h"
    }

    includedirs
    {
        "include",
        "%{prj.name}/vendor/Catch2/single_include"

    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        symbols "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"

    filter "options:std=20"
        cppdialect "c++20"

    filter "options:std=2a"
        cppdialect "c++2a"




