project "Confuse"
   kind "SharedLib"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",

      "../vendor/imgui",
      "../vendor/glfw/include",
      "../vendor/stb_image",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",
   }

   links
   {
      "GLFW",
      "ImGui",
      "%{Library.Vulkan}",
      "GL"
   }

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:linux"
      cppdialect "C++17"
      systemversion "latest"
      staticruntime "on"
      defines { "CE_PLATFORM_LINUX", "CE_DEBUG"}

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
      defines { "CE_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"