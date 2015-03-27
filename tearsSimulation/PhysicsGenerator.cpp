#include "PhysicsGenerator.h"
#include "util.h"

#include <cmath>

PhysicsGenerator::PhysicsGenerator():
	_triangle_num(0),
	_sub_vao(0),
	_model_vertice_buffer(0),
	_model_normal_buffer(0),
	_model_texcoord_buffer(0),
	_model_texture(0),
	_gravity_texture(0),
	_press_texture(0),
	_normal_texture(0),
	_vertice_texture(0),
	_model_info_program(0),
	_press_texture_program(0),
	_gravity_texture_program(0),
	_vertice(0)
{
	
}

void PhysicsGenerator::loadModel(const char* path)
{
	Util ut;
	bool res = ut.loadOBJ(path, _vertice, _texcoord, _normal);
	_triangle_num = (_vertice.size()-JESSI) / 3;
}

void PhysicsGenerator::loadTexture(const char* path)
{
	int textureWidth, textureHeight;
	GLubyte *bmps = NULL;
	bmps = gltReadBMPBits(path, &textureWidth, &textureHeight);
	if (bmps)
	{
		glGenTextures(1, &_model_texture);
		glBindTexture(GL_TEXTURE_2D, _model_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)bmps);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	free(bmps);
}

void PhysicsGenerator::loadToVbos()
{
	_model_info_program = compileProgram(uvModelInfoVertex, uvModelInfoFragment);
	glGenVertexArrays(1, &_sub_vao);
	glBindVertexArray(_sub_vao);

	glGenBuffers(1, &_model_vertice_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _model_vertice_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (_vertice.size()-JESSI), NULL, GL_DYNAMIC_DRAW);
	GLfloat *verticeptr = NULL;
	verticeptr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < _vertice.size()-JESSI ; i++)
	{
		verticeptr[3*i] = _vertice[i+JESSI].x;
		verticeptr[3*i + 1] = _vertice[i+JESSI].y;
		verticeptr[3*i + 2] = _vertice[i+JESSI].z;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_model_info_program, 0, "vVertex");

	verticeptr = NULL;

	glGenBuffers(1, &_model_texcoord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _model_texcoord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) *(_vertice.size()-JESSI), NULL, GL_DYNAMIC_DRAW);
	GLfloat *texcoordptr = NULL;
	texcoordptr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < _vertice.size()-JESSI; i++)
	{
		texcoordptr[2*i] = _texcoord[i+JESSI].x;
		texcoordptr[2*i + 1] = _texcoord[i+JESSI].y;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_model_info_program, 0, "vTexCoords");
	texcoordptr = NULL;

	glGenBuffers(1, &_model_normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _model_normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (_vertice.size()-JESSI), NULL, GL_DYNAMIC_DRAW);
	GLfloat *normalptr = NULL;
	normalptr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < _vertice.size()-JESSI; i++)
	{
		normalptr[3*i] = _normal[i+JESSI].x;
		normalptr[3*i + 1] = _normal[i+JESSI].y;
		normalptr[3*i + 2] = _normal[i+JESSI].z;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_model_info_program, 0, "vNormal");

	normalptr = NULL;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PhysicsGenerator::calculateTbnMatrix()
{
	for (int i = 0; i < _triangle_num; i++)
	{
		glm::mat3 tbn = glm::mat3(1.0f);
		glm::vec3 v1 = _vertice[i*3];
		glm::vec3 v2 = _vertice[i*3+1];
		glm::vec3 v3 = _vertice[i*3+2];
		glm::vec2 c1 = _texcoord[i*3];
		glm::vec2 c2 = _texcoord[i*3+1];
		glm::vec2 c3 = _texcoord[i*3+2];

		glm::vec3 v2v1 = v2 - v1;
		glm::vec3 v3v1 = v3 - v1;
		float c2c1t = c2.x - c1.x;
		float c3c1t = c3.x - c1.x;
		float c2c1b = c2.y - c1.y;
		float c3c1b = c3.y - c1.y;

		float tbdeterminant = c2c1t * c3c1b - c3c1t * c2c1b;

		if (std::fabs(tbdeterminant) > 1.0e-10f)
		{
			glm::vec3 t = (1.0f/tbdeterminant) * (c3c1b*v2v1 - c2c1b*v3v1);
			glm::vec3 b = (1.0f/tbdeterminant) * (-c3c1t*v2v1 + c2c1t*v3v1);
			glm::vec3 n = glm::cross(t, b);

			tbn[0] = t;
			tbn[1] = b;
			if (glm::dot(n,_normal[i*3]) > 0.0f)
			{
				tbn[2] = n;
			}

			else
			{
				tbn[2] = glm::cross(b, t);
			}
			//tbn[2] = n;
		}

		//std::cout << tbn[0].x << ' ' << tbn[0].y << ' ' << tbn[0].z << '\n'
		//	<< tbn[1].x << ' ' << tbn[1].y << ' ' << tbn[1].z << '\n'
		//	<< tbn[2].x << ' ' << tbn[2].y << ' ' << tbn[2].z << '\n'
		//	<< std::endl;
		_vtangent.push_back(glm::normalize(tbn[0]));
		_tbn_matrix.push_back(tbn);
	}
}

