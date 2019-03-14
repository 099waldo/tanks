-- premake5.lua
workspace "TankGame"
   configurations { "Debug", "Release" }

project "Tanks"
   kind "WindowedApp"
   language "C++"
   targetdir ""

   files { "**.h", "**.cpp", "**.c", "**.hpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

    configuration "Linux"
        location "build"
        libdirs "./lib"
        includedirs "./include"
        defines { "DEBUG" }
        symbols "On"
        links { "sfml-graphics", "sfml-window", "sfml-system"}
    --  cppdialect "C++17"
