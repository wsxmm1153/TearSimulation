#include "TearSimulator.h"

TearSimulator::TearSimulator():
_particle_num(0),
_feedback_in_vao(0),
_generator_in_vao(0),
_velocity_buffer(0),
_position_buffer(0),
_velocity_buffer_feedback(0),
_position_buffer_feedback(0),
_in_out_feedback(0),
_a_inside_buffer(0),
_surface_tension_buffer(0),
_mass_buffer(0),
_press_texture(0),
_gravity_texture(0),
_height_texture(0),
_fbo(0),
_feedback_program(0),
_generate_height_texture_program(0)
{

}

TearSimulator::~TearSimulator()
{
	_particle_list.clear();
	if (_velocity_buffer)
	{
		glDeleteBuffers(1, &_velocity_buffer);
	}
	if (_position_buffer)
	{
		glDeleteBuffers(1, &_position_buffer);
	}
	if (_velocity_buffer_feedback)
	{
		glDeleteBuffers(1, &_velocity_buffer_feedback);
	}
	if (_position_buffer_feedback)
	{
		glDeleteBuffers(1, &_position_buffer_feedback);
	}
	if (_in_out_feedback)
	{
		glDeleteBuffers(1, &_in_out_feedback);
	}
	if (_a_inside_buffer)
	{
		glDeleteBuffers(1, &_a_inside_buffer);
	}
	if (_surface_tension_buffer)
	{
		glDeleteBuffers(1, &_surface_tension_buffer);
	}
	if (_mass_buffer)
	{
		glDeleteBuffers(1, &_mass_buffer);
	}
	if (_feedback_program)
	{
		glDeleteProgram(_feedback_program);
	}
	if (_generate_height_texture_program)
	{
		glDeleteProgram(_generate_height_texture_program);
	}
	if (_height_texture)
	{
		glDeleteTextures(1, &_height_texture);
	}
	if (_feedback_in_vao)
	{
		glDeleteVertexArrays(1, &_feedback_in_vao);
	}
	if (_generator_in_vao)
	{
		glDeleteVertexArrays(1, &_generator_in_vao);
	}
	if (_fbo)
	{
		glDeleteFramebuffers(1, &_fbo);
	}
}

void TearSimulator::setPressTexture(GLuint presstexture)
{
	_press_texture = presstexture;
}

void TearSimulator::setGravityTexture(GLuint gravitytexture)
{
	_gravity_texture = gravitytexture;
}

void TearSimulator::systemInitial()
{
	_particle_list.clear();
	//test initial
	//for (int i = 0; i < 540; i++)
	//{
	//	for (int j = 0; j < 1024; j++)
	//	{
	//		GLfloat r2 = ((GLfloat)i - 245.0f) * ((GLfloat)i - 245.0f) +
	//			((GLfloat)j - 465.0f) * ((GLfloat)j - 465.0f);
	//		GLfloat an = ((GLfloat)i - 245.0f) * ((GLfloat)i - 245.0f) +
	//			((GLfloat)j - 560.0f) * ((GLfloat)j - 560.0f);
	//		if (r2 < 100.0f)
	//		{
	//			GLfloat c = (1.0f / 10.0f) * sqrt(100.0f - r2);
	//			_add_particle((float)j/1024.0, (float)i/540.0, c);	
	//		}
	//		else if (an < 100.0f)
	//		{
	//			GLfloat c = (1.0f / 10.0f) * sqrt(100.0f - an);
	//			_add_particle((float)j/1024.0, (float)i/540.0, c);
	//		}
	//	}
	//}
	//_particle_num = _particle_list.size();

	//generate buffers
	glGenVertexArrays(1, &_feedback_in_vao);
	//glBindVertexArray(_feedback_in_vao);
	glGenBuffers(1, &_position_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glGenBuffers(1, &_velocity_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, _velocity_buffer);
	glGenBuffers(1, &_a_inside_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, _a_inside_buffer);
	glGenBuffers(1, &_surface_tension_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, _surface_tension_buffer);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &_position_buffer_feedback);
	glGenBuffers(1, &_velocity_buffer_feedback);
	glGenBuffers(1, &_in_out_feedback);

	glGenVertexArrays(1, &_generator_in_vao);
	//glBindVertexArray(_generator_in_vao);
	//glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glGenBuffers(1, &_mass_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, _mass_buffer);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenTextures(1, &_height_texture);

	//generate programs
	_setup_shaders();

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glBindTexture(GL_TEXTURE_2D, _height_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _height_texture, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1024, 540, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBindVertexArray(_generator_in_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_generate_height_texture_program, 0, "vVertex");

	glBindBuffer(GL_ARRAY_BUFFER, _mass_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_generate_height_texture_program, 0, "vMass");
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(_feedback_in_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_feedback_program, 0, "vPosition");

	glBindBuffer(GL_ARRAY_BUFFER, _velocity_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_feedback_program, 0, "vVelocity");

	glBindBuffer(GL_ARRAY_BUFFER, _surface_tension_buffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_feedback_program, 0, "fSurface");

	glBindBuffer(GL_ARRAY_BUFFER, _a_inside_buffer);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(_feedback_program, 0, "vAinside");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//static const char* feedbacks[] = 
	//{"vPositionFeedback", "vVelocityFeedback", "inOutFeedback"};
	//glTransformFeedbackVaryings(_feedback_program, 3, feedbacks, GL_SEPARATE_ATTRIBS);

	//glLinkProgram(_feedback_program);
}

void TearSimulator::testStep()
{
	_step_initial();

	_generate_height_texture();

	_set_neighbours();
	_density_press();
	_a_inside_surface();

	_feedback_pipeline_in();
	_feedback_pipeline();
	_feedback_pipeline_out();
}

void TearSimulator::tearStep()
{
	//particles out with time
	//...
	_step_initial();

	_generate_height_texture();

	_set_neighbours();
	_density_press();
	_a_inside_surface();

	_feedback_pipeline_in();
	_feedback_pipeline();
	_feedback_pipeline_out();
}

GLuint TearSimulator::getHeightTexture()
{
	return _height_texture;
}

void TearSimulator::_add_particle(float x, float y, float mass)
{
	Particle* p = new Particle();
	p->position = glm::vec2(x, y);
	p->velocity = glm::vec2(0.0f, 0.0f);
	p->mass = mass;
	p->density = 0.0f;
	p->pressure = 0.0f;
	p->aInside = glm::vec2(0.0f, 0.0f);
	p->surfaceTension = 0.0f;
	p->inOrOut = 1;
	p->neighbours.clear();

	_particle_list.push_back(*p);
	delete p;
}

void TearSimulator::_setup_shaders()
{
	_generate_height_texture_program = compileProgram(genHeightTextureVertex, genHeightTextureFragment);

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* src = updatePvVertex;
	glShaderSource(vshader, 1, &src, 0);

	glCompileShader(vshader);

	_feedback_program = glCreateProgram();

	glAttachShader(_feedback_program, vshader);

	static const char* feedbacks[] = 
	{"vPositionFeedback", "vVelocityFeedback", "inOutFeedback"};
	glTransformFeedbackVaryings(_feedback_program, 3, feedbacks, GL_SEPARATE_ATTRIBS);

	glLinkProgram(_feedback_program);
	GLint success = 0;
	glGetProgramiv(_feedback_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		char temp[256];
		glGetProgramInfoLog(_feedback_program, 256, 0, temp);
		printf("Fail to link program:\n%s\n", temp);
		glDeleteProgram(_feedback_program);
		_feedback_program = 0;
	}
}

//steps
void TearSimulator::_step_initial()
{
	list<Particle>::iterator it = _particle_list.begin();
	while (it != _particle_list.end())
	{
		if (it->inOrOut)
		{
			it->neighbours.clear();
			it++;
		}
		else
		{
			it = _particle_list.erase(it);
		}
	}
	_particle_num = _particle_list.size();

	if (scount < 31)
	{
		for (int i = 0; i < 4; i++)
		{
			_add_particle((512.0 - tear_out[i].x) / 512.0, (270.0 - tear_out[i].y) / 270.0, 
				(sqrt(15.0*15.0 - ((double)scount-15.0) * ((double)scount -15.0))) / 15.0);
		}
	}

	_particle_num = _particle_list.size();

	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _particle_num, 0, GL_DYNAMIC_DRAW);
	GLfloat* positionptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	glBindBuffer(GL_ARRAY_BUFFER, _velocity_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _particle_num, 0, GL_DYNAMIC_DRAW);
	GLfloat* velocityptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	glBindBuffer(GL_ARRAY_BUFFER, _mass_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _particle_num, 0, GL_DYNAMIC_DRAW);
	GLfloat* massptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	it = _particle_list.begin();
	for (int i = 0; i < _particle_num; i++)
	{
		positionptr[2*i+0] = (it->position).x;
		positionptr[2*i+1] = (it->position).y;
		
		velocityptr[2*i+0] = (it->velocity).x;
		velocityptr[2*i+1] = (it->velocity).y;

		massptr[i] = it->mass;

		it++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, _velocity_buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, _mass_buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _position_buffer_feedback);
	glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(glm::vec2) * _particle_num, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _velocity_buffer_feedback);
	glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(glm::vec2) * _particle_num, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _in_out_feedback);
	glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(int) * _particle_num, 0, GL_DYNAMIC_COPY);
	
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
}

void TearSimulator::_set_neighbours()
{
	list<Particle>::iterator it = _particle_list.begin();
	while(it != _particle_list.end())
	{
		list<Particle>::iterator p = it;
		while (p != _particle_list.end())
		{
			float d = glm::distance(it->position, p->position);
			if (d < SMOOTH_LENGTH)
			{
				it->neighbours.push_back(p);
				p->neighbours.push_back(it);
			}
			p++;
		}
		it++;
	}
}

void TearSimulator::_density_press()
{
	float constantf = MASS_PER_PIXEL * 4.0/(PI * pow(SMOOTH_LENGTH, 8.0));
	list<Particle>::iterator it = _particle_list.begin();
	while (it != _particle_list.end())
	{
		it->density = 0.0f;
		glm::vec2 ri = it->position;
		vector<list<Particle>::iterator>::iterator p = it->neighbours.begin();
		while (p != it->neighbours.end())
		{
			list<Particle>::iterator tmp = *p;
			glm::vec2 rj = tmp->position;
			float r = glm::distance(ri, rj);
			float mj = tmp->mass;
			it->density += (mj * pow((SMOOTH_LENGTH*SMOOTH_LENGTH - r*r),3.0));
			p++;
		}
		it->density *= constantf;
		it->pressure = ((it->density) - REST_DENSITY) * PRESSURE_CONSTANT;
		it++;
	}
}

void TearSimulator::_a_inside_surface()
{
	float constant1f = MASS_PER_PIXEL * MASS_PER_PIXEL * 15.0f / (PI * pow(SMOOTH_LENGTH, 5.0));
	float constant2f = MASS_PER_PIXEL * MOTION_DAMPING * MASS_PER_PIXEL * 20.0f / (PI * pow(SMOOTH_LENGTH, 5.0));
	list<Particle>::iterator it = _particle_list.begin();
	while (it != _particle_list.end())
	{
		glm::vec2 fp = glm::vec2(0.0f, 0.0f);
		glm::vec2 fv = glm::vec2(0.0f, 0.0f);
		float surfacetension = 0.0f;
		float h = SMOOTH_LENGTH;
		float di = it->density;
		float pi = it->pressure;
		glm::vec2 ri = it->position;
		glm::vec2 ui = it->velocity;
		vector<list<Particle>::iterator>::iterator p = it->neighbours.begin();
		while (p != it->neighbours.end())
		{
			list<Particle>::iterator tmp = *p;
			glm::vec2 rj = tmp->position;
			glm::vec2 uj = tmp->velocity;
			float r = glm::distance(ri, rj);
			float mj = tmp->mass;
			float dj = tmp->density;
			float pj = tmp->pressure;
			glm::vec2 rv = ri - rj;
			if (r > 0.0f)
			{
				fp += (mj * (pi + pj) * (h - r) * (h - r) / (dj * r)) * rv;
			}
			fv += (mj * (h - r) / dj) * (uj - ui);

			p++;
		}

		fp *= constant1f;
		fv *= constant2f;
		it->aInside = (fp+fv)/di;
		it->surfaceTension = CONSTANT_SITA * exp(-glm::length(fp) / di) ;

		it++;
	}
}

