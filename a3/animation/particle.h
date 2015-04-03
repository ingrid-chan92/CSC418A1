/*
	Stucture for particles
*/
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#define MAX_PARTICLES 10000
#define MAX_PARTICLE_AGE 1000
#define WINDOW_W 120
#define WINDOW_H 100

class ParticleEffect 
{
public:
	struct Particle 
	{
		float X, Y, Z;		// Current position
		float sX, sY, sZ;	// Current speed
		float tX, tY, tZ;	// Target speed
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
