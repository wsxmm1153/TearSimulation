#include "TearRender.h"
#include "shaderSrc.h"
#include <iostream>

TearRender::TearRender():
	_model_vao(0),
	_bump_vao(0),
	_model_program(0),
	_bump_program(0),
	_model_texture(0),
	_height_texture(0),
	_physics_generator(NULL),
	_tear_simulator(NULL)
{
	_physics_generator = new PhysicsGenerator();
	_tear_simulator = new TearSimulator();
}

TearRender::~TearRender()
{
	if (_model_vao)
	{
		glDeleteVertexArrays(1, &_model_vao);
	}

	if (_physics_generator)
	{
		delete _physics_generator;
		_physics_generator = NULL;
	}

	if (_tear_simulator)
	{
		delete _tear_simulator;
		_tear_simulator = NULL;
	}
}

void TearRender::load3DModel(const char* path)
{
	_physics_generator->loadModel(path);
	_physics_generator->loadToVbos();
}

void TearRender::load3DModelTexture(const char* path)
{
	_physics_generator->loadTexture(path);
	_model_texture = _physics_generator->getModelTextureObject();
}

void TearRender::loadModelTbnInverse()
{
	//_physics_generator->calculateTbnMatrix();
	//_physics_generator->calculateTbnInverseMatrix();
	//_model_tbn_inverse = _physics_generator->getTbnInverseMatrix();
	//_vtangent = _physics_generator->getTangent();
	_physics_generator->loadForceTextures(1024, 540);

	//for (int i = 0; i < _model_tbn_inverse.size(); i++)
	//{
	//	std::cout << _model_tbn_inverse[i][0].x << ' '
	//		<< _model_tbn_inverse[i][1].x << ' '
	//		<< _model_tbn_inverse[i][2].x << '\n'
	//		<< _model_tbn_inverse[i][0].y << ' '
	//		<< _model_tbn_inverse[i][1].y << ' '
	//		<< _model_tbn_inverse[i][2].y << '\n'
	//		<< _model_tbn_inverse[i][0].z << ' '
	//		<< _model_tbn_inverse[i][1].z << ' '
	//		<< _model_tbn_inverse[i][2].z << '\n'
	//		<< std::endl;
	//}
}

void TearRender::setViewMatrix(glm::mat4 view)
{
	_view_matrix = view;
}

void TearRender::setProjectMatrix(glm::mat4 project)
{
	_project_matrix = project;
}

void TearRender::setLightPosition(glm::vec3 lightposition)
{
	_light_position = lightposition;
}

void TearRender::setLightDiffuseColor(glm::vec4 dcolor)
{
	_light_diffuse = dcolor;
}

void TearRender::setLightAmbientColor(glm::vec4 acolor)
{
	_light_ambient = acolor;
}

void TearRender::setlightSpecularColor(glm::vec4 scolor)
{
	_light_specular = scolor;
}

void TearRender::setHeightTexture(GLuint htexture)
{
	_height_texture = htexture;
}

void TearRender::init3DModel()
{
	_model_program = compileProgram(phongVertex, phongFragment);
	glGenVertexArrays(1, &_model_vao);
	glBindVertexArray(_model_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _physics_generator->getModelVerticeBuffer());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_model_program, 0, "vVertex");

	glBindBuffer(GL_ARRAY_BUFFER, _physics_generator->getModelNormalBuffer());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_model_program, 0, "vNormal");

	glBindBuffer(GL_ARRAY_BUFFER, _physics_generator->getModelTexcoordBuffer());
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_model_program, 0, "vTexCoords");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniform3fv(glGetUniformLocation(_model_program, "vLightPosition"), 1, glm::value_ptr(_light_position));
	glUniform4fv(glGetUniformLocation(_model_program, "ambientColor"), 1, glm::value_ptr(_light_ambient));
	glUniform4fv(glGetUniformLocation(_model_program, "diffuseColor"), 1, glm::value_ptr(_light_diffuse));
	glUniform4fv(glGetUniformLocation(_model_program, "specularColor"), 1, glm::value_ptr(_light_specular));
}

void TearRender::render3DModel()
{
	glUseProgram(_model_program);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glUniformMatrix4fv(glGetUniformLocation(_model_program, "mv"), 1, GL_FALSE, glm::value_ptr(_view_matrix));
	glUniformMatrix4fv(glGetUniformLocation(_model_program, "p"), 1, GL_FALSE, glm::value_ptr(_project_matrix));

	if (_model_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _model_texture);
		GLint iTexture = glGetUniformLocation(_model_program, "colorMap");
		glUniform1i(iTexture, 0);
	}

	glBindVertexArray(_model_vao);
	glDrawArrays(GL_TRIANGLES, 0, (_physics_generator->getTriangleNum()) * 3);

	glFlush();
}

