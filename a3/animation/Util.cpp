/**
 * Code for Skybox Utility found in
 * http://www.davidwparker.com/2012/12/26/opengl-screencast-video-21-skybox/
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
 
#include <GL/gl.h>
#include <GL/glut.h>
#include "Util.h"
 
static void reverse(void* x,const int n)
{
	int k;
	char* ch = (char*)x;
	for (k=0;k<n/2;k++)
	{
		char tmp = ch[k];
		ch[k] = ch[n-1-k];
		ch[n-1-k] = tmp;
	}
}

GLUint loadTexBMP(char* file) {
	unsigned int texture; /* Texture name */
	FILE* f; /* File pointer */
	unsigned short magic; /* Image magic */
	unsigned int dx,dy,size; /* Image dimensions */
	unsigned short nbp,bpp; /* Planes and bits per pixel */
	unsigned char* image; /* Image data */
	unsigned int k; /* Counter */
	
	/* Open file */
	f = fopen(file,"rb");
	if (!f) 
		printf("Cannot open file %s\n",file);

	/* Check image magic */
	if (fread(&magic,2,1,f)!=1) 
		printf("Cannot read magic from %s\n",file);

	if (magic!=0x4D42 && magic!=0x424D) 
		printf("Image magic not BMP in %s\n",file);

	/* Seek to and read header */
	if (fseek(f,16,SEEK_CUR) || fread(&dx ,4,1,f)!=1 || fread(&dy ,4,1,f)!=1 ||
			fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
		printf("Cannot read header from %s\n",file);

	/* Reverse bytes on big endian hardware (detected by backwards magic) */
	if (magic==0x424D) {
		reverse(&dx,4);
		reverse(&dy,4);
		reverse(&nbp,2);
		reverse(&bpp,2);
		reverse(&k,4);
	}
	
	/* Check image parameters */
	if (dx<1 || dx>65536) 
		printf("%s image width out of range: %d\n",file,dx);
	if (dy<1 || dy>65536) 
		printf("%s image height out of range: %d\n",file,dy);
	if (nbp!=1) 
		printf("%s bit planes is not 1: %d\n",file,nbp);
	if (bpp!=24) 
		printf("%s bits per pixel is not 24: %d\n",file,bpp);
	if (k!=0) 
		printf("%s compressed files not supported\n",file);

	/* Allocate image memory */
	size = 3*dx*dy;
	image = (unsigned char*) malloc(size);
	if (!image) 
		printf("Cannot allocate %d bytes of memory for image %s\n",size,file);

	/* Seek to and read image */
	if (fseek(f,20,SEEK_CUR) || fread(image,size,1,f)!=1)
		printf("Error reading data from image %s\n",file);

	fclose(f);

	/* Reverse colors (BGR -> RGB) */
	for (k=0;k<size;k+=3) {
		unsigned char temp = image[k];
		image[k] = image[k+2];
		image[k+2] = temp;
	}

	/* Sanity check */
	/* Generate 2D texture */
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);

	/* Copy image */
	glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
	if (glGetError()) printf("Error in glTexImage2D %s %dx%d\n",file,dx,dy);

	/* Scale linearly when image size doesn't match */
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	/* Free image memory */
	free(image);

	/* Return texture name */
	return texture;
}
