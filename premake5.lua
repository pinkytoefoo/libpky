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

    targetdir "%{wks.location}/bin/"
    objdir "%{wks.location}/bin-obj/"

    files
	{
		"src/**.h",
		"src/**.cpp",
    }

    includedirs
	{
		"src",
	}

    filter "configurations:Debug"
		targetdir "bin/debug"
        targetdir "bin-obj/debug"

	filter "configurations:Release"
		targetdir "bin/release"
        targetdir "bin-obj/release"

    filter "system:windows"
		systemversion "latest"

        links
        {
        }
