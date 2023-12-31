#include "Scene.h"

#include <string>

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::CreateObjects() {
	
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

void Scene::Init(float aspectRatio)
{
	//CreateObjects();

	params.sceneRotationY = 0.0f;
	params.sceneRotationZ = 0.0f;

	params.cameraPosition = glm::vec3(-2.0f, 0.0f, 0.0f);
	params.yaw = 0.0f;
	params.pitch = 0.0f;
	camera = Camera(params.cameraPosition, glm::vec3(0.0f, 1.0f, 0.0f), params.yaw, params.pitch, 5.0f, 0.5f);

    std::string modelPath   = "../../../Litho3D/assets/models/";
    std::string texturePath = "../../../Litho3D/assets/textures/";

    std::string brickTexturePath = texturePath + "brick.png";
	brickTexture = Texture(brickTexturePath.c_str());
	brickTexture.LoadTexture();

    std::string dirtTexturePath = texturePath + "dirt.png";
	dirtTexture = Texture((dirtTexturePath).c_str());
	dirtTexture.LoadTexture();

    std::string gridTexturePath = texturePath + "grid_64x64.png";
	plainTexture = Texture((gridTexturePath).c_str());
	plainTexture.LoadTexture();

	testObject = Model();
	testObject.LoadModel(modelPath + "maya/maya.obj");
	params.xXWingPos = 25.0f;

	bed = Model();
	bed.LoadModel(modelPath + "bed/bed.obj");
	bed.addModel(testObject);

	shinyMaterial = Material(1.0f, 32);
	roughMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	params.spotLightCount = 0;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.2f, 0.1f,
		20.0f);
	params.spotLightCount++;

	projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
}

void Scene::Update(bool* keys, GLfloat scrollXChange, GLfloat scrollYChange, float deltaTime, bool freeCamera)
{
	if (freeCamera) {
		camera.keyControl(keys, deltaTime);
		//camera.mouseControl(glm::radians(mainWindow.getXChange()), glm::radians(mainWindow.getYChange()));
		camera.scrollControl(scrollXChange, scrollYChange, deltaTime);
	}
	else {
		camera.setCameraPosition(params.cameraPosition);
		camera.setCameraOrientation(params.yaw, params.pitch);
	}
}

void Scene::Render(GLuint uniformProjection, GLuint uniformModel, GLuint uniformView,
	GLuint uniformSpecularIntensity, GLuint uniformShininess, GLuint uniformEyePosition)
{
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	glm::mat4 model(1.0f);
	/*model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
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
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, params.xXWingPos));
	//model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	testObject.RenderModel();*/

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
	model = glm::rotate(model, params.sceneRotationY, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, params.sceneRotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	bed.RenderModel();
}

void Scene::Clear()
{
}
