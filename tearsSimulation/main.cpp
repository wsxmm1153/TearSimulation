#include "camera.h"
#include "TearRender.h"
#include "math.h"
#include <windows.h>

static GLfloat hightImage[1024][1024];
GLuint heightTexture;
TearRender* render;
TearRender* eyerender;
int scount = 0;
glm::vec2 tear_out[8] = 
{
	//glm::vec2(214.0f, 137.0f),
	//glm::vec2(224.0f, 145.0f), 
	glm::vec2(233.0f, 140.0f),
	glm::vec2(233.0f, 140.0f),
	glm::vec2(283.0f, 141.0f),
	glm::vec2(283.0f, 141.0f),
	//glm::vec2(293.0f, 145.0f), 
	//glm::vec2(298.0f, 137.0f)
};
//
float stime;
char sfps[40] = "Tear DEMO, fps:  ";
static float total = 0;	
static unsigned int tcount = 0;

void makeHeightImage(void)
{
	for (int i = 0; i < 1024; i++)
	{
		for (int j = 0; j < 1024; j++)
		{
			GLfloat r2 = ((GLfloat)i - 480.0f) * ((GLfloat)i - 480.0f) +
				((GLfloat)j - 465.0f) * ((GLfloat)j - 465.0f);
			GLfloat an = ((GLfloat)i - 480.0f) * ((GLfloat)i - 480.0f) +
				((GLfloat)j - 560.0f) * ((GLfloat)j - 560.0f);
			if (r2 < 100.0f)
			{
				GLfloat c = (1.0f / 10.0f) * sqrt(100.0f - r2);
				hightImage[i][j] = c;	
			}
			else if (an < 100.0f)
			{
				GLfloat c = (1.0f / 10.0f) * sqrt(100.0f - an);
				hightImage[i][j] = c;	
			}
			else
				hightImage[i][j] = 0.0f;
		}
	}
}

void genHeightTexture()
{
	//bmp initial
	//int textureWidth, textureHeight;
	//GLubyte *bmps = NULL;
	//bmps = gltReadBMPBits("initear.bmp", &textureWidth, &textureHeight);
	////bmps = gltReadBMPBits("inter.bmp", &textureWidth, &textureHeight);
	//if (bmps)
	//{
	//	glGenTextures(1, &heightTexture);
	//	glBindTexture(GL_TEXTURE_2D, heightTexture);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)bmps);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}

	//free(bmps);

	//map initial
	makeHeightImage();
	glGenTextures(1, &heightTexture);
	glBindTexture(GL_TEXTURE_2D, heightTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RED, GL_FLOAT, hightImage);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void init(void)
{
	glShadeModel(GL_SMOOTH);

	glewInit();//必须初始化glew
	
	genHeightTexture();

	render = new TearRender();
	eyerender = new TearRender();
	render->load3DModel("Jessi.obj");
	render->load3DModelTexture("Jessi_face.bmp");
	//render->load3DModel("earth.obj");
	//render->load3DModel("Jessi.obj");
	//render->load3DModel("cc.obj");
	//render->load3DModelTexture("face.bmp");
	//render->load3DModelTexture("earth.bmp");
	//render->load3DModelTexture("11.bmp");

	render->setLightPosition(glm::vec3(8.0f, 0.0f, 0.0f));
	render->setLightDiffuseColor(glm::vec4(0.2f, 0.4f, 1.0f, 1.0f));
	render->setlightSpecularColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	render->setLightAmbientColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	//render->init3DModel();
	eyerender->load3DModel("eyes.obj");
	eyerender->load3DModelTexture("eyes.bmp");

	eyerender->setLightPosition(glm::vec3(8.0f, 0.0f, 0.0f));
	eyerender->setLightDiffuseColor(glm::vec4(0.2f, 0.4f, 1.0f, 1.0f));
	eyerender->setlightSpecularColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	eyerender->setLightAmbientColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	eyerender->init3DModel();

	render->loadModelTbnInverse();
	render->initBumpRender();

	//render->setHeightTexture(heightTexture);

	//glFrontFace(GL_CW);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

}

void display(void)
{
	glClearColor(0.65f, 0.65f, 0.55f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	cameraDisplay();
	//glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0, -2.0, 0.0);
	glScalef(0.85, 0.85, 0.85);
	//glScalef(12, 12, 12);

	GLfloat _mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, _mv);
	glm::mat4 mv = glm::mat4(
		_mv[0], _mv[1], _mv[2], _mv[3],
		_mv[4], _mv[5], _mv[6], _mv[7],
		_mv[8], _mv[9], _mv[10], _mv[11],
		_mv[12], _mv[13], _mv[14], _mv[15]
	);
	render->setViewMatrix(mv);
	//glm::mat4 eyeMv = glm::rotate(glm::mat4(1.0f), 15.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	eyerender->setViewMatrix(mv);
	GLfloat _p[16];
	glGetFloatv(GL_PROJECTION_MATRIX, _p);
	glm::mat4 p = glm::mat4(
		_p[0], _p[1], _p[2], _p[3],
		_p[4], _p[5], _p[6], _p[7],
		_p[8], _p[9], _p[10], _p[11],
		_p[12], _p[13], _p[14], _p[15]
	);
	render->setProjectMatrix(p);
	eyerender->setProjectMatrix(p);
	
	eyerender->render3DModel();
	render->bumpRender();
	glFinish();

	float t = (float)timeGetTime();
	float dt = t - stime;
	total += dt*0.001f;
	stime = t;
	if(total > 1.0f)
	{
		sprintf(&sfps[15], "%f", tcount*1.0f);
		total = 0;
		tcount = 0;
	}
	tcount ++;
	scount ++;
	if (scount == 150)
	{
		scount = 0;
	}
	glutSetWindowTitle(sfps);

	glutPostRedisplay();
	glutSwapBuffers();
}  

void shutDown()
{
	delete render;
	delete eyerender;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(sfps);
	init();

	glutDisplayFunc(display);

	cameraLoop();
	glutMainLoop();

	shutDown();
	return 0;
}

//the 2D test
//#include "PhysicsGenerator.h"
//#include "TearSimulator.h"
//
//PhysicsGenerator *phy = NULL;
//TearSimulator *ts = NULL;
//GLuint verticeTex;
//static GLfloat hightImage[1024][1024];
//GLuint heightTexture;
//
//void makeHeightImage(void)
//{
//		for (int i = 0; i < 1024; i++)
//		{
//			for (int j = 0; j < 1024; j++)
//			{
//				GLfloat r2 = ((GLfloat)i - 480.0f) * ((GLfloat)i - 480.0f) +
//					((GLfloat)j - 480.0f) * ((GLfloat)j - 480.0f);
//				GLfloat an = ((GLfloat)i - 480.0f) * ((GLfloat)i - 480.0f) +
//					((GLfloat)j - 555.0f) * ((GLfloat)j - 555.0f);
//				if (r2 < 100.0f)
//				{
//					GLfloat c = (1.0f / 10.0f) * sqrt(100.0f - r2);
//					hightImage[i][j] = c;	
//				}
//				else if (an < 100.0f)
//				{
//					GLfloat c = (1.0f / 10.0f) * sqrt(100.0f - an);
//					hightImage[i][j] = c;	
//				}
//				else
//					hightImage[i][j] = 0.0f;
//			}
//		}
//}
//
//void genHeightTexture()
//{
//	makeHeightImage();
//	glGenTextures(1, &heightTexture);
//
//	//glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, heightTexture);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RED, GL_FLOAT, hightImage);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void init(void)
//{
//	glewInit();//必须初始化glew
//
//	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	phy = new PhysicsGenerator();
//	ts = new TearSimulator();
//	ts->systemInitial();
//	phy->loadModel("Jessi.obj");
//	//phy->loadModel("earth.obj");
//	phy->loadToVbos();
//	phy->loadForceTextures(1024, 540);
//	//verticeTex = phy->getVerticeTexture();
//	//verticeTex = phy->getPressTexture();
//	//verticeTex = phy->getGravityTexture();
//	ts->setGravityTexture(phy->getGravityTexture());
//	ts->setPressTexture(phy->getPressTexture());
//	verticeTex = ts->getHeightTexture();
//
//	genHeightTexture();
//	
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glEnable(GL_TEXTURE_2D);
//	glShadeModel(GL_SMOOTH);
//	//glDisable(GL_DEPTH_TEST);
//	//glDisable(GL_BLEND);
//}
//
//void display(void)
//{
//	ts->testStep();
//	verticeTex = ts->getHeightTexture();
//	//verticeTex = ts->getHeightTexture();
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//	glViewport(0, 0, 1024, 540);
//	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//	
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, verticeTex);
//	//glBindTexture(GL_TEXTURE_2D, heightTexture);
//	glBegin(GL_QUADS);
//	{
//		glTexCoord2f(0.0f, 0.0f);
//		//glVertex2f(0.0f, 0.0f);
//		glVertex2f(0.1f, 0.1f);
//
//		glTexCoord2f(1.0f, 0.0f);
//		//glVertex2f(1.0f, 0.0f);
//		glVertex2f(0.9f, 0.1f);
//
//		glTexCoord2f(1.0f, 1.0f);
//		//glVertex2f(1.0f, 1.0f);
//		glVertex2f(0.9f, 0.9f);
//
//		glTexCoord2f(0.0f, 1.0f);
//		//glVertex2f(0.0f, 1.0f);
//		glVertex2f(0.1f, 0.9f);
//	}
//	glEnd();
//
//		scount ++;
//		if (scount == 150)
//		{
//			scount = 0;
//		}
//
//	glutSwapBuffers();
//	glutPostRedisplay();
//}  
//
//void shutDown()
//{
//	delete phy;
//	//delete ts;
//}
//
//int main(int argc, char **argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//	glutInitWindowSize(1024, 540);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow(argv[0]);
//	init();
//
//	glutDisplayFunc(display);
//
//	glutMainLoop();
//
//	shutDown();
//	return 0;
//}