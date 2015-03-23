/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  

	Material *mat = ray.intersection.mat;

	// Calculate necessary vectors for colour calculation
	Vector3D normal = ray.intersection.normal;
	Vector3D pointSourceDir = _pos - ray.intersection.point;	
	Vector3D cameraDir = -ray.dir;	

	normal.normalize();
	pointSourceDir.normalize();
	cameraDir.normalize();

	Vector3D mirrorDir = (2 * (normal.dot(pointSourceDir)) * normal) - pointSourceDir;
	mirrorDir.normalize();

	// Calculate Ambient Colour
	Colour ambient = mat->ambient * _col_ambient;

	// Calculate Diffuse Colour
	Colour diffuse = fmax(0, normal.dot(pointSourceDir)) * mat->diffuse * _col_diffuse;

	// Calculate Specular Colour
	float specularHighlight =  pow(fmax(0, mirrorDir.dot(cameraDir)), mat->specular_exp);
	Colour specular = specularHighlight * mat->specular * _col_specular;

	ray.col = ambient + diffuse + specular;
	ray.col.clamp();

}

