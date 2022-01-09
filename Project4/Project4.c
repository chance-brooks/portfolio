/*
 * template.c
 *
 * An OpenGL source code template.
 */


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "../mylib/initShader.h"
#include "../mylib/matlib.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices = 0;
int num_colors = 0;
int cubie_verts = 132;
int fTurn, bTurn, rTurn, lTurn, uTurn, dTurn, shuffle, count;
int indices[27], temp[27];

GLfloat left = -0.05, right = 0.05;
GLfloat top = 0.1, bottom = 0.0;
GLfloat near = -0.15, far = -250.0;

float zoom = 1.0;
float glx1, gly1, glz1, glx2, gly2, glz2;
float border_thickness = 0.05;
float cube_size = 0.35;
float theta = 0.0, phi = 0.0;
float fOmega = 0.0, bOmega = 0.0, lOmega = 0.0, rOmega = 0.0, uOmega = 0.0, dOmega = 0.0;
float stoppingPoint, omega = M_PI/128.0;


vec4 v, u, unitV;
vec4 eye = {0.0, 0.0, 1.0, 0.0}, at = {0.0, 0.0, 0.0, 0.0}, up = {0.0, 5.0, 0.0, 0.0};

vec4 red = {1.0, 0, 0, 1.0};
vec4 orange = {1.0, 0.647, 0, 1.0};
vec4 yellow = {1.0, 1.0, 0.0, 1.0};
vec4 green = {0, 1.0, 0, 1.0};
vec4 blue = {0, 0, 1.0, 1.0};
vec4 white = {1.0, 1.0, 1.0, 1.0};
vec4 black = {0, 0, 0, 1.0};
    
vec4 vertices[3564];
vec4 colors[3564];

GLuint projection_location;
mat4 projection_matrix =  {{1,0,0,0},
                           {0,1,0,0},
                           {0,0,1,0},
                           {0,0,0,1}};

GLuint model_view_location;
mat4 model_view =  {{1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,0},
                    {0,0,0,1}};

mat4 horizontal = {{1,0,0,0},
                   {0,1,0,0},
                   {0,0,1,0},
                   {0,0,0,1}};

mat4 vertical = {{1,0,0,0},
                 {0,1,0,0},
                 {0,0,1,0},
                 {0,0,0,1}};

mat4 zoom_model =  {{1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,0},
                    {0,0,0,1}};

GLuint ctm_location;
mat4 ctm = {{1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}};

mat4 old_zoom, xRot, oldHorizontal, oldVertical;
mat4 ctms[27];

float radians(float degrees)
{
    return degrees * (M_PI / 180.0);
}

float toGLCoord(int winCoord) 
{
    //SPLIT AT 255.5
    float GLCoord;

    if ((winCoord>255) & (winCoord<256))
        GLCoord = 0;
    else if (winCoord<=255)
        GLCoord = -1 + winCoord/256.0;
    else if (winCoord>=256)
        GLCoord = 0 + (winCoord-256)/256.0;

    return GLCoord;
}

mat4 look_at(vec4 eye, vec4 at, vec4 up)
{
    vec4 nPrime, uPrime, vPrime;

    nPrime = vecSub(eye, at);
    nPrime = normalize(nPrime);

    uPrime = crossProd(up, nPrime);
    uPrime = normalize(uPrime);

    vPrime = crossProd(nPrime, uPrime);
    vPrime = normalize(vPrime);

    mat4 rotMat = {{uPrime.x, vPrime.x, nPrime.x, 0},
                   {uPrime.y, vPrime.y, nPrime.y, 0},
                   {uPrime.z, vPrime.z, nPrime.z, 0},
                   {0, 0, 0, 1}};
    
    mat4 transMat = {{1, 0, 0, 0},
                     {0, 1, 0, 0},
                     {0, 0, 1, 0},
                     {-eye.x, -eye.y, -eye.z, 1}};
    return matMult(transMat, rotMat);
}

mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far) 
{
    mat4 projMat = {{-2*near/(right-left), 0, 0, 0},
                    {0, -2*near/(top-bottom), 0, 0},
                    {(left+right)/(right-left), (bottom+top)/(top-bottom), (near+far)/(far-near), -1},
                    {0, 0, -2*near*far/(far-near), 0}};
    return projMat;
}

