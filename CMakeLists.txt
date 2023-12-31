cmake_minimum_required(VERSION 3.25)
project(litho_3d__)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver la bibliothèque GLFW3, GLEW, OpenGL & Assimp
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

include_directories("includes" "libs")

add_executable(litho_3d__ src/main.cpp src/Mesh.cpp src/Camera.cpp src/Shader.cpp src/Window.cpp src/DirectionalLight.cpp src/Light.cpp src/Material.cpp src/PointLight.cpp src/SpotLight.cpp src/Texture.cpp src/Model.cpp src/Scene.cpp libs/imgui/imgui_widgets.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_impl_opengl3.cpp libs/imgui/imgui_impl_glfw.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui.cpp)

# Lier GLFW, GLEW, OpenGL & Assimp
target_link_libraries(litho_3d__ glfw GLEW::GLEW ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARIES})
