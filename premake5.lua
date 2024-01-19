workspace "Confuse"
    architecture "x64"
    configurations {"Release", "Dist" }
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "ConfuseExternal.lua"
include "Sandbox"