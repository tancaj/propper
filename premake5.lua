workspace "propper"
    architecture "x64"
    startproject "sandbox"

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
        cppdialect "c++20"
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

    project "tests"
        kind "ConsoleApp"
        language "C++"
        cppdialect "c++20"
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




