#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <time.h>
#include <cstdlib>
#include <stdio.h>

#include "particle.h"
#include "./SOIL/SOIL.h"

// Loads the texture and saves into the particle class
bool ParticleEffect::loadBitmapTexture(char* filename)
{
	// Calls the SOIL library to load the texture
	txParticle = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	// TODO: Error if can't load the particle
	if (txParticle == 0)
		return false;

	return true;
}

// Initializes the particles' direction and texture
void ParticleEffect::initParticles() 
{
	// Seed random
	srand((int)time(NULL));

	part_size = 0.05f;
	win[0] = WINDOW_W;
	win[1] = WINDOW_H;

	// Initialize target direction
	for (int p = 0; p < MAX_PARTICLES; p++)
	{
		Particles[p].active = false;
		Particles[p].start = true;
		Particles[p].tX = 0.0f;
		Particles[p].tY = -0.1f;
		Particles[p].tZ = 0.0f;
	}

	loadBitmapTexture("./Snow.bmp");
}

// Set the particles to start (if dead, re-starts them!)
// Called at every iteration for dead particles
void ParticleEffect::setParticles()
{
	for (int p = 0; p < MAX_PARTICLES; p++)
	{
		if (!Particles[p].active)
		{
			// Start the particle at the origin
			Particles[p].X = ((float)(rand() % (win[0]*100 + 1)) / 100.0) - (win[0]/2.0); // Start along any x within the window
			// If screen has no particles yet, start anywhere. Else (particles already falling), start at the top
			Particles[p].Y = Particles[p].start ? ((float)(rand() % (win[1]*100 + 1)) / 100.0) - (win[1]/2.0) : win[1]/2.0;
			Particles[p].Z = 0.0f;

			// Set a random speed value 
			// (rand() % 100 + 1) - from 1 to 100
			Particles[p].sX = ((float)((rand() % 100) + 1) / 1000.0f) - 0.05f;
			Particles[p].sY = ((float)((rand() % 100) + 1) / 1000.0f) - 0.05f;
			Particles[p].sZ = ((float)((rand() % 100) + 1) / 1000.0f) - 0.05f;

			// Set particle information
			Particles[p].active = true;
			Particles[p].age = 0;
			Particles[p].maxAge = MAX_PARTICLE_AGE;
			Particles[p].start = false;
		}
	}
}

// Updates the particles' location and death timer
void ParticleEffect::updateParticles()
{
	for (int p = 0; p < MAX_PARTICLES; p++)
	{
		// Move speed towards the target speed by 2% (1/50)
		Particles[p].sX += (Particles[p].tX - Particles[p].sX) / 50.0f; 
		Particles[p].sY += (Particles[p].tY - Particles[p].sY) / 50.0f;
		Particles[p].sZ += (Particles[p].tZ - Particles[p].sZ) / 50.0f;

		// Adjust position of the particle using the speed
		Particles[p].X += Particles[p].sX;
		Particles[p].Y += Particles[p].sY;
		Particles[p].Z += Particles[p].sZ;

		// Kill the particle if it gets too far out of the window
		if (Particles[p].X < -(win[0]/2 + 0.5) || Particles[p].X > (win[0]/2 + 0.5) || 
			Particles[p].Y < -(win[1]/2) || Particles[p].Y > (win[1]/2 + 0.5))
			Particles[p].active = false;

		// Check the age
		Particles[p].age++;
		if (Particles[p].age > Particles[p].maxAge)
			Particles[p].active = false;
	}
}

// Render the particles on the screen
void ParticleEffect::renderParticles()
{
	// Enable textures and bind our particle
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txParticle);

	// Disable depth testing
	glDisable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);

	for (int p = 0; p < MAX_PARTICLES; p++)
	{
		if (Particles[p].active)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // Stay white for now
			glPushMatrix();
				// Move the particle to its respective position
				glTranslatef(	Particles[p].X,
								Particles[p].Y,
								Particles[p].Z);
				// Draw the particle (as a quad due to the texture)
				glBegin(GL_QUADS);
					glNormal3f(0.0f, 0.0f, 1.0f);

					glTexCoord2f(0.0f, 0.0f);
					glVertex3f(-part_size, -part_size, 0.0f);

					glTexCoord2f(1.0f, 0.0f);
					glVertex3f(part_size, -part_size, 0.0f);

					glTexCoord2f(1.0f, 1.0f);
					glVertex3f(part_size, part_size, 0.0f);

					glTexCoord2f(0.0f, 1.0f);
					glVertex3f(-part_size, part_size, 0.0f);
				glEnd();
			glPopMatrix();
		}
	}

	// Renable depth testing for the rest of the scene
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

// Re-initialize the particles (clear them from the screen)
void ParticleEffect::clearParticles()
{
	for (int p = 0; p < MAX_PARTICLES; p++)
	{
		Particles[p].active = false;
		Particles[p].start = true;
		Particles[p].age = 0;
	}
	renderParticles();
}
