/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include <algorithm>
#include "scene_object.h"

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	// Convert ray to Object space	
	Ray3D objRay;
	objRay.origin = worldToModel * ray.origin;
	objRay.dir = worldToModel * ray.dir;

	// Calculate lambda where [[0.5, -0.5, 0] - [origin - dir*lambda]] . normal = 0
	// Simplifies to cz - dirz*lambda = 0  ==  lambda = -cz/dirz
	float lambda = -objRay.origin[2]/objRay.dir[2];		
	if (lambda <= 0) {
		return false;
	}

	// Using lambda, calculate the intersection point of the ray and the square
	float intersectX = objRay.origin[0] + (lambda * objRay.dir[0]);
	float intersectY = objRay.origin[1] + (lambda * objRay.dir[1]);

	bool isXIntersecting = -0.5 >= intersectX && intersectX <= 0.5;
	bool isYIntersecting = -0.5 >= intersectY && intersectY <= 0.5;

	// Does ray intersect with unit square and is closer to the existing intersection?
	if (isXIntersecting && isYIntersecting && (ray.intersection.none || ray.intersection.t_value > lambda)) {		

		ray.intersection.point = modelToWorld * Point3D(intersectX, intersectY, 0);
		ray.intersection.normal = modelToWorld * Vector3D(0, 0, 1);
		ray.intersection.normal.normalize();
		ray.intersection.none = false;
		ray.intersection.t_value = lambda;
		return true;

	} else {
		// Intersecting point does not go through unit square/exists a closer point
		return false;
	}	
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	// Convert ray to Object space	
	Ray3D objRay;
	objRay.origin = worldToModel * ray.origin;
	objRay.dir = worldToModel * ray.dir;

	// Calculate A, B, and C for quadratic equation At^2 + 2Bt + C = 0
	Point3D sphereOrigin(0, 0, 0);
	float A = (objRay.dir).dot(objRay.dir);
	float B = 2 * (objRay.origin - sphereOrigin).dot(objRay.dir);
	float C = (objRay.origin - sphereOrigin).dot(objRay.origin - sphereOrigin) - 1;
	
	// Calculate determinant
	float det = B*B - 4*A*C;

	if (det < 0) {
		// Does not intersect 
		return false;
	}

	// At this point, the ray must intersect the sphere somewhere
	float lambda;
	float lambda1 = (-B + sqrt(det)) / 2*A;
	float lambda2 = (-B - sqrt(det)) / 2*A;
	
	if (lambda1 < 0 && lambda2 < 0) {
		// Intersections behind view plane. Not visible
		return false;
	} else if (lambda1 < 0 || lambda2 < 0) {
		// Exists one intersection that is off screen
		lambda = std::max(lambda1, lambda2);
	} else {
		// Both intersections on screen. Choose closer one
		lambda = std::min(lambda1, lambda2);
	}

	if (ray.intersection.none || lambda < ray.intersection.t_value) {
		Point3D intersection(objRay.origin + lambda*objRay.dir);

		ray.intersection.point = modelToWorld * intersection;
		ray.intersection.normal = worldToModel.transpose() * (intersection - sphereOrigin);
		ray.intersection.normal.normalize();
		ray.intersection.none = false;
		ray.intersection.t_value = lambda;
		return true;

	} else {
		// Closer intersection exists
		return false;

	}	

}

