-- premake5.lua
workspace "ShitRacer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ShitRacer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "ShitRacer"