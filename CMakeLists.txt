cmake_minimum_required(VERSION 3.25)
project(litho_3d__)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver la bibliothèque GLFW3, GLEW, OpenGL & Assimp
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

include_directories("includes" "imgui" "libs")

add_executable(litho_3d__ src/main.cpp src/Mesh.cpp src/Camera.cpp src/Shader.cpp src/Window.cpp src/DirectionalLight.cpp src/Light.cpp src/Material.cpp src/PointLight.cpp src/SpotLight.cpp src/Texture.cpp src/Model.cpp src/Scene.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_impl_glfw.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imgui.cpp)

# Lier GLFW, GLEW, OpenGL & Assimp
target_link_libraries(litho_3d__ glfw GLEW::GLEW ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARIES})
