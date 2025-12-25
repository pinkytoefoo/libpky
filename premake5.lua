workspace "pky"
    architecture "x86_64"
    startproject "pky"

    configurations
    {
        "Debug",
        "Release"
    }

-- outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "pky"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    staticruntime "off"

    targetdir "%{wks.location}/build/"
    objdir "%{wks.location}/build/"

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
    }

    filter "system:windows"
        systemversion "latest"

        links
        {
        }
