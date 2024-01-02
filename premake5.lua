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
IncludeDir["Glad"] = "Confuse/vendor/Glad/include"
IncludeDir["ImGui"] = "Confuse/vendor/imgui"
IncludeDir["glm"] = "Confuse/vendor/glm"

include "Confuse/vendor/GLFW"
include "Confuse/vendor/Glad"
include "Confuse/vendor/imgui"

project "Confuse"
    location "Confuse"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

    pchheader "CEpch.h"

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines{
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs{
        "%{prj.name}/src/",
        "%{prj.name}/vendor/spdlog/include/",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
    }

    links{
        "GLFW",
        "Glad",
        "ImGui",
        "GL"
    }

    filter "system:linux"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines{
            "CE_PLATFORM_LINUX",
            "CE_BUILD_LIB",
            "GLFW_INCLUDE_NONE"
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
        "Confuse/src",
        "Confuse/vendor/",
        "%{IncludeDir.glm}"
    }
    
    links{
        "Confuse"
    }

    filter "system:linux"
        cppdialect "C++17"
        pic "On"
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