local function getLatestMSVCToolset()
	local programFiles = os.getenv("ProgramFiles(x86)") or os.getenv("ProgramFiles")

	if programFiles then
		local vswhere = programFiles .. "\\Microsoft Visual Studio\\Installer\\vswhere.exe"

		if os.isfile(vswhere) then
			local installationVersion = os.outputof('"' .. vswhere .. '" -latest -property installationVersion')

			if installationVersion then
				local major = tonumber(installationVersion:match("^(%d+)"))

				if major == 17 then
					return "msc-v143"
				elseif major == 18 then
					return "msc-v145"
				end
			end
		end
	end

	return "msc"
end

workspace "Amber"
	configurations { "Debug", "Release" }
	platforms { "Win32", "x64" }
	startproject "AmberEditor"

	filter "system:windows"
		systemversion "latest"
		toolset(getLatestMSVCToolset())
	filter {}

outputdir = "%{wks.location}/../../Bin/"
objoutdir = "%{wks.location}/../../Bin-Int/"
dependdir = "%{wks.location}/../../Dependencies/"

include "../../Dependencies/ImGui"
include "../../Dependencies/tinyxml2"
include "AmberTools"
include "AmberDebug"
include "AmberMaths"
include "AmberWindowing"
include "AmberRendering"
include "AmberPhysics"
include "AmberAudio"
include "AmberUI"
include "AmberCore"
include "AmberEditor"
include "AmberGame"