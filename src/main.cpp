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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material roughMaterial;

Model testObject;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

static const char* vShader = "../Shaders/shader.vert";
static const char* fShader = "../Shaders/shader.frag";

void CreateObjects(){
    Sphere sphere(glm::vec3(0.0f, 2.0f, 0.0f), 1.0f);
    Mesh* obj = new Mesh(sphere, 64);
    meshList.push_back(obj);

    Pyramid pyramid(glm::vec3(-1.0f, -1.0f, -0.6f),
                glm::vec3(0.0f, -1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, -0.6f),
                glm::vec3(0.0f, 1.0f, 0.0f));
    Mesh* pyramidMesh = new Mesh(pyramid);
    meshList.push_back(pyramidMesh);

    Plane plane = Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	Mesh* planeMesh = new Mesh(plane);
	meshList.push_back(planeMesh);
    
}

void CreateShaders() {
    Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main() {

    mainWindow = Window(1920, 1080);
    mainWindow.Initialize();

    CreateObjects();
    CreateShaders();

    bool freeCamera = false;
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.41f, 0.43f, 0.47f, 1.00f);
	
    glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;
    camera = Camera(cameraPosition, glm::vec3(0.0f, 1.0f, 0.0f), yaw, pitch, 5.0f, 0.5f);

	brickTexture = Texture((char*)"../assets/textures/brick.png");
    brickTexture.LoadTextureA();
    dirtTexture = Texture((char*)"../assets/textures/dirt.png");
    dirtTexture.LoadTextureA();
    plainTexture = Texture((char*)"../assets/textures/grid_64x64.png");
    plainTexture.LoadTextureA();

	testObject = Model();
    testObject.LoadModel("../assets/models/x-wing/x-wing.obj");
    float xXWingPos = 25.0f;

    shinyMaterial = Material(1.0f, 32);
	roughMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
                                 0.3f, 0.3f,
                                 0.0f, 0.0f, -1.0f);

	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		                        0.0f, 1.0f,
		                        0.0f, 0.0f, 0.0f,
		                        0.0f, -1.0f, 0.0f,
		                        0.3f, 0.2f, 0.1f,
		                        20.0f);
	spotLightCount++;
    
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
		   uniformSpecularIntensity = 0, uniformShininess = 0, uniformEyePosition = 0;
    
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getShouldClose()) {
        
		GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        if (freeCamera) {
		    camera.keyControl(mainWindow.getKeys(), deltaTime);
            //camera.mouseControl(glm::radians(mainWindow.getXChange()), glm::radians(mainWindow.getYChange()));
            camera.scrollControl(mainWindow.getScrollXChange(), mainWindow.getScrollYChange(),
                                    deltaTime);
        }
        else {
            camera.setCameraPosition(cameraPosition);
            camera.setCameraOrientation(yaw, pitch);
        }

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

        shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        brickTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
		roughMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(6.0f, -1.5f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        plainTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, xXWingPos));
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        //model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        testObject.RenderModel();

        glUseProgram(0);

        {
            ImGui::Begin("Control Panel");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::SeparatorText("Camera");

            ImGui::Checkbox("Free Camera", &freeCamera);
            ImGui::SliderFloat("posX", &cameraPosition.x, -50.0f, 50.0f);
            ImGui::SliderFloat("posY", &cameraPosition.y, -50.0f, 50.0f);
            ImGui::SliderFloat("posZ", &cameraPosition.z, -50.0f, 50.0f);

            ImGui::SliderAngle("yaw angle", &yaw, -180.0f, 180.0f);
            ImGui::SliderAngle("pitch angle", &pitch, -89.0f, 89.0f);

            ImGui::SeparatorText("X-wing");
            ImGui::Text("X-Wing position");
			ImGui::SliderFloat("X", &xXWingPos, -50.0f, 50.0f);

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
