#ifndef TEAR_SIMULATOR
#define TEAR_SIMULATOR
#include "glinclude.h"
#include "particel2D.h"
#include "constants.h"
#include <list>
#include <math.h>
using namespace std;
class TearSimulator
{
public:
	TearSimulator();
	~TearSimulator();
	void setPressTexture(GLuint presstexture);
	void setGravityTexture(GLuint gravitytexture);
	void systemInitial();
	void testStep();
	void tearStep();

	GLuint getHeightTexture();
private:
	list<Particle> _particle_list;
	unsigned int _particle_num;//change every frame
	GLuint _press_texture;
	GLuint _gravity_texture;
	void _setup_shaders();
	void _add_particle(float x, float y, float mass);

	//in the step
	void _step_initial();//add and delete particles, clear neighbours
	void _set_neighbours();
	void _density_press();
	void _a_inside_surface();
	void _feedback_pipeline_in();
	void _feedback_pipeline();
	void _feedback_pipeline_out();
	void _generate_height_texture();

	GLuint _fbo;
	GLuint _height_texture;
	//buffers
	GLuint _feedback_in_vao;
	GLuint _generator_in_vao;
	GLuint _velocity_buffer;
	GLuint _position_buffer;
	GLuint _velocity_buffer_feedback;
	GLuint _position_buffer_feedback;
	GLuint _in_out_feedback;
	GLuint _a_inside_buffer;
	GLuint _surface_tension_buffer;
	GLuint _mass_buffer;
	//progarms
	GLuint _feedback_program;
	GLuint _generate_height_texture_program;
};


#endif