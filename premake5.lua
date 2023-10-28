project "assimp"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"
    staticruntime "On"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"*.cpp",
		"*.h"
	}


	filter "system:windows"
		systemversion "latest"
		
        defines{
                
        }

	filter "configurations:Debug"
		runtime "Debug"
		buildoptions "/MT"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		buildoptions "/MT"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		buildoptions "/MT"
		optimize "on"
        symbols "off"