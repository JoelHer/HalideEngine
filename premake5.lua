-- premake5.lua
workspace "HalideEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "HalideEngine"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "HalideEngine"