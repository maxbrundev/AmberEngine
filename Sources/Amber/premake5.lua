workspace "Amber"
	configurations { "Debug", "Release" }
	platforms { "Win32", "x64" }
	startproject "AmberEditor"

outputdir = "%{wks.location}/../../Bin/"
objoutdir = "%{wks.location}/../../Bin-Int/"
dependdir = "%{wks.location}/../../Dependencies/"

include "AmberEngine"
include "AmberEditor"