#define INVERSE_NO_USE_GLM
void PhysicsGenerator::calculateTbnInverseMatrix()
{
	for (int i = 0; i < _tbn_matrix.size(); i++)
	{
		//glm::mat3 tbn = _tbn_matrix[i];
		glm::mat3 tbn = glm::transpose(_tbn_matrix[i]);
		glm::mat3 tbninverse = glm::affineInverse(tbn);

		//tbninverse[0] = glm::normalize(tbninverse[0]);
		//tbninverse[1] = glm::normalize(tbninverse[1]);
		//tbninverse[2] = glm::normalize(tbninverse[2]);

#ifdef INVERSE_NO_USE_GLM
		//not use glm
		tbninverse = glm::mat3(1.0f);
		float scale = 
			(tbn[0].x*tbn[1].y*tbn[2].z - tbn[0].z*tbn[1].y*tbn[2].x +
			tbn[1].x*tbn[2].y*tbn[0].z - tbn[1].z*tbn[2].y*tbn[0].x +
			tbn[2].x*tbn[0].y*tbn[1].z - tbn[2].z*tbn[0].y*tbn[1].x );

		if (std::fabs(scale) > 1.0e-10f)
		{
			glm::vec3 bcrossn = glm::cross(tbn[1], tbn[2]);
			glm::vec3 ncrosst = glm::cross(tbn[2], tbn[0]);
			glm::vec3 tcrossb = glm::cross(tbn[0], tbn[1]);

			//tbninverse[0] = glm::normalize((1.0f/scale) * glm::vec3(bcrossn.x, -ncrosst.x, tcrossb.x));
			//tbninverse[1] = glm::normalize((1.0f/scale) * glm::vec3(-bcrossn.y, ncrosst.y, -tcrossb.y));
			//tbninverse[2] = glm::normalize((1.0f/scale) * glm::vec3(bcrossn.z, -ncrosst.z, tcrossb.z));
			//tbninverse[0] = (scale * glm::vec3(bcrossn.x, -ncrosst.x, tcrossb.x));
			//tbninverse[1] = (scale * glm::vec3(-bcrossn.y, ncrosst.y, -tcrossb.y));
			//tbninverse[2] = (scale * glm::vec3(bcrossn.z, -ncrosst.z, tcrossb.z));

			tbninverse[0] = (scale * glm::vec3(tbn[1].y*tbn[2].z-tbn[2].y*tbn[1].z, -(tbn[0].y*tbn[2].z-tbn[2].y*tbn[0].z), tbn[0].y*tbn[1].z-tbn[1].y*tbn[0].z));
			tbninverse[1] = (scale * glm::vec3(-(tbn[1].x*tbn[2].z-tbn[2].x*tbn[1].z), tbn[0].x*tbn[2].z-tbn[2].x*tbn[0].z, -(tbn[0].x*tbn[1].z-tbn[1].x*tbn[0].z)));
			tbninverse[2] = (scale * glm::vec3(tbn[1].x*tbn[2].y-tbn[2].x*tbn[1].y, -(tbn[0].x*tbn[2].y-tbn[2].x*tbn[0].y), tbn[0].x*tbn[1].y-tbn[1].x*tbn[0].y));
		}
#endif
		_tbn_inverse_matrix.push_back(tbninverse);
	}
}

void PhysicsGenerator::loadForceTextures(GLuint w, GLuint h)
{
	if (_model_vertice_buffer)
	{
		_generateSubTextures(w, h);
		_generateVerticeTexture(w, h);
		_generatePressTexture(w, h);
		_generateGravityTexture(w, h);
	}
	_deleteSubTexTures();
}

unsigned int PhysicsGenerator::getTriangleNum()
{
	return _triangle_num;
}

GLuint PhysicsGenerator::getModelVerticeBuffer()
{
	return _model_vertice_buffer;
}

GLuint PhysicsGenerator::getModelTexcoordBuffer()
{
	return _model_texcoord_buffer;
}

GLuint PhysicsGenerator::getModelNormalBuffer()
{
	return _model_normal_buffer;
}

GLuint PhysicsGenerator::getModelTextureObject()
{
	return _model_texture;
}

vector<glm::mat3>& PhysicsGenerator::getTbnMatrix()
{
	return _tbn_matrix;
}

vector<glm::mat3>& PhysicsGenerator::getTbnInverseMatrix()
{
	return _tbn_inverse_matrix;
}

vector<glm::vec3>& PhysicsGenerator::getTangent()
{
	return _vtangent;
}

GLuint PhysicsGenerator::getVerticeTexture()
{
	return _vertice_texture;
}

GLuint PhysicsGenerator::getGravityTexture()
{
	return _gravity_texture;
}

GLuint PhysicsGenerator::getPressTexture()
{
	return _press_texture;
}

//render interpolated normals and vertices in u-v space to 2D textures
void PhysicsGenerator::_generateSubTextures(GLuint w, GLuint h)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glGenTextures(1, &_normal_texture);
	glGenTextures(1, &_vertice_texture);
	glBindTexture(GL_TEXTURE_2D, _normal_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _normal_texture, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_FLOAT, NULL);
	
	glBindTexture(GL_TEXTURE_2D, _vertice_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_FLOAT, NULL);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _vertice_texture, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum fboBuffers[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1
	};
	
	glUseProgram(_model_info_program);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	glDrawBuffers(2, fboBuffers);	

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_model_info_program, "p"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 modelview = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_model_info_program, "mv"), 1, GL_FALSE, glm::value_ptr(modelview));
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(_sub_vao);

	glDrawArrays(GL_TRIANGLES, 0, _triangle_num*3);

	glBindVertexArray(0);

	glUseProgram(0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);

	glEnable(GL_DEPTH_TEST);

	glFinish();
}

//for calculate TBN matrix(from x-y-z space to u-v-n space) in the fragment shader, render it to texture
void PhysicsGenerator::_generateVerticeTexture(GLuint w, GLuint h)
{

}

