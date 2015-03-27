#ifndef TEAR_RENDER
#define TEAR_RENDER

#include "glinclude.h"
#include "PhysicsGenerator.h"
#include "TearSimulator.h"

class TearRender
{
public:
	TearRender();
	~TearRender();
	void load3DModel(const char* path);
	void load3DModelTexture(const char* path);
	void init3DModel();
	void initBumpRender();
	void loadModelTbnInverse();

	void setViewMatrix(glm::mat4 view);
	void setProjectMatrix(glm::mat4 project);

	void setLightPosition(glm::vec3 lightposition);
	void setLightAmbientColor(glm::vec4 acolor);
	void setLightDiffuseColor(glm::vec4 dcolor);
	void setlightSpecularColor(glm::vec4 scolor);
	void setHeightTexture(GLuint htexture);

	void render3DModel();
	void bumpRender();

private:
	PhysicsGenerator* _physics_generator;
	TearSimulator* _tear_simulator;
	glm::mat4 _view_matrix;
	glm::mat4 _project_matrix;
	glm::vec3 _light_position;
	glm::vec4 _light_diffuse;
	glm::vec4 _light_ambient;
	glm::vec4 _light_specular;

	GLuint _model_vao;
	GLuint _model_program;
	GLuint _model_texture;

	GLuint _bump_vao;
	GLuint _bump_program;

	GLuint _height_texture;

	vector<glm::mat3> _model_tbn_inverse; 
	vector<glm::vec3> _vtangent;
};

#endif