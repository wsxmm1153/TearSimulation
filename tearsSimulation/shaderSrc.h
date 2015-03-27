////how to use it
//#include "shaderSrc.h"
// 
//extern const char *shaderName = STRINGIFY(
//	shader source(string)
//	);
//compileProgram(vertexSrc, fragmentSrc, geometrySrc);

#ifndef SHADER_SRC
#define SHADER_SRC

#define STRINGIFY(A) #A

extern const char* phongVertex;
extern const char* phongFragment;
extern const char* bumpVertex;
extern const char* bumpFragment;
extern const char* uvModelInfoVertex;
extern const char* uvModelInfoFragment;
extern const char* pressTextureVertex;
extern const char* pressTextureFragment;
extern const char* gravityTextureVertex;
extern const char* gravityTextureFragment;
extern const char* genHeightTextureVertex;
extern const char* genHeightTextureFragment;
extern const char* updatePvVertex;

#include <GL/glew.h>
#include <stdio.h>

GLuint compileProgram(const char* vertexSrc, const char* fragmentSrc);

#endif

