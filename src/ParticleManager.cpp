

#include "ParticleManager.h"

/*	Constructor
________________________________________________________________________________ */

ParticleManager::ParticleManager() 
{
	flyaway = false;
}

/*	Load Texture
 ________________________________________________________________________________ */

void ParticleManager::loadTexture(string path, int cellsInRow, int cellsInCol) 
{
	ofDisableArbTex();
	texture.loadImage(path);
	ofEnableArbTex();
	
	texW = texture.getWidth();
	texH = texture.getHeight();
	
	cellRows  = cellsInRow;
	cellColls = cellsInCol; 
}

/*	Init
 ________________________________________________________________________________ */

void ParticleManager::init() 
{
	
	noise = new ofxPerlin();
	
	printf("-------------------------------\n");
	
	printf("Max Particles: %i\n", MAX_PARTICLES);
	printf("Pos Size: %i\n", MAX_PARTICLES*4);
	
	printf("-------------------------------\n");
	
	
	for(int i=0; i<MAX_PARTICLES; i++) 
	{
		setParticleSize(i, ofRandom(3, 7));
		setParticleColor(i, 1, 1, 1, 1);
		
		// init values
		radians[i] = ofRandom(0, 2 * PI);
		radius[i] = ofRandom(50, 300);
		rotspeed[i] = ofRandom(0.005, 0.02);
		time[i] = ofRandom(0, 1000);
		timeAdd[i] = ofRandom(rotspeed[i], rotspeed[i] * 2);
		
		// position
		float px = cos(radians[i]) * radius[i] + ofGetWidth() / 2;
		float py = sin(radians[i]) * radius[i] + ofGetHeight() / 2;
		float pz = 0;
		setParticlePos(i, px, py, pz);
		
		setParticleTexCoords(i, (int)ofRandom(0, 2), (int)ofRandom(0, 2));
	}
	
	
	// Setup the VBO
	
	glGenBuffersARB(3, &particleVBO[0]);	// Create 3 vertex buffer object. Give address to GLUint array where ID's are stored
	
	// color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);	// Init VBO 0 as a vertex array VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*4*sizeof(float), color, GL_STREAM_DRAW_ARB);	// Copy data into VBO 0: 
	// Second parameter is the number of bytes to allocate
	// Third parameter is pointer to the actual data
	// Fourth parameter is for performance and says how the VBO is going to be used: Stream means that it will be changed every frame. Draw means data is sent to GPU
	
	// vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*3*sizeof(float), pos, GL_STREAM_DRAW_ARB);
	
	// texture coords
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*2*sizeof(float), texcords, GL_STREAM_DRAW_ARB);
	
}

/*	Set texture coordinates
 ________________________________________________________________________________ */

void ParticleManager::setParticleTexCoords(int i, float columnID, float rowID) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	if(columnID > cellColls) columnID = cellColls;
	if(rowID    > cellRows)	 rowID	  = cellRows;
	
	if(columnID < 0) columnID = 0;
	if(rowID < 0)	 rowID	  = 0;
	
	
	
	// get the cell image width
	float cellWidth  = texW / cellRows;
	float cellHeight = texH / cellColls;
	
	float row = rowID;
	float col = columnID;
	
	// P1
	texcords[(i*4)+0].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+0].v = (cellHeight * col)	/ texH;
	
	// P2
	texcords[(i*4)+1].u = ((cellWidth * row)	+ cellWidth)	/ texW;
	texcords[(i*4)+1].v = (cellHeight * col)	/ texH;
	
	// P2
	texcords[(i*4)+2].u = ((cellWidth * row) + cellWidth)		/ texW;
	texcords[(i*4)+2].v = ((cellHeight * col) + cellHeight)	/ texH;	
	
	// P2
	texcords[(i*4)+3].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+3].v = ((cellHeight * col)+cellHeight)	/ texH;	
	
	
}

/*	Set Color
 ________________________________________________________________________________ */

void ParticleManager::setParticleColor(int i, float r, float g, float b, float a) 
{
	if(i < 0) i = 0;
	if(i > MAX_PARTICLES) i = MAX_PARTICLES;
	
	// Color 1
	color[(i*4)+0].r = r;
	color[(i*4)+0].g = g;
	color[(i*4)+0].b = b;
	//color[(i*4)+0].a = a;
	
	// Color 2
	color[(i*4)+1].r = r;
	color[(i*4)+1].g = g;
	color[(i*4)+1].b = b;
	//color[(i*4)+1].a = a;
	
	// Color 3
	color[(i*4)+2].r = r;
	color[(i*4)+2].g = g;
	color[(i*4)+2].b = b;
	//color[(i*4)+2].a = a;
	
	// Color 4
	color[(i*4)+3].r = r;
	color[(i*4)+3].g = g;
	color[(i*4)+3].b = b;
	//color[(i*4)+3].a = a;
	
}

/*	Set Particle Position
 ________________________________________________________________________________ */

