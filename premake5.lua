workspace "Confuse Engine"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Confuse/vendor/GLFW/include"

include "Confuse/vendor/GLFW"

project "Confuse"
    location "Confuse"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

    pchheader "CEpch.h"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/src/",
        "%{prj.name}/vendor/spdlog/include/",
        "%{IncludeDir.GLFW}"
    }

    links{
        "GLFW",
        "GL"
    }

    filter "system:linux"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            "CE_PLATFORM_LINUX",
            "CE_BUILD_LIB"
        }

    postbuildcommands{
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "Sandbox")
    }

    filter "configurations:Debug"
        defines "CE_DEBUG"
        buildoptions "-pipe"
        symbols "On"
    
    filter "configurations:Release"
        defines "CE_RELEASE"
        buildoptions "-pipe"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CE_DIST"
        buildoptions "-pipe"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Confuse/vendor/spdlog/include/",
        "Confuse/src"
    }
    
    links{
        "Confuse"
    }

    filter "system:linux"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            "CE_PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines "CE_DEBUG"
        buildoptions "-pipe"
        symbols "On"
    
    filter "configurations:Release"
        defines "CE_RELEASE"
        buildoptions "-pipe"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CE_DIST"
        buildoptions "-pipe"
        optimize "On"