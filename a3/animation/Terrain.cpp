#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
 
#include <GL/gl.h>
#include <GL/glut.h>

#include "Terrain.h"

// Initializes the terrain's size and textures
void Terrain::initTerrain() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// clear to black
	glShadeModel(GL_SMOOTH);					   // use smooth shading
	glEnable(GL_DEPTH_TEST);					   // hidden surface removal
	glEnable(GL_CULL_FACE);						   // do not calculate inside of poly's
	glFrontFace(GL_CCW);						      // counter clock-wise polygons are out
	glEnable(GL_TEXTURE_2D);					   // enable 2D texturing

	// Retrieve files for terrain
	texture = loadTexBMP("snowTexture.bmp");

	// Terrain init
	// loop through all of the heightfield points, calculating
	// the coordinates for each point
	for (int z = 0; z < MAP_Z; z++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			terrain[x][z][0] = float(x)*MAP_SCALE;				
			terrain[x][z][1] = 0.0f;
			terrain[x][z][2] = -float(z)*MAP_SCALE;
		}
	}
}

void Terrain::renderTerrain() {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);		

		// set the current texture to the land texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// Adjust terrain so it is in the center of map
		glTranslatef(-MAP_X/2.0f * MAP_SCALE, -10.0f, MAP_Z/2.0f * MAP_SCALE);
		glScalef(50.0f, 50.0f, 50.0f);

		// we are going to loop through all of our terrain's data points,
		// but we only want to draw one triangle strip for each set along the x-axis.
		for (int z = 0; z < MAP_Z-1; z++)
		{			
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < MAP_X-1; x++)
			{
				// for each vertex, we calculate the grayscale shade color, 
				// we set the texture coordinate, and we draw the vertex.
				/*
				   the vertices are drawn in this order:

				   0  ---> 1
					      /
						/
					 |/
				   2  ---> 3
				*/
		
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(terrain[x][z][0], terrain[x][z][1], terrain[x][z][2]);

				glTexCoord2f(1.0f, 0.0f);				
				glVertex3f(terrain[x+1][z][0], terrain[x+1][z][1], terrain[x+1][z][2]);

				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(terrain[x][z+1][0], terrain[x][z+1][1], terrain[x][z+1][2]);

				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(terrain[x+1][z+1][0], terrain[x+1][z+1][1], terrain[x+1][z+1][2]);

			}
			glEnd();
		}
		// enable blending
		glEnable(GL_BLEND);

		// enable read-only depth buffer
		glDepthMask(GL_FALSE);

		// set the blend function to what we use for transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// set back to normal depth buffer mode (writable)
		glDepthMask(GL_TRUE);

		// disable blending
		glDisable(GL_BLEND);

		glFlush();

	glPopMatrix();
}


