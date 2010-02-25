
#pragma once
#include "ofMain.h"
#include "ofxNoise.h"


#define MAX_PARTICLES 20000


class PVert {
public:
	float x, y, z;
	PVert() {
		x=0.0; y=0.0; z=0.0;
	}
};

class PColor {
public:
	float r, g, b, a;
	PColor() {
		r=0.0; g=1.0; b=1.0; a=1.0;
	}
};

class PTexture {
public:
	float u, v;
	PTexture() {
		u = 0.0; v = 0.0;
	}
};





class ParticleManager 
{
	
public:
	
	ofxPerlin * noise;
	
	// Texture
	ofImage		texture;
	int			cellRows;
	int			cellColls;
	float		texW, texH;
	
	// OpenGL
	GLuint		particleVBO[3];
	
	// Values
	float		dim[MAX_PARTICLES];			// particle size (w/h)
	PVert		pos[MAX_PARTICLES*4];		// vertex (quad) of particle
	PVert		oldpos[MAX_PARTICLES];	
	PTexture	texcords[MAX_PARTICLES*4];	// texture coords
	PColor		color[MAX_PARTICLES*4];		// particle color rgba
	PVert		vel[MAX_PARTICLES];
	
	float		radians[MAX_PARTICLES];
	float		radius[MAX_PARTICLES];
	float		rotspeed[MAX_PARTICLES];
	float		time[MAX_PARTICLES];
	float		timeAdd[MAX_PARTICLES];
	
	bool flyaway;
	
	// ------------------------------------------
	ParticleManager();
	
	// ------------------------------------------
	void setParticleColor(int i, float r, float g, float b, float a=0.0);
	void setParticlePos(int i, float px, float py, float pz=0.0);
	void setParticleSize(int i, float particleDim);

	// ------------------------------------------ Add Position
	
	void addPosition(int i, float x, float y, float z);
	
	// ------------------------------------------ set the texture coords
	// The idea here is to create a texture that is broken up into cells
	
	// |----||----|
	// |  1 ||  2 |
	// |----||----|
	// |  3 ||  4 |
	// |----||----|
	
	// in each cell there is a image. You can then just shift the
	// texture coords and display a new image for the particle
	void setParticleTexCoords(int i, float columnID, float rowID);

	// ------------------------------------------
	void loadTexture(string path, int cellsInRow, int cellsInCol);
	
	// ------------------------------------------
	void init();
	void update();

	// ------------------------------------------ Add Particles
	void addParticles(int amt, float _x, float _y, float _z);
	
	// ------------------------------------------
	void render();
	
	void setFlyAway();
	
	
};