void ParticleManager::setParticlePos(int i, float px, float py, float pz) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	// Set old positions
	
	// Only save origin point
	oldpos[i].x = pos[(i*4)+0].x;
	oldpos[i].y = pos[(i*4)+0].y;
	oldpos[i].z = pos[(i*4)+0].z;
	
	// Set new positions
	
	// P1
	pos[(i*4)+0].x = px;
	pos[(i*4)+0].y = py;
	pos[(i*4)+0].z = pz;
	
	// P2
	pos[(i*4)+1].x = px + dim[i];
	pos[(i*4)+1].y = py;
	pos[(i*4)+1].z = pz;
	
	// P3
	pos[(i*4)+2].x = px + dim[i];
	pos[(i*4)+2].y = py + dim[i];
	pos[(i*4)+2].z = pz;
	
	// P4
	pos[(i*4)+3].x = px;
	pos[(i*4)+3].y = py + dim[i];
	pos[(i*4)+3].z = pz;
}

/*	Add Position
 ________________________________________________________________________________ */

void ParticleManager::addPosition(int i, float x, float y, float z) {
	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	// P1
	pos[(i*4)+0].x += x;
	pos[(i*4)+0].y += y;
	pos[(i*4)+0].z += z;
	
	// P2
	pos[(i*4)+1].x += x;
	pos[(i*4)+1].y += y;
	pos[(i*4)+1].z += z;
	
	// P2
	pos[(i*4)+2].x += x;
	pos[(i*4)+2].y += y;
	pos[(i*4)+2].z += z;
	
	// P2
	pos[(i*4)+3].x += x;
	pos[(i*4)+3].y += y;
	pos[(i*4)+3].z += z;
}

/*	Set Particle Size
 ________________________________________________________________________________ */

void ParticleManager::setParticleSize(int i, float particleDim) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	dim[i] = particleDim;
}

/*	Update
________________________________________________________________________________ */

void ParticleManager::update() 
{
	float px;
	float py;
	float pz;
	float noi;
	float div = 100;
	float mul = 50;
	
	for(int i=0; i<MAX_PARTICLES; i++) 
	{		
		if(flyaway)
		{
			addPosition(i, vel[i].x, vel[i].y, vel[i].z);
		}
		else 
		{
			radians[i] += rotspeed[i];
			time[i] += timeAdd[i];
			
			noi = noise->noise((float)i/div, (float)time[i]) * mul;
			
			px = cos(radians[i]) * (radius[i] + noi) + ofGetWidth() / 2;
			py = sin(radians[i]) * (radius[i] + noi) + ofGetHeight() / 2;
			pz = 0;
			
			// Set velocity
			vel[i].x = px - oldpos[i].x;
			vel[i].y = py - oldpos[i].y;
			vel[i].z = pz - oldpos[i].z;
			
			setParticlePos(i, px, py, pz);
		}		
	}
}

/*	Add Particles
 ________________________________________________________________________________ */

void ParticleManager::addParticles(int amt, float _x, float _y, float _z) 
{
	
}

/*	Render
 ________________________________________________________________________________ */

void ParticleManager::render() 
{
	
	
	glEnable(GL_TEXTURE_2D);	// Tells OpenGL that we want ot draw a 2d teture
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enables the Texture coordinate array for drawing with glDrawArrays or glDrawElements calls 
	glEnableClientState(GL_VERTEX_ARRAY); // Enables the Vertex array for drawing with glDrawArrays or glDrawElements calls 
	glEnableClientState(GL_COLOR_ARRAY); // Enables the Color array for drawing with glDrawArrays or glDrawElements calls 
	
	// bind tex coors
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]); // init VBO (see above)
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*2*sizeof(float), texcords); // put data in VBO
	glTexCoordPointer(2, GL_FLOAT, 0, 0); // Tell OpenGL that we have 2 coordinates and the coordinates in the array are floats
	
	
	// bind color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*4*sizeof(float), color);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	
	// bind vertices [these are quads]
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*3*sizeof(float), pos);
	glVertexPointer(3, GL_FLOAT, 0, 0); // Tell OpenGL that we have 3 coordinates (x, y, z) and the coordinates are stored as floats in the array
	
	
	// draw the vbo
	glDisable(GL_DEPTH_TEST); // disable depth test (whatever it is)
	ofEnableArbTex();	
	
	ofEnableAlphaBlending();
	texture.getTextureReference().bind();

	glDrawArrays(GL_QUADS, 0, MAX_PARTICLES*4);  // render all array data we putted in VBO's as quads, at index 0, and to the end of the arrays
	
	texture.getTextureReference().unbind();
	ofDisableAlphaBlending();
	
	ofDisableArbTex();
	glEnable(GL_DEPTH_TEST);
	
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	
}

/*	Getter / Setter
 ________________________________________________________________________________ */


void ParticleManager::setFlyAway()
{
	flyaway = true;
}

