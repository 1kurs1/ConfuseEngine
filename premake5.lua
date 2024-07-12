workspace "Confuse"
    architecture "x64"
    targetdir "build"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Glad"] = "Confuse/vendor/Glad/include"
IncludeDir["ImGui"] = "Confuse/vendor/imgui"

include "Confuse/vendor/Glad"
include "Confuse/vendor/imgui"

project "Confuse"
    location "Confuse"
    kind "SharedLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "cepch.h"
    pchsource "Confuse/src/cepch.cpp"

    files{
        "%{prj.name}/**.def",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/src",
        "%{prj.name}/vendor",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/GLFW/include",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }

    libdirs{
		"%{prj.name}/vendor/GLFW/lib-vc2022"
	}

    links{
        "glfw3_mt.lib",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "kernel32.lib",
        "winspool.lib",
        "comdlg32.lib",
		"user32.lib",
		"gdi32.lib",
        "advapi32.lib",
		"shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "odbc32.lib",
        "odbccp32.lib",
        "msvcrtd.lib",
        "libcmt.lib",
        "libcmtd.lib"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

        defines{
            "CE_PLATFORM_WINDOWS",
            "CE_BUILD_DLL"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "CE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CE_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "CE_DIST"
        symbols "On"

project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"
        systemversion "latest"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        links{
            "Confuse"
        }

        files{
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.c",
            "%{prj.name}/src/**.hpp",
            "%{prj.name}/src/**.cpp"
        }

        includedirs{
            "%{prj.name}/src",
            "Confuse/src",
            "Confuse/vendor",
            "Confuse/vendor/spdlog/include"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"

            links{
                "Confuse"
            }

            defines{
                "CE_PLATFORM_WINDOWS"
            }

        filter "configurations:Debug"
            defines "CE_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "CE_RELEASE"
            optimize "On"

        filter "configurations:Dist"
            defines "CE_DIST"
            optimize "On"