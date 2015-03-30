/*
	Stucture for particles
*/
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#define MAX_PARTICLES 300
#define MAX_PARTICLE_AGE 150
#define WINDOW_W 11
#define WINDOW_H 9

class ParticleEffect 
{
public:
	struct Particle 
	{
		float X, Y, Z;		// Current position
		float sX, sY, sZ;	// Current speed
		float tX, tY, tZ;	// Target speed
		//float R, G, B;		// Colour
		bool active;
		bool start;
		int age;
		int maxAge;			// Max age before it dies
	} Particles[MAX_PARTICLES];

	void initParticles();
	void setParticles();
	void updateParticles();
	void renderParticles();
	void clearParticles();
	bool loadBitmapTexture(char* filename);

	GLubyte face[6][64][64][3];

	GLuint txParticle;			// Holds the info for the texture of the particle
	float part_size;			// Size of the particle
	int win[2];					// window (x,y) size
	

	// TODO: Create a clearParticles function -- 
	// this should reinit the particles to random places again (once implement random starting)
};

#endif // __PARTICLE_H__
