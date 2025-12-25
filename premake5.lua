-- TODO: see if this file works
workspace "pky-lib"
    architecture "x86_64"
    startproject "pky_playground"

    configurations
    {
        "Debug",
        "Release"
    }

project "pky"
    kind "StaticLib"
    language "C++"
    targetdir "build/lib"

    files {
        "src/**.cpp",
        "include/**.h"
    }

    includedirs {
        "include"
    }

project "pky_playground"
    kind "ConsoleApp"
    language "C++"
    targetdir "build/bin"

    files {
        "tests/**.cpp"
    }

    links {
        "pky"
    }

    includedirs {
        "include"
    }