void shuffleCube()
{
    if (count == 30) shuffle = 0;
    else
    {
        srand(time(0));
        int face = rand() % 12;
        
        if (face == 1)
        {
            omega = -M_PI/128.0;
            stoppingPoint = fOmega - M_PI/2.0;
            fTurn = 1;
        }

        else if (face == 2)
        {
            omega = M_PI/128.0;
            stoppingPoint = fOmega + M_PI/2.0;
            fTurn = -1;
        }

        else if (face == 3)
        {
            omega = -M_PI/128.0;
            stoppingPoint = bOmega - M_PI/2.0;
            bTurn = 1;
        }

        else if (face == 4)
        {
            omega = M_PI/128.0;
            stoppingPoint = bOmega + M_PI/2.0;
            bTurn = -1;
        }

        else if (face == 5)
        {
            omega = -M_PI/128.0;
            stoppingPoint = rOmega - M_PI/2.0;
            rTurn = 1;
        }

        else if (face == 6)
        {
            omega = M_PI/128.0;
            stoppingPoint = rOmega + M_PI/2.0;
            rTurn = -1;
        }

        else if (face == 7)
        {
            omega = -M_PI/128.0;
            stoppingPoint = lOmega - M_PI/2.0;
            lTurn = 1;
        }

        else if (face == 8)
        {
            omega = M_PI/128.0;
            stoppingPoint = lOmega + M_PI/2.0;
            lTurn = -1;
        }

        else if (face == 9)
        {
            omega = -M_PI/128.0;
            stoppingPoint = uOmega - M_PI/2.0;
            uTurn = 1;
        }

        else if (face == 10)
        {
            omega = M_PI/128.0;
            stoppingPoint = uOmega + M_PI/2.0;
            uTurn = -1;
        }

        else if (face == 11)
        {
            omega = -M_PI/128.0;
            stoppingPoint = dOmega - M_PI/2.0;
            dTurn = 1;
        }

        else if (face == 12)
        {
            omega = M_PI/128.0;
            stoppingPoint = dOmega + M_PI/2.0;
            dTurn = -1;
        }

        else shuffleCube();
    }
}

void makeCube(float top, float bottom, float left, float right, float front, float back)
{
    //front face
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = green;
    num_colors = num_vertices;

    //right face
    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = red;
    num_colors = num_vertices;

    //back face
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = blue;
    num_colors = num_vertices;

    //left face
    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = orange;
    num_colors = num_vertices;

    //top face
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = white;
    num_colors = num_vertices;

    //bottom face
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = yellow;
    num_colors = num_vertices;

    //top borders - front
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0; 
    
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;  

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //top borders - right
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //top borders - back
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0; 
    
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //top borders - left
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom borders - front
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0; 
    
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom borders - right
    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = right+border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom borders - back
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom borders - left
    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //front borders - right
    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //front borders - left
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //back borders - right
    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //back borders - left
    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //top front right corner
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //top front left corner
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //top back right corner
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0; 

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //top back left corner
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top + border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = top;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom front right corner
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0; 

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom front left corner
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = front - border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom back right corner
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;
    
    vertices[num_vertices].x = right; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0; 

    vertices[num_vertices].x = right + border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;

    //bottom back left corner
    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom - border_thickness;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left - border_thickness; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back + border_thickness;
    vertices[num_vertices++].w = 1.0;

    vertices[num_vertices].x = left; 
    vertices[num_vertices].y = bottom;
    vertices[num_vertices].z = back;
    vertices[num_vertices++].w = 1.0; 

    for (int i = num_colors; i<num_vertices; i++)
        colors[i] = black;
    num_colors = num_vertices;
}

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view");
    projection_location = glGetUniformLocation(program, "projection_matrix");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);

    for (int i=0; i<27; i++) 
    {
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctms[i]);
        glDrawArrays(GL_TRIANGLES, i*cubie_verts, cubie_verts);
    }

    glutSwapBuffers();
}

