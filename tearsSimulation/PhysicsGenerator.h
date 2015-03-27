#ifndef PHYSICS_GEN
#define PHYSICS_GEN

#include "glinclude.h"
#include "readBMP.h"
class PhysicsGenerator
{
public:
	PhysicsGenerator();
	~PhysicsGenerator();

	void loadModel(const char* path);
	void loadTexture(const char* path);
	void loadToVbos();
	void calculateTbnMatrix();
	void calculateTbnInverseMatrix();
	void loadForceTextures(GLuint w, GLuint h);

	unsigned int getTriangleNum();
	GLuint getModelVerticeBuffer();
	GLuint getModelTexcoordBuffer();
	GLuint getModelNormalBuffer();
	GLuint getModelTextureObject();
	GLuint getVerticeTexture();
	GLuint getGravityTexture();
	GLuint getPressTexture();
	vector<glm::mat3>& getTbnMatrix();
	vector<glm::mat3>& getTbnInverseMatrix();
	vector<glm::vec3>& getTangent();

private:
	unsigned int _triangle_num;

	GLuint _sub_vao;
	GLuint _model_vertice_buffer;
	GLuint _model_normal_buffer;
	GLuint _model_texcoord_buffer;
	//GLuint _tbn_matrix_buffer;
	//GLuint _tbn_inverse_matrix_buffer;
	GLuint _model_texture;
	//GLuint _TBN_texture;
	GLuint _gravity_texture;
	GLuint _press_texture;
	
	GLuint _vertice_texture;
	//sub textures
	GLuint _normal_texture;
	
	GLuint _model_info_program;
	GLuint _press_texture_program;
	GLuint _gravity_texture_program;

	vector<glm::vec3> _vertice;
	vector<glm::vec3> _normal;
	vector<glm::vec2> _texcoord;
	vector<glm::mat3> _tbn_matrix;
	vector<glm::mat3> _tbn_inverse_matrix;

	vector<glm::vec3> _vtangent;

	void _generateSubTextures(GLuint w, GLuint h);
	void _deleteSubTexTures();
	void _generateVerticeTexture(GLuint w, GLuint h);
	void _generateGravityTexture(GLuint w, GLuint h);
	void _generatePressTexture(GLuint w, GLuint h);
};

#endif