void TearRender::initBumpRender()
{
	//GLuint tbn_inverse[3];
	//glGenBuffers(3, tbn_inverse);

	//for (int i = 0; i < 3; i++)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, tbn_inverse[i]);
	//	glBufferData(GL_ARRAY_BUFFER,  sizeof(glm::vec3)*(_physics_generator->getTriangleNum())*3, 0, GL_DYNAMIC_DRAW);

	//	GLfloat* ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	//	for (int m = 0; m < _physics_generator->getTriangleNum(); m++)
	//	{
	//		for(int n = 0; n < 3; n++)
	//		{
	//			ptr[m*9+n*3+0] = _model_tbn_inverse[m][i].x;
	//			ptr[m*9+n*3+1] = _model_tbn_inverse[m][i].y;
	//			ptr[m*9+n*3+2] = _model_tbn_inverse[m][i].z;
	//		}
	//	}

	//	//if (i == 0)

	//	//for (int c = 0; c < _physics_generator->getTriangleNum() * 9 ; c++)
	//	//{
	//	//	std::cout << ptr[c] << ' ';
	//	//	if ((c+1)%3 == 0)
	//	//	{
	//	//		std::cout << endl;
	//	//	}
	//	//	if ((c+1)%9 == 0)
	//	//	{
	//	//		std::cout << endl;
	//	//	}
	//	//}
	//	glUnmapBuffer(GL_ARRAY_BUFFER);
	//	ptr = 0;
	//}

	//glBindBuffer(GL_ARRAY_BUFFER, tbn_inverse[0]);
	//glBufferData(GL_ARRAY_BUFFER,  sizeof(glm::vec3)*(_physics_generator->getTriangleNum())*3, 0, GL_DYNAMIC_DRAW);

	//GLfloat* ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	//for (int m = 0; m < _physics_generator->getTriangleNum(); m++)
	//{
	//	for(int n = 0; n < 3; n++)
	//	{
	//		ptr[m*3+n*3+0] = _vtangent[m].x;
	//		ptr[m*3+n*3+1] = _vtangent[m].y;
	//		ptr[m*3+n*3+2] = _vtangent[m].z;
	//	}
	//}

	//glUnmapBuffer(GL_ARRAY_BUFFER);
	//ptr = 0;

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	_physics_generator->loadForceTextures(1024, 540);
	_tear_simulator->systemInitial();

	_tear_simulator->setGravityTexture(_physics_generator->getGravityTexture());
	_tear_simulator->setPressTexture(_physics_generator->getPressTexture());

	_bump_program = compileProgram(bumpVertex, bumpFragment);
	glGenVertexArrays(1, &_bump_vao);
	glBindVertexArray(_bump_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _physics_generator->getModelVerticeBuffer());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_bump_program, 0, "vVertex");

	glBindBuffer(GL_ARRAY_BUFFER, _physics_generator->getModelNormalBuffer());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_bump_program, 0, "vNormal");

	glBindBuffer(GL_ARRAY_BUFFER, _physics_generator->getModelTexcoordBuffer());
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_bump_program, 0, "vTexCoords");

	//glBindBuffer(GL_ARRAY_BUFFER, tbn_inverse[0]);
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindAttribLocation(_bump_program, 0, "t");

	//glBindBuffer(GL_ARRAY_BUFFER, tbn_inverse[1]);
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindAttribLocation(_bump_program, 0, "b");

	//glBindBuffer(GL_ARRAY_BUFFER, tbn_inverse[2]);
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glBindAttribLocation(_bump_program, 0, "n");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniform3fv(glGetUniformLocation(_bump_program, "vLightPosition"), 1, glm::value_ptr(_light_position));
	glUniform4fv(glGetUniformLocation(_bump_program, "ambientColor"), 1, glm::value_ptr(_light_ambient));
	glUniform4fv(glGetUniformLocation(_bump_program, "diffuseColor"), 1, glm::value_ptr(_light_diffuse));
	glUniform4fv(glGetUniformLocation(_bump_program, "specularColor"), 1, glm::value_ptr(_light_specular));
}

void TearRender::bumpRender()
{
	_tear_simulator->testStep();

	setHeightTexture(_tear_simulator->getHeightTexture());
	glUseProgram(_bump_program);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, 800, 800);
	glUniformMatrix4fv(glGetUniformLocation(_bump_program, "mv"), 1, GL_FALSE, glm::value_ptr(_view_matrix));
	glUniformMatrix4fv(glGetUniformLocation(_bump_program, "p"), 1, GL_FALSE, glm::value_ptr(_project_matrix));

	if (_model_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _model_texture);
		GLint iTexture = glGetUniformLocation(_bump_program, "colorMap");
		glUniform1i(iTexture, 0);
	}

	if (_height_texture)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _height_texture);
		GLint iTexture = glGetUniformLocation(_bump_program, "heightMap");
		glUniform1i(iTexture, 1);
	}

	if (_physics_generator->getVerticeTexture())
	{
		GLuint v_texture = _physics_generator->getVerticeTexture();
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, v_texture);
		GLint iTexture = glGetUniformLocation(_bump_program, "verticeMap");
		glUniform1i(iTexture, 2);
	}

	glBindVertexArray(_bump_vao);
	glDrawArrays(GL_TRIANGLES, 0, (_physics_generator->getTriangleNum()) * 3);

	glFlush();
}