void idle()
{
    //front       middle        back
    //5 3 4       14 12 13      23 21 22
    //2 0 1       11 9  10      20 18 19
    //8 6 7       17 15 16      26 24 25
    for (int i=0; i<27; i++)
        temp[i] = indices[i];

    if (dTurn == -1)
    {
        dOmega += M_PI/128.0;
        if (dOmega > stoppingPoint) omega = omega-(dOmega-stoppingPoint);
        mat4 yRot = {{cos(omega), 0, -sin(omega), 0},
                    {0, 1, 0, 0},
                    {sin(omega), 0, cos(omega), 0},
                    {0, 0, 0, 1}};
        for (int i=6; i<27; i+=9)
        {
            ctms[indices[i]] = matMult(ctms[indices[i]], yRot);
            ctms[indices[i+1]] = matMult(ctms[indices[i+1]], yRot);
            ctms[indices[i+2]] = matMult(ctms[indices[i+2]], yRot);
        }
        if (dOmega >= stoppingPoint)
        {
            indices[8] = temp[26];
            indices[6] = temp[17];
            indices[7] = temp[8];
            indices[16] = temp[6];
            indices[25] = temp[7];
            indices[24] = temp[16];
            indices[26] = temp[25];
            indices[17] = temp[24];
            dTurn = 0;
            
            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (dTurn == 1)
    {
        dOmega -= M_PI/128.0;
        if (dOmega < stoppingPoint) omega = omega-(dOmega-stoppingPoint);
        mat4 yRot = {{cos(omega), 0, -sin(omega), 0},
                    {0, 1, 0, 0},
                    {sin(omega), 0, cos(omega), 0},
                    {0, 0, 0, 1}};
        for (int i=6; i<27; i+=9)
        {
            ctms[indices[i]] = matMult(ctms[indices[i]], yRot);
            ctms[indices[i+1]] = matMult(ctms[indices[i+1]], yRot);
            ctms[indices[i+2]] = matMult(ctms[indices[i+2]], yRot);
        }
        if (dOmega <= stoppingPoint)
        {
            indices[8] = temp[7];
            indices[6] = temp[16];
            indices[7] = temp[25];
            indices[16] = temp[24];
            indices[25] = temp[26];
            indices[24] = temp[17];
            indices[26] = temp[8];
            indices[17] = temp[6];
            dTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (uTurn == -1)
    {
        uOmega += M_PI/128.0;
        if (uOmega > stoppingPoint) omega = omega-(uOmega-stoppingPoint);
        mat4 yRot = {{cos(omega), 0, -sin(omega), 0},
                    {0, 1, 0, 0},
                    {sin(omega), 0, cos(omega), 0},
                    {0, 0, 0, 1}};
        for (int i=3; i<27; i+=9)
        {
            ctms[indices[i]] = matMult(ctms[indices[i]], yRot);
            ctms[indices[i+1]] = matMult(ctms[indices[i+1]], yRot);
            ctms[indices[i+2]] = matMult(ctms[indices[i+2]], yRot);
        }
        if (uOmega >= stoppingPoint)
        {
            indices[5] = temp[23];
            indices[3] = temp[14];
            indices[4] = temp[5];
            indices[13] = temp[3];
            indices[22] = temp[4];
            indices[21] = temp[13];
            indices[23] = temp[22];
            indices[14] = temp[21];
            uTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }
    
    else if (uTurn == 1)
    {
        uOmega -= M_PI/128.0;
        if (uOmega < stoppingPoint) omega = omega-(uOmega-stoppingPoint);
        mat4 yRot = {{cos(omega), 0, -sin(omega), 0},
                    {0, 1, 0, 0},
                    {sin(omega), 0, cos(omega), 0},
                    {0, 0, 0, 1}};
        for (int i=3; i<27; i+=9)
        {
            ctms[indices[i]] = matMult(ctms[indices[i]], yRot);
            ctms[indices[i+1]] = matMult(ctms[indices[i+1]], yRot);
            ctms[indices[i+2]] = matMult(ctms[indices[i+2]], yRot);
        }
        if (uOmega <= stoppingPoint)
        {
            indices[5] = temp[4];
            indices[3] = temp[13];
            indices[4] = temp[22];
            indices[13] = temp[21];
            indices[22] = temp[23];
            indices[21] = temp[14];
            indices[23] = temp[5];
            indices[14] = temp[3];
            uTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (lTurn == 1)
    {
        lOmega -= M_PI/128.0;
        if (lOmega < stoppingPoint) omega = omega-(lOmega-stoppingPoint);
        mat4 xRot = {{1, 0, 0, 0},
                    {0, cos(omega), sin(omega), 0},
                    {0, -sin(omega), cos(omega), 0},
                    {0, 0, 0, 1}};

        for (int i=2; i<27; i+=3)
            ctms[indices[i]] = matMult(ctms[indices[i]], xRot);
  
        if (lOmega <= stoppingPoint)
        {
            indices[5] = temp[8];
            indices[2] = temp[17];
            indices[8] = temp[26];
            indices[17] = temp[20];
            indices[26] = temp[23];
            indices[20] = temp[14];
            indices[23] = temp[5];
            indices[14] = temp[2];
            lTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (lTurn == -1)
    {
        lOmega += M_PI/128.0;
        if (lOmega > stoppingPoint) omega = omega-(lOmega-stoppingPoint);
        mat4 xRot = {{1, 0, 0, 0},
                    {0, cos(omega), sin(omega), 0},
                    {0, -sin(omega), cos(omega), 0},
                    {0, 0, 0, 1}};

        for (int i=2; i<27; i+=3)
            ctms[indices[i]] = matMult(ctms[indices[i]], xRot);
  
        if (lOmega >= stoppingPoint)
        {
            indices[5] = temp[23];
            indices[2] = temp[14];
            indices[8] = temp[5];
            indices[17] = temp[2];
            indices[26] = temp[8];
            indices[20] = temp[17];
            indices[23] = temp[26];
            indices[14] = temp[20];
            lTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (rTurn == 1)
    {
        rOmega -= M_PI/128.0;
        if (rOmega < stoppingPoint) omega = omega-(rOmega-stoppingPoint);
        mat4 xRot = {{1, 0, 0, 0},
                    {0, cos(omega), sin(omega), 0},
                    {0, -sin(omega), cos(omega), 0},
                    {0, 0, 0, 1}};

        for (int i=1; i<27; i+=3)
            ctms[indices[i]] = matMult(ctms[indices[i]], xRot);

        if (rOmega <= stoppingPoint)
        {
            indices[4] = temp[7];
            indices[13] = temp[1];
            indices[22] = temp[4];
            indices[19] = temp[13];
            indices[25] = temp[22];
            indices[16] = temp[19];
            indices[7] = temp[25];
            indices[1] = temp[16];
            rTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (rTurn == -1)
    {
        rOmega += M_PI/128.0;
        if (rOmega > stoppingPoint) omega = omega-(rOmega-stoppingPoint);
        mat4 xRot = {{1, 0, 0, 0},
                    {0, cos(omega), sin(omega), 0},
                    {0, -sin(omega), cos(omega), 0},
                    {0, 0, 0, 1}};

        for (int i=1; i<27; i+=3)
            ctms[indices[i]] = matMult(ctms[indices[i]], xRot);

        if (rOmega >= stoppingPoint)
        {
            indices[4] = temp[22];
            indices[13] = temp[19];
            indices[22] = temp[25];
            indices[19] = temp[16];
            indices[25] = temp[7];
            indices[16] = temp[1];
            indices[7] = temp[4];
            indices[1] = temp[13];
            rTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (bTurn == -1)
    {
        bOmega += M_PI/128.0;
        if (bOmega > stoppingPoint) omega = omega-(bOmega-stoppingPoint);
        mat4 zRot = {{cos(omega), sin(omega), 0, 0},
                    {-sin(omega), cos(omega), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};

        for (int i=18; i<27; i++)
            ctms[indices[i]] = matMult(ctms[indices[i]], zRot);

        if (bOmega >= stoppingPoint)
        {
            indices[23] = temp[22];
            indices[21] = temp[19];
            indices[22] = temp[25];
            indices[19] = temp[24];
            indices[25] = temp[26];
            indices[24] = temp[20];
            indices[26] = temp[23];
            indices[20] = temp[21];
            bTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (bTurn == 1)
    {
        bOmega -= M_PI/128.0;
        if (bOmega < stoppingPoint) omega = omega-(bOmega-stoppingPoint);
        mat4 zRot = {{cos(omega), sin(omega), 0, 0},
                    {-sin(omega), cos(omega), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};

        for (int i=18; i<27; i++)
            ctms[indices[i]] = matMult(ctms[indices[i]], zRot);

        if (bOmega <= stoppingPoint)
        {
            indices[23] = temp[26];
            indices[21] = temp[20];
            indices[22] = temp[23];
            indices[19] = temp[21];
            indices[25] = temp[22];
            indices[24] = temp[19];
            indices[26] = temp[25];
            indices[20] = temp[24];
            bTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (fTurn == 1)
    {

        fOmega -= M_PI/128.0;
        if (fOmega < stoppingPoint) omega = omega-(fOmega-stoppingPoint);
        mat4 zRot = {{cos(omega), sin(omega), 0, 0},
                    {-sin(omega), cos(omega), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};

        for (int i=0; i<9; i++)
            ctms[indices[i]] = matMult(ctms[indices[i]], zRot);

        if (fOmega <= stoppingPoint)
        {
            indices[7] = temp[4];
            indices[1] = temp[3];
            indices[4] = temp[5];
            indices[3] = temp[2];
            indices[5] = temp[8];
            indices[2] = temp[6];
            indices[8] = temp[7];
            indices[6] = temp[1];
            fTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    else if (fTurn == -1)
    {
        fOmega += M_PI/128.0;
        if (fOmega > stoppingPoint) omega = omega-(fOmega-stoppingPoint);
        mat4 zRot = {{cos(omega), sin(omega), 0, 0},
                    {-sin(omega), cos(omega), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};

        for (int i=0; i<9; i++)
            ctms[indices[i]] = matMult(ctms[indices[i]], zRot);
       
        if (fOmega >= stoppingPoint)
        {
            indices[7] = temp[8];
            indices[1] = temp[6];
            indices[4] = temp[7];
            indices[3] = temp[1];
            indices[5] = temp[4];
            indices[2] = temp[3];
            indices[8] = temp[5];
            indices[6] = temp[2];
            fTurn = 0;

            if (shuffle == 1)
            {
                count++;
                shuffleCube();
            }
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
        glutLeaveMainLoop();

    if (fTurn==0 && bTurn==0 && rTurn==0 && lTurn==0 && uTurn==0 && dTurn==0 && shuffle == 0)
    {
        if (key == 'F')
        {
            omega = -M_PI/128.0;
            stoppingPoint = fOmega - M_PI/2.0;
            fTurn = 1;
        }

        if (key == 'f')
        {
            omega = M_PI/128.0;
            stoppingPoint = fOmega + M_PI/2.0;
            fTurn = -1;
        }

        if (key == 'B')
        {
            omega = -M_PI/128.0;
            stoppingPoint = bOmega - M_PI/2.0;
            bTurn = 1;
        }

        if (key == 'b')
        {
            omega = M_PI/128.0;
            stoppingPoint = bOmega + M_PI/2.0;
            bTurn = -1;
        }

        if (key == 'R')
        {
            omega = -M_PI/128.0;
            stoppingPoint = rOmega - M_PI/2.0;
            rTurn = 1;
        }

        if (key == 'r')
        {
            omega = M_PI/128.0;
            stoppingPoint = rOmega + M_PI/2.0;
            rTurn = -1;
        }

        if (key == 'L')
        {
            omega = -M_PI/128.0;
            stoppingPoint = lOmega - M_PI/2.0;
            lTurn = 1;
        }

        if (key == 'l')
        {
            omega = M_PI/128.0;
            stoppingPoint = lOmega + M_PI/2.0;
            lTurn = -1;
        }

        if (key == 'U')
        {
            omega = -M_PI/128.0;
            stoppingPoint = uOmega - M_PI/2.0;
            uTurn = 1;
        }

        if (key == 'u')
        {
            omega = M_PI/128.0;
            stoppingPoint = uOmega + M_PI/2.0;
            uTurn = -1;
        }

        if (key == 'D')
        {
            omega = -M_PI/128.0;
            stoppingPoint = dOmega - M_PI/2.0;
            dTurn = 1;
        }

        if (key == 'd')
        {
            omega = M_PI/128.0;
            stoppingPoint = dOmega + M_PI/2.0;
            dTurn = -1;
        }

        if (key == 'S' || key == 's') 
        {
            shuffle = 1;
            count = 0;
            shuffleCube();
        }
    }

    if (key == '+') 
    {
        zoom = zoom * 1.02;
        mat4 new_zoom =  {{zoom,0,0,0},
                        {0,zoom,0,0},
                        {0,0,zoom,0},
                        {0,0,0,1}};
        old_zoom = zoom_model;
        zoom_model = new_zoom;
        model_view = matMult(model_view, inverse(old_zoom));
        model_view = matMult(model_view, zoom_model);
    }

    if (key == '-')
    {
        zoom = zoom / 1.02;
        mat4 new_zoom =  {{zoom,0,0,0},
                        {0,zoom,0,0},
                        {0,0,zoom,0},
                        {0,0,0,1}};
        old_zoom = zoom_model;
        zoom_model = new_zoom;
        model_view = matMult(model_view, inverse(old_zoom));
        model_view = matMult(model_view, zoom_model);
    }

    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    oldHorizontal = horizontal;
    oldVertical = vertical;

    if (key == GLUT_KEY_UP)
    {
        if (phi < M_PI/6) phi += (M_PI/192.0);
        mat4 xRot = {{1, 0, 0, 0},
                    {0, cos(phi), sin(phi), 0},
                    {0, -sin(phi), cos(phi), 0},
                    {0, 0, 0, 1}};

        vertical = matMult(vertical, inverse(vertical));
        vertical = matMult(vertical, xRot);
        model_view = matMult(model_view, inverse(oldVertical));
        model_view = matMult(model_view, vertical);
    }

    if (key == GLUT_KEY_DOWN)
    {
        if (phi > -M_PI/6) phi -= (M_PI/192.0);
        mat4 xRot = {{1, 0, 0, 0},
                    {0, cos(phi), sin(phi), 0},
                    {0, -sin(phi), cos(phi), 0},
                    {0, 0, 0, 1}};

        vertical = matMult(vertical, inverse(vertical));
        vertical = matMult(vertical, xRot);
        model_view = matMult(model_view, inverse(oldVertical));
        model_view = matMult(model_view, vertical);
    }

    if (key == GLUT_KEY_RIGHT)
    {
        /*if (theta > -M_PI/6)*/ theta -= (M_PI/192.0);
        mat4 yRot = {{cos(theta), 0, -sin(theta), 0},
                    {0, 1, 0, 0},
                    {sin(theta), 0, cos(theta), 0},
                    {0, 0, 0, 1}};

        horizontal = matMult(horizontal, inverse(horizontal));
        horizontal = matMult(horizontal, yRot);
        model_view = matMult(model_view, inverse(oldHorizontal));
        model_view = matMult(model_view, horizontal);
    }

    if (key == GLUT_KEY_LEFT)
    {

        /*if (theta < M_PI/6)*/ theta += (M_PI/192.0);
        mat4 yRot = {{cos(theta), 0, -sin(theta), 0},
                    {0, 1, 0, 0},
                    {sin(theta), 0, cos(theta), 0},
                    {0, 0, 0, 1}};

        horizontal = matMult(horizontal, inverse(horizontal));
        horizontal = matMult(horizontal, yRot);
        model_view = matMult(model_view, inverse(oldHorizontal));
        model_view = matMult(model_view, horizontal);
    }

    glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

int main(int argc, char **argv)
{
    for (int i=0; i<27; i++)
    {
        mat4 initializer = {{1,0,0,0},
                            {0,1,0,0},
                            {0,0,1,0},
                            {0,0,0,1}};
        ctms[i] = initializer;
        indices[i] = i;
    }

    float top = 0.125;
    float bottom = -0.125;
    float left = -0.125;
    float right = 0.125;
    float front = 0.525;
    float back = 0.175;

    for (int i=0; i<3; i++)
    {
        //middle row
        makeCube(top, bottom, left, right, front, back);

        right += cube_size;
        left += cube_size;
        makeCube(top, bottom, left, right, front, back);

        right -= 2*cube_size;
        left -= 2*cube_size;
        makeCube(top, bottom, left, right, front, back);

        //top row
        right += cube_size;
        left += cube_size;
        top += cube_size;
        bottom += cube_size;
        makeCube(top, bottom, left, right, front, back);

        right += cube_size;
        left += cube_size;
        makeCube(top, bottom, left, right, front, back);

        right -= 2*cube_size;
        left -= 2*cube_size;
        makeCube(top, bottom, left, right, front, back);

        //bottom row
        right += cube_size;
        left += cube_size;
        top -= 2*cube_size;
        bottom -= 2*cube_size;
        makeCube(top, bottom, left, right, front, back);

        right += cube_size;
        left += cube_size;
        makeCube(top, bottom, left, right, front, back);

        right -= 2*cube_size;
        left -= 2*cube_size;
        makeCube(top, bottom, left, right, front, back);

        //reset
        top += cube_size;
        bottom += cube_size;
        right += cube_size;
        left += cube_size;
        front -= cube_size;
        back -= cube_size;
    }

    printf("###########################\n");
    printf("Arrow Keys: Look around cube\n");
    printf("+/-: Zoom\n");
    printf("F/f: Turn front face\n");
    printf("B/b: Turn back face\n");
    printf("R/r: Turn right face\n");
    printf("L/l: Turn left face\n");
    printf("U/u: Turn top face\n");
    printf("D/d: Turn bottom face\n");
    printf("S/s: Shuffle cube\n");
    printf("###########################\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(1000,100);
    glutCreateWindow("Project 4");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
