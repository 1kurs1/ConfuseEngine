project "Sandbox"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",

      "../Confuse/src",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",
   }

    links
    {
      "Confuse"
    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:linux"
      cppdialect "C++17"
      pic "on"
      systemversion "latest"

      defines { "CE_PLATFORM_LINUX" }

   filter "configurations:Debug"
      defines { "CE_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "CE_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "CE_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"