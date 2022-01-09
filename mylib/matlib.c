#include "matlib.h"
#include <stdio.h>
#include <math.h>
//****************

//vector functions

void printVec(vec4 v){
	printf("\n%f \n%f \n%f \n%f \n", v.x, v.y, v.z, v.w);
}

vec4 scalVecMult(float s, vec4 v){
	v.x = v.x * s;
	v.y = v.y * s;
	v.z = v.z * s;
	v.w = v.w * s;
	return v;
}

vec4 vecAdd(vec4 v1, vec4 v2){
	v1.x = v1.x + v2.x;
	v1.y = v1.y + v2.y;
	v1.z = v1.z + v2.z;
	v1.w = v1.w + v2.w;
	return v1;	
}

vec4 vecSub(vec4 v1, vec4 v2){
	v1.x = v1.x - v2.x;
	v1.y = v1.y - v2.y;
	v1.z = v1.z - v2.z;
	v1.w = v1.w - v2.w;
	return v1;	
}

float magnitude(vec4 v){
	float mag = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
	return mag;
}

vec4 normalize(vec4 v){
	float mag = magnitude(v);
	vec4 result = scalVecMult(1.0/mag, v);
	return result;
}

float dotProd(vec4 v1, vec4 v2){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

vec4 crossProd(vec4 v1, vec4 v2){
	vec4 result;
	result.x = v1.y*v2.z - v1.z*v2.y;
	result.y = v1.z*v2.x - v1.x*v2.z;
	result.z = v1.x*v2.y - v1.y*v2.x;
	result.w = 0;
	return result;
}

//matrix funtions

void printMat(mat4 m){
	printf("\n%f %f %f %f \n", m.x.x, m.y.x, m.z.x, m.w.x);
	printf("%f %f %f %f \n", m.x.y, m.y.y, m.z.y, m.w.y);
	printf("%f %f %f %f \n", m.x.z, m.y.z, m.z.z, m.w.z);
	printf("%f %f %f %f \n", m.x.w, m.y.w, m.z.w, m.w.w);
}

mat4 scalMatMult(float s, mat4 m){
	m.x = scalVecMult(s, m.x);
	m.y = scalVecMult(s, m.y);
	m.z = scalVecMult(s, m.z);
	m.w = scalVecMult(s, m.w);
	return m;
}

mat4 matAdd(mat4 m1, mat4 m2){
	m1.x = vecAdd(m1.x, m2.x);
	m1.y = vecAdd(m1.y, m2.y);
	m1.z = vecAdd(m1.z, m2.z);
	m1.w = vecAdd(m1.w, m2.w);
	return m1;
}

mat4 matSub(mat4 m1, mat4 m2){
	m1.x = vecSub(m1.x, m2.x);
	m1.y = vecSub(m1.y, m2.y);
	m1.z = vecSub(m1.z, m2.z);
	m1.w = vecSub(m1.w, m2.w);
	return m1;
}

mat4 matMult(mat4 m1, mat4 m2){
	mat4 result;
	vec4 m2Row1 = {m2.x.x, m2.y.x, m2.z.x, m2.w.x};
	vec4 m2Row2 = {m2.x.y, m2.y.y, m2.z.y, m2.w.y};
	vec4 m2Row3 = {m2.x.z, m2.y.z, m2.z.z, m2.w.z};
	vec4 m2Row4 = {m2.x.w, m2.y.w, m2.z.w, m2.w.w};

	result.x.x = dotProd(m1.x, m2Row1);
	result.x.y = dotProd(m1.x, m2Row2);
	result.x.z = dotProd(m1.x, m2Row3);
	result.x.w = dotProd(m1.x, m2Row4);

	result.y.x = dotProd(m1.y, m2Row1);
	result.y.y = dotProd(m1.y, m2Row2);
	result.y.z = dotProd(m1.y, m2Row3);
	result.y.w = dotProd(m1.y, m2Row4);

	result.z.x = dotProd(m1.z, m2Row1);
	result.z.y = dotProd(m1.z, m2Row2);
	result.z.z = dotProd(m1.z, m2Row3);
	result.z.w = dotProd(m1.z, m2Row4);

	result.w.x = dotProd(m1.w, m2Row1);
	result.w.y = dotProd(m1.w, m2Row2);
	result.w.z = dotProd(m1.w, m2Row3);
	result.w.w = dotProd(m1.w, m2Row4);
	return result;
}

mat4 inverse(mat4 m){

	/////CALCULATE WHOLE MATRIX OF MINORS AND APPLY CHECKERBOARD TO THAT 
	mat4 matMinor;
	matMinor.x.x = det3x3(m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w);
	matMinor.x.y = det3x3(m.y.x, m.z.x, m.w.x, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w);
	matMinor.x.z = det3x3(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.w, m.z.w, m.w.w);
	matMinor.x.w = det3x3(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z);

	matMinor.y.x = det3x3(m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w);
	matMinor.y.y = det3x3(m.x.x, m.z.x, m.w.x, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w);
	matMinor.y.z = det3x3(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.w, m.z.w, m.w.w);
	matMinor.y.w = det3x3(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z);

	matMinor.z.x = det3x3(m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w);
	matMinor.z.y = det3x3(m.x.x, m.y.x, m.w.x, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w);
	matMinor.z.z = det3x3(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.w, m.y.w, m.w.w);
	matMinor.z.w = det3x3(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z);

	matMinor.w.x = det3x3(m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w);
	matMinor.w.y = det3x3(m.x.x, m.y.x, m.z.x, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w);
	matMinor.w.z = det3x3(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.w, m.y.w, m.z.w);
	matMinor.w.w = det3x3(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z);

	float det = m.x.x*matMinor.x.x - m.y.x*matMinor.y.x + m.z.x*matMinor.z.x - m.w.x*matMinor.w.x;

	matMinor.y.x = -matMinor.y.x;
	matMinor.w.x = -matMinor.w.x;
	matMinor.x.y = -matMinor.x.y;
	matMinor.z.y = -matMinor.z.y;
	matMinor.y.z = -matMinor.y.z;
	matMinor.w.z = -matMinor.w.z;
	matMinor.x.w = -matMinor.x.w;
	matMinor.z.w = -matMinor.z.w;

	matMinor = transpose(matMinor);
	mat4 inverse = scalMatMult(1/det, matMinor);
	return inverse;
}

mat4 transpose(mat4 m){
	mat4 result;
	vec4 row1 = {m.x.x, m.y.x, m.z.x, m.w.x};
	result.x = row1;

	vec4 row2 = {m.x.y, m.y.y, m.z.y, m.w.y};
	result.y = row2;

	vec4 row3 = {m.x.z, m.y.z, m.z.z, m.w.z};
	result.z = row3;

	vec4 row4 = {m.x.w, m.y.w, m.z.w, m.w.w};
	result.w = row4;
	return result;
}

vec4 matVecMult(mat4 m, vec4 v){
	m.x = scalVecMult(v.x, m.x);
	m.y = scalVecMult(v.y, m.y);
	m.z = scalVecMult(v.z, m.z);
	m.w = scalVecMult(v.w, m.w);
	
	vec4 result;
	result.x = m.x.x + m.y.x + m.z.x + m.w.x;
	result.y = m.x.y + m.y.y + m.z.y + m.w.y;
	result.z = m.x.z + m.y.z + m.z.z + m.w.z;
	result.w = m.x.w + m.y.w + m.z.w + m.w.w;
	return result;
}

float det3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i){
	return a*e*i + b*f*g + c*d*h - g*e*c - h*f*a - i*d*b;
}