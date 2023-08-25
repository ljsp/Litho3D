#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include <filesystem>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Shader.h"

#include "Scene.h"

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Window mainWindow;
std::vector<Shader> shaderList;

static const char* vShader = "../Shaders/shader.vert";
static const char* fShader = "../Shaders/shader.frag";

void CreateShaders() {
    Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main() {

    mainWindow = Window(1920, 1080);
    mainWindow.Initialize();

    CreateShaders();

	Scene scene = Scene();
    float aspectRatio = mainWindow.getBufferWidth() / mainWindow.getBufferHeight();
	scene.Init(aspectRatio);

    bool freeCamera = false;
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.41f, 0.43f, 0.47f, 1.00f);
    
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
		   uniformSpecularIntensity = 0, uniformShininess = 0, uniformEyePosition = 0;


    while (!mainWindow.getShouldClose()) {
        
		GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        scene.Update(mainWindow.getKeys(), mainWindow.getScrollXChange(), mainWindow.getScrollYChange(), deltaTime, freeCamera);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        shaderList[0].SetDirectionalLight(scene.getMainLight());
		shaderList[0].SetSpotLights(scene.getSpotLights(), scene.params.spotLightCount);

        scene.Render(uniformProjection, uniformModel, uniformView,
            uniformSpecularIntensity, uniformShininess, uniformEyePosition);

        glUseProgram(0);

        {
            ImGui::Begin("Control Panel");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::SeparatorText("Camera");

            ImGui::Checkbox("Free Camera", &freeCamera);
            ImGui::SliderFloat("posX", &scene.params.cameraPosition.x, -50.0f, 50.0f);
            ImGui::SliderFloat("posY", &scene.params.cameraPosition.y, -50.0f, 50.0f);
            ImGui::SliderFloat("posZ", &scene.params.cameraPosition.z, -50.0f, 50.0f);

            ImGui::SliderAngle("yaw angle", &scene.params.yaw, -180.0f, 180.0f);
            ImGui::SliderAngle("pitch angle", &scene.params.pitch, -89.0f, 89.0f);

            ImGui::SeparatorText("Scene");
            ImGui::Text("Scene RotationY");
			ImGui::SliderAngle("rotY", &scene.params.sceneRotationY, -10000.0f, 10000.0f);
            ImGui::SliderAngle("rotz", &scene.params.sceneRotationZ, -180.0f, 180.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

		if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mainWindow.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