void PhysicsGenerator::_generateGravityTexture(GLuint w, GLuint h)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glGenTextures(1, &_gravity_texture);
	glBindTexture(GL_TEXTURE_2D, _gravity_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gravity_texture, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint program;
	program = compileProgram(gravityTextureVertex, gravityTextureFragment);
	GLfloat vertex[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	GLfloat texCoord[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	GLuint vao;
	GLuint vbo[2];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), (GLvoid*)vertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_press_texture_program, 0, "vVertex");

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), (GLvoid*)texCoord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_press_texture_program, 0, "vTexCoords");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(program);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	glm::mat4 identity = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_FALSE, glm::value_ptr(identity));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(program, "p"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::vec3 g = glm::vec3(GRAVITY_X, GRAVITY_Y, GRAVITY_Z);
	glUniform3fv(glGetUniformLocation(program, "gravity"), 1, glm::value_ptr(g));

	glEnable(GL_TEXTURE_2D);
	if (_vertice_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _vertice_texture);
		GLint iTexture = glGetUniformLocation(_vertice_texture, "verticeMap");
		glUniform1i(iTexture, 0);
	}

	if (_press_texture)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _press_texture);
		GLint iTexture = glGetUniformLocation(_press_texture, "pressMap");
		glUniform1i(iTexture, 1);
	}

	if (_normal_texture)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _normal_texture);
		GLint iTexture = glGetUniformLocation(_normal_texture, "normalMap");
		glUniform1i(iTexture, 2);
	}

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLenum fboBuffers[] = {
		GL_COLOR_ATTACHMENT0
	};

	glDrawBuffers(1, fboBuffers);	
	glBindVertexArray(vao);

	glDrawArrays(GL_QUADS, 0, 4);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_TEXTURE_2D);
	glUseProgram(0);

	glDeleteFramebuffers(1, &fbo);

	glFinish();
}

void PhysicsGenerator::_generatePressTexture(GLuint w, GLuint h)
{
	
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glGenTextures(1, &_press_texture);
	glBindTexture(GL_TEXTURE_2D, _press_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _press_texture, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	_press_texture_program = compileProgram(pressTextureVertex, pressTextureFragment);
	GLfloat vertex[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	GLfloat texCoord[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	GLuint vao;
	GLuint vbo[2];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), (GLvoid*)vertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_press_texture_program, 0, "vVertex");

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), (GLvoid*)texCoord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_press_texture_program, 0, "vTexCoords");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(_press_texture_program);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	
	glm::mat4 identity = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_press_texture_program, "mv"), 1, GL_FALSE, glm::value_ptr(identity));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_press_texture_program, "p"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::vec3 g = glm::vec3(GRAVITY_X, GRAVITY_Y, GRAVITY_Z);
	glUniform3fv(glGetUniformLocation(_press_texture_program, "gravity"), 1, glm::value_ptr(g));

	glEnable(GL_TEXTURE_2D);
	if (_normal_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _normal_texture);
		GLint iTexture = glGetUniformLocation(_normal_texture, "normalMap");
		glUniform1i(iTexture, 0);
	}

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	GLenum fboBuffers[] = {
		GL_COLOR_ATTACHMENT0
	};

	glDrawBuffers(1, fboBuffers);	
	glBindVertexArray(vao);
	
	glDrawArrays(GL_QUADS, 0, 4);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_TEXTURE_2D);
	glUseProgram(0);

	glDeleteFramebuffers(1, &fbo);

	glFinish();
}

void PhysicsGenerator::_deleteSubTexTures()
{
	if (_normal_texture)
	{
		glDeleteTextures(1, &_normal_texture);
	}
	//if (_vertice_texture)
	//{
	//	glDeleteTextures(1, &_vertice_texture);
	//}
}

PhysicsGenerator::~PhysicsGenerator()
{
	if (_sub_vao)
	{
		glDeleteVertexArrays(1, &_sub_vao);
	}
	if (_model_vertice_buffer)
	{
		glDeleteBuffers(1, &_model_vertice_buffer);
	}
	if (_model_texcoord_buffer)
	{
		glDeleteBuffers(1, &_model_texcoord_buffer);
	}
	if (_model_normal_buffer)
	{
		glDeleteBuffers(1, &_model_normal_buffer);
	}
	if (_model_texture)
	{
		glDeleteTextures(1, &_model_texture);
	}
	if (_vertice_texture)
	{
		glDeleteTextures(1, &_vertice_texture);
	}
	if (_gravity_texture)
	{
		glDeleteTextures(1, &_gravity_texture);
	}
	if (_press_texture)
	{
		glDeleteTextures(1, &_press_texture);
	}
	if (_model_info_program)
	{
		glDeleteProgram(_model_info_program);
	}
	if (_press_texture_program)
	{
		glDeleteProgram(_press_texture_program);
	}
	if (_gravity_texture_program)
	{
		glDeleteProgram(_gravity_texture_program);
	}
	_deleteSubTexTures();
}