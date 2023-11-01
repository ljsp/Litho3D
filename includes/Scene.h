#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "CommonValues.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

class Scene
{
public:
	Scene();
	~Scene();

	void CreateObjects();

	void Init(float aspectRatio);
	void Update(bool* keys, GLfloat scrollXChange, GLfloat scrollYChange, float deltaTime, bool freeCamera);
	void Render(GLuint uniformProjection, GLuint uniformModel, GLuint uniformView, GLuint uniformSpecularIntensity, GLuint uniformShininess, GLuint uniformEyePosition);
	void Clear();

	Camera getCamera() { return camera; }
	DirectionalLight* getMainLight() { return &mainLight; }
	PointLight* getPointLights() { return pointLights; }
	SpotLight* getSpotLights() { return spotLights; }

	struct Parameters
	{
		glm::vec3 cameraPosition;
		float yaw;
		float pitch;

		float sceneRotationY;
		float sceneRotationZ;

		unsigned int spotLightCount;

		float xXWingPos;
	} params;
	
private:
	
	std::vector<Mesh*> meshList;

	glm::mat4 projection;
	Camera camera;

	Texture brickTexture;
	Texture dirtTexture;
	Texture plainTexture;

	Material shinyMaterial;
	Material roughMaterial;

	Model testObject;
	Model bed;

	DirectionalLight mainLight;
	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];
};

