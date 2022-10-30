-- premake5.lua
workspace "Ray"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Ray"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "Ray"