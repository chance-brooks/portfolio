typedef struct
{
    float s;
    float t;
} vec2;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct {
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;

//vector functions

extern void printVec(vec4);

extern vec4 scalVecMult(float, vec4);

extern vec4 vecAdd(vec4, vec4);

extern vec4 vecSub(vec4, vec4);

extern float magnitude(vec4);

extern vec4 normalize(vec4);

extern float dotProd(vec4, vec4);

extern vec4 crossProd(vec4, vec4);

//matrix funtions

extern void printMat(mat4);

extern mat4 scalMatMult(float, mat4);

extern mat4 matAdd(mat4, mat4);

extern mat4 matSub(mat4, mat4);

extern mat4 matMult(mat4, mat4);

extern mat4 inverse(mat4);

extern mat4 transpose(mat4);

extern vec4 matVecMult(mat4, vec4);

extern float det3x3(float, float, float, float, float, float, float, float, float);