void TearSimulator::_feedback_pipeline_in()
{
	glBindBuffer(GL_ARRAY_BUFFER, _a_inside_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _particle_num, 0, GL_DYNAMIC_DRAW);
	GLfloat* ainsideptr = (GLfloat* )glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	glBindBuffer(GL_ARRAY_BUFFER, _surface_tension_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _particle_num, 0, GL_DYNAMIC_DRAW);
	GLfloat* surfacetensionptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	list<Particle>::iterator it = _particle_list.begin();
	for (int i = 0; i < _particle_num; i++)
	{
		ainsideptr[2*i+0] = (it->aInside).x;
		ainsideptr[2*i+1] = (it->aInside).y;

		surfacetensionptr[i] = it->surfaceTension;

		it++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _surface_tension_buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, _a_inside_buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TearSimulator::_feedback_pipeline()
{
	glUseProgram(_feedback_program);
	//{"vPositionFeedback", "vVelocityFeedback", "inOutFeedback"};

	//static const char* feedbacks[] = 
	//{"vPositionFeedback", "vVelocityFeedback", "inOutFeedback"};
	//glTransformFeedbackVaryings(_feedback_program, 3, feedbacks, GL_SEPARATE_ATTRIBS);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _position_buffer_feedback);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, _velocity_buffer_feedback);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, _in_out_feedback);

	//glUseProgram(_feedback_program);

	glBindVertexArray(_feedback_in_vao);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _press_texture);
	GLint iTexture = glGetUniformLocation(_feedback_program, "pressTex");
	glUniform1i(iTexture, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _gravity_texture);
	GLint iTexture1 = glGetUniformLocation(_feedback_program, "gravityTex");
	glUniform1i(iTexture1, 1);

	glUniform1f(glGetUniformLocation(_feedback_program, "timeStep"), TIME_STEP);
	glUniform1f(glGetUniformLocation(_feedback_program, "miu"), F_MIU);
	glUniform1f(glGetUniformLocation(_feedback_program, "constfn"), FN_CONSTANT);

	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, _particle_num);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	glUseProgram(0);
}

void TearSimulator::_feedback_pipeline_out()
{
	glFinish();
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _position_buffer_feedback);
	GLfloat* positionptr = (GLfloat*)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_ONLY);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _velocity_buffer_feedback);
	GLfloat* velocityptr = (GLfloat*)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_ONLY);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _in_out_feedback);
	GLint* inoutptr = (GLint*)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_ONLY);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

	list<Particle>::iterator it = _particle_list.begin();
	for (int i = 0; i < _particle_num; i++)
	{
		(it->position).x = positionptr[2*i+0];
		(it->position).y = positionptr[2*i+1];

		(it->velocity).x = velocityptr[2*i+0];
		(it->velocity).y = velocityptr[2*i+1];

		it->inOrOut = inoutptr[i];

		it++;
	}

	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _position_buffer_feedback);
	glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _velocity_buffer_feedback);
	glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _in_out_feedback);
	glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);

	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
	glFinish();
}

void TearSimulator::_generate_height_texture()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glEnable(GL_POINT_SPRITE);
	glUseProgram(_generate_height_texture_program);

	glm::mat4 identity = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_generate_height_texture_program, "mv"), 1, GL_FALSE, glm::value_ptr(identity));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_generate_height_texture_program, "p"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1f(glGetUniformLocation(_generate_height_texture_program, "poly"), 4.0/(PI*pow(1.0, 8.0)));

	glViewport(0.0, 0.0, 1024, 540);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLenum fboBuffers[] = {
		GL_COLOR_ATTACHMENT0
	};

	glDrawBuffers(1, fboBuffers);

	glBindVertexArray(_generator_in_vao);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDrawArrays(GL_POINTS, 0, _particle_num);
	glDisable(GL_BLEND);
	glUseProgram(0);

	//glEnable(GL_DEPTH_TEST);
	//glColor3f(1.0, 0.0, 0.0);
	//glutSolidCube(0.05f);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);

	glFinish();
}

