#ifndef CAMERA
#define CAMERA

#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>

int buttenState = GLUT_LEFT_BUTTON;
int ox, oy;
GLfloat camera_trans[] = {0.0f, 0.0f, -4.0f};
GLfloat camera_rot[] = {0.0f, 0.0f, 0.0f};
GLfloat camera_trans_offset[] = {0.0f , 0.0f, -4.0f};
GLfloat camera_rot_offset[] = {0.0f, 0.0f, 0.0f};
const GLfloat offset = 0.1f;

void cameraDisplay()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for (int c = 0; c < 3; ++c)
	{
		camera_trans_offset[c] += (camera_trans[c] - camera_trans_offset[c]) * offset;
		camera_rot_offset[c] += (camera_rot[c] - camera_rot_offset[c]) * offset;
	}
	glTranslatef(camera_trans_offset[0], camera_trans_offset[1], camera_trans_offset[2]);
	glRotatef(camera_rot_offset[0], 1.0f, 0.0f, 0.0f);
	glRotatef(camera_rot_offset[1], 0.0f, 1.0f, 0.0f);
}

void mouse(int butten, int state, int x, int y)
{
	if(state == GLUT_DOWN)
		buttenState |= 1<<butten;
	else if(state == GLUT_UP)
		buttenState = 0;
	ox = x; oy = y;

	glutPostRedisplay();
}

void motion(int x, int y)
{
	GLfloat dx, dy;
	dx = (GLfloat)(x - ox); dy = (GLfloat)(y - oy);
	if(buttenState & 4)//right:z
		camera_trans[2] += dy/100.0f * 0.5f * fabs(camera_trans[2]);
	else if (buttenState & 2)//middle:x/y translate
	{
		camera_trans[0] += dx/100.0f;
		camera_trans[1] -= dy/100.0f;
	}
	else if (buttenState & 1)//left:rotate
	{
		camera_rot[0] += dy/5.0f;
		camera_rot[1] += dx/5.0f;
	}
	ox = x; oy = y;

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	if(h == 0)
		h = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void cameraLoop()
{
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
}

#endif