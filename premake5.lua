workspace "Litho3d"
	architecture "x64"
    configurations 
    { 
    	"Debug",
    	"Release"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Litho3D"
	location "Litho3D"
    kind "WindowedApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files 
	{ 
    	"Litho3D/includes/**.h", "Litho3D/src/**.cpp",
		"libs/glm/**.h", "libs/glm/**.cpp",
    	"libs/imgui/**.h", "libs/imgui/**.cpp"  
	}

    includedirs {
    	"Litho3D/includes",
    	"Litho3D/src",
        "libs/glm",
        "libs/imgui"
    }

    links { "GLEW", "glfw", "GL", "assimp" }

    filter "system:windows"
    	cppdialect "C++17"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"
