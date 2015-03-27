#ifndef PARTICLE_2D
#define PARTICLE_2D

#include "glinclude.h"
#include <vector>
#include <list>
using namespace std;

class Particle
{
public:
	Particle(){}
	~Particle(){}
	glm::vec2 position;
	glm::vec2 velocity;

	float mass;
	float density;
	float pressure;
	glm::vec2 aInside;
	float surfaceTension;
	int inOrOut;//IN = 1; OUT = 0;

	//vector<int> neighbours; 
	vector<list<Particle>::iterator> neighbours;
private:

};

#endif