/*
 * template.c
 *
 * An OpenGL source code template.
 */


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

#include "../mylib/initShader.h"
#include "../mylib/matlib.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int  num_vertices;
GLuint program;

float zoom = 1.0;
float glx1, gly1, glz1, glx2, gly2, glz2;
float alpha = 0.1, theta = 0.0, phi = 0.0;

GLfloat left = -0.05, right = 0.05;
GLfloat top = 0.1, bottom = 0.0;
GLfloat near = -0.15, far = -250.0;

GLboolean map = 0, walk = 0;

vec4 v, u, unitV;
vec4 eye = {0.0, 0.1, 0.0, 0.0}, at = {0.0, 0.1, -2.0, 0.0}, up = {0.0, 5.0, 0.0, 0.0};
vec4 tempUp, point2, point3;
vec4 green = {0.0, 1.0, 0.0, 1.0};
vec4 blue = {0.0, 0.0, 1.0, 1.0};

vec2 tex_coords[60000];
vec4 vertices[60000];
vec4 colors[60000];

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

mat4 moveMat = {{1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {0,0,0,1}};

mat4 turnMat = {{1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {0,0,0,1}};

mat4 lookMat = {{1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {0,0,0,1}};

GLuint ctm_location;
mat4 ctm = {{1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}};

mat4 zoom_ctm =  {{1,0,0,0},
                  {0,1,0,0},
                  {0,0,1,0},
                  {0,0,0,1}};

mat4 old_zoom, xRot, oldMove, oldTurn, oldLook;

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

mat4 ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far) 
{
    mat4 projMat = {{2.0/(right-left), 0, 0, -(right+left)/(right-left)},
                    {0, 2.0/(top-bottom), 0, -(top+bottom)/(top-bottom)},
                    {0, 0, 2.0/(near-far),   -(near+far)/(near-far)},
                    {0, 0, 0, 1}};
    return projMat; 
}

mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far) 
{
    mat4 projMat = {{-2*near/(right-left), 0, 0, 0},
                    {0, -2*near/(top-bottom), 0, 0},
                    {(left+right)/(right-left), (bottom+top)/(top-bottom), (near+far)/(far-near), -1},
                    {0, 0, -2*near*far/(far-near), 0}};
    return projMat;
}

void init(void)
{
    int width = 1024;
    int height = 1024;
    GLubyte my_texels[width][height][3];

    FILE *fp = fopen("city.data", "r");
    fread(my_texels, width * height * 3, 1, fp);
    fclose(fp);

    program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(tex_coords), tex_coords);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices) + sizeof(colors)));

    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);
    
    ctm_location = glGetUniformLocation(program, "ctm");

    model_view_location = glGetUniformLocation(program, "model_view");

    projection_location = glGetUniformLocation(program, "projection_matrix");

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);

    glUniform1i(glGetUniformLocation(program, "use_color"), 0);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices-9);

    glUniform1i(glGetUniformLocation(program, "use_color"), 1);
    glDrawArrays(GL_TRIANGLES, num_vertices-9, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{ 
    oldMove = moveMat;
    oldTurn = turnMat;
    oldLook = lookMat;
    vec4 difference = vecSub(at, eye);
    vec4 perpendicular = crossProd(vecSub(eye, at), up);
    vec4 direction = normalize(perpendicular);
    float d = sqrt(pow(direction.y,2) + pow(direction.z,2));


    mat4 rotMat = {{d, 0, direction.x, 0},
                   {0, 1, 0, 0},
                   {-direction.x, 0, d, 0},
                   {0, 0, 0, 1}};

    if(key == 'q')
        glutLeaveMainLoop();

    //move forward
    if (key == 'w')
    {
        eye = vecAdd(scalVecMult(alpha, difference), eye);
        at = vecAdd(scalVecMult(alpha, difference), at);
        moveMat = matMult(moveMat, inverse(moveMat));
        moveMat = matMult(moveMat, look_at(eye, at, up));
    }

    //move back
    if (key == 's')
    {
        eye = vecAdd(eye, scalVecMult(-alpha, difference));
        at = vecAdd(at, scalVecMult(-alpha, difference));
        moveMat = matMult(moveMat, inverse(moveMat));
        moveMat = matMult(moveMat, look_at(eye, at, up));
    }

    //move right
    if (key == 'd')
    {
        eye = vecAdd(eye, scalVecMult(-alpha, direction));
        at = vecAdd(at, scalVecMult(-alpha, direction));
        moveMat = matMult(moveMat, inverse(moveMat));
        moveMat = matMult(moveMat, look_at(eye, at, up));
    }

    //move left
    if (key == 'a')
    {
        eye = vecAdd(eye, scalVecMult(alpha, direction));
        at = vecAdd(at, scalVecMult(alpha, direction));
        moveMat = matMult(moveMat, inverse(moveMat));
        moveMat = matMult(moveMat, look_at(eye, at, up));
    }

    //turn right
    if (key == 'l')
    {
        theta += (M_PI/384.0);
        mat4 transMat = {{1, 0, 0, 0},
                         {0, 1, 0, 0},
                         {0, 0, 1, 0},
                         {0, -eye.y, 0, 1}};

        mat4 yRot = {{cos(theta), 0, -sin(theta), 0},
                    {0, 1, 0, 0},
                    {sin(theta), 0, cos(theta), 0},
                    {0, 0, 0, 1}};

        turnMat = matMult(turnMat, inverse(turnMat));
        turnMat = matMult(turnMat, transMat);
        turnMat = matMult(turnMat, yRot);
        turnMat = matMult(turnMat, inverse(transMat));
    }

    //turn left
    if (key == 'j')
    {
        theta -= (M_PI/384.0);
        mat4 transMat = {{1, 0, 0, 0},
                         {0, 1, 0, 0},
                         {0, 0, 1, 0},
                         {0, -eye.y, 0, 1}};

        mat4 yRot = {{cos(theta), 0, -sin(theta), 0},
                    {0, 1, 0, 0},
                    {sin(theta), 0, cos(theta), 0},
                    {0, 0, 0, 1}};

        turnMat = matMult(turnMat, inverse(turnMat));
        turnMat = matMult(turnMat, transMat);
        turnMat = matMult(turnMat, yRot);
        turnMat = matMult(turnMat, inverse(transMat));
    }

    //look up
    if (key == 'i') 
    {
        phi += (M_PI/192.0);
        mat4 transMat = {{1, 0, 0, 0},
                         {0, 1, 0, 0},
                         {0, 0, 1, 0},
                         {-eye.x, -eye.y, 0, 1}};

        mat4 zRot = {{cos(phi), sin(phi), 0, 0},
                    {-sin(phi), cos(phi), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};

        lookMat = matMult(lookMat, inverse(lookMat));
        lookMat = matMult(lookMat, transMat);
        lookMat = matMult(lookMat, rotMat);
        lookMat = matMult(lookMat, zRot);
        lookMat = matMult(lookMat, transpose(rotMat));
        lookMat = matMult(lookMat, inverse(transMat));
    }

    //look down 
    if (key == 'k')
    {
        phi -= (M_PI/192.0);
        mat4 transMat = {{1, 0, 0, 0},
                         {0, 1, 0, 0},
                         {0, 0, 1, 0},
                         {-eye.x, -eye.y, 0, 1}};

        mat4 zRot = {{cos(phi), sin(phi), 0, 0},
                    {-sin(phi), cos(phi), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};

        lookMat = matMult(lookMat, inverse(lookMat));
        lookMat = matMult(lookMat, transMat);
        lookMat = matMult(lookMat, rotMat);
        lookMat = matMult(lookMat, zRot);
        lookMat = matMult(lookMat, transpose(rotMat));
        lookMat = matMult(lookMat, inverse(transMat));
    }

    //map mode
    if (key == 'm')
    {
        map = 1;
        tempUp.x = eye.x;
        tempUp.y = 0.1;
        tempUp.z = eye.z;
        tempUp.w = eye.w;

        point2.x = eye.x-2;
        point2.y = 1;
        point2.z = eye.z+0.5;
        point2.w = 1.0;
        point3.x = eye.x+2;
        point3.y = 1;
        point3.z = eye.z+0.5;
        point3.w = 1.0;

        vertices[num_vertices-3] = eye;
        colors[num_vertices-3] = blue;
        vertices[num_vertices-2] = point2;
        colors[num_vertices-2] = blue;
        vertices[num_vertices-1] = point3;
        colors[num_vertices-1] = blue;
        init();
    }

    //walking mode
    if (key == 'e')
        walk = 1;

    if (key == 'w' || key == 'a' || key == 's' || key == 'd')
    {
        model_view = matMult(model_view, inverse(oldMove));
        model_view = matMult(model_view, moveMat);
    }
    else if (key == 'l' || key == 'j')
    {
        model_view = matMult(model_view, inverse(oldTurn));
        model_view = matMult(model_view, turnMat);        
    }
    else if(key == 'i' || key =='k')
    {
        model_view = matMult(model_view, inverse(oldLook));
        model_view = matMult(model_view, lookMat);
    }
    glutPostRedisplay();
}

void idle()
{
    if (map == 1 && tempUp.y < 200)
    {
        tempUp.y += 1; 
        model_view = look_at(tempUp, eye, at);
    }
    else if (tempUp.y >= 200)
        map = 0;

    if (walk == 1 && tempUp.y > 0.1)
    {
        tempUp.y -= 1;
        model_view = look_at(tempUp, eye, at);
    }
    else if (tempUp.y <= 0.1 && walk == 1)
    {
        walk = 0;
        model_view = look_at(eye, at, up);
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{

    if (button == 3) 
    {
        zoom = zoom * 1.02;
        mat4 new_zoom = {{zoom,0,0,0},
                        {0,zoom,0,0},
                        {0,0,zoom,0},
                        {0,0,0,1}};
        old_zoom = zoom_ctm;
        zoom_ctm = new_zoom;
        ctm = matMult(ctm, inverse(old_zoom));
        ctm = matMult(ctm, zoom_ctm);
    }

    else if (button == 4)
    {
        zoom = zoom / 1.02;
        mat4 new_zoom =  {{zoom,0,0,0},
                        {0,zoom,0,0},
                        {0,0,zoom,0},
                        {0,0,0,1}};
        old_zoom = zoom_ctm;
        zoom_ctm = new_zoom;
        ctm = matMult(ctm, inverse(old_zoom));
        ctm = matMult(ctm, zoom_ctm);
    }

    if (state == GLUT_DOWN) 
    {
        glx1 = toGLCoord(x);
        gly1 = -toGLCoord(y);
        glz1 = sqrt(1 - pow(glx1,2) - pow(gly1,2));
    }
    glutPostRedisplay();
}

void motion(int x, int y)
{
    glx2 = toGLCoord(x);
    gly2 = -toGLCoord(y);
    glz2 = sqrt(1 - pow(glx2,2) - pow(gly2,2));

    if (1 - pow(glx2,2) - pow(gly2,2) >= 0)
    {
        v.x = glx1;
        v.y = gly1;
        v.z = glz1;
        v.w = 0;

        u.x = glx2;
        u.y = gly2;
        u.z = glz2;
        u.w = 0;

        unitV = crossProd(v, u);
        unitV = scalVecMult(1/magnitude(unitV), unitV);

        float mag = acos(dotProd(v,u));
        float d = sqrt(pow(unitV.y,2) + pow(unitV.z,2));

        if (d == 0)
        {
            mat4 xRotNew = {{1,0,0,0},
                            {0,1,0,0},
                            {0,0,1,0},
                            {0,0,0,1}};
            xRot = xRotNew;
        }
        else
        {
            mat4 xRotNew = {{1, 0, 0, 0},
                            {0, unitV.z/d, unitV.y/d, 0},
                            {0, -unitV.y/d, unitV.z/d, 0},
                            {0, 0, 0, 1}};
            xRot = xRotNew;
        }

        mat4 yRot = {{d, 0, unitV.x, 0},
                     {0, 1, 0, 0},
                     {-unitV.x, 0, d, 0},
                     {0, 0, 0, 1}};

        mat4 zRot = {{cos(mag), sin(mag), 0, 0},
                    {-sin(mag), cos(mag), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};

        ctm = matMult(ctm, xRot);
        ctm = matMult(ctm, yRot);
        ctm = matMult(ctm, zRot);
        ctm = matMult(ctm, transpose(yRot));
        ctm = matMult(ctm, transpose(xRot));

        glx1 = glx2;
        gly1 = gly2;
        glz1 = glz2;
    }

    glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

int main(int argc, char **argv)
{
    int numVertRef=0, numTexRef=0;
    char line[20];
    FILE *file = fopen("city.obj", "r");

    //find num vertices
    char v1[10], v2[10], v3[10];
    while (fscanf(file, "%s", line) != EOF)
    {
        if (strcmp(line, "v") == 0) 
            numVertRef++;

        else if (strcmp(line, "vt") == 0)
            numTexRef++;

        else if (strcmp(line, "f") == 0) 
        {    
            if (fscanf(file, "%s %s %s", v1, v2, v3) != EOF)
            {
                num_vertices += 3; 
                while (fscanf(file, "%s", line) != EOF)
                    num_vertices += 3;
            }
        }
    }

    vec4 *vertRef = (vec4 *) malloc(sizeof(vec4) * numVertRef);
    vec2 *texRef = (vec2 *) malloc(sizeof(vec2) * numTexRef);
    fseek(file, 0, SEEK_SET);

    //store ref verts
    for (int i=1; i<numVertRef+1; i++)
    {
        if (fscanf(file, "%s %f %f %f", line, &vertRef[i].x, &vertRef[i].y, &vertRef[i].z) != EOF)
        {
            vertRef[i].w = 1.0;
            if (strcmp(line, "v") != 0) printf("ERROR at vertRef line: %d", i);
        }
    }
    
    //store ref texs
    for (int i=1; i<numTexRef+1; i++)
    {
        if (fscanf(file, "%s %f %f", line, &texRef[i].s, &texRef[i].t) != EOF)
        {
            texRef[i].t = 1 - texRef[i].t;
            if (strcmp(line, "vt") != 0) printf("ERROR at texRef line: %d", i);
        }
    }
    
    //populate verts/tex
    int vertsCounter = 0;
    int texCounter = 0;
    int EOFTracker, firstVec=0, firstTex=0, lastVec=0, lastTex=0;
    while (fscanf(file, "%s", line) != EOF)
    {
        if (strcmp(line, "f") == 0)
        {
                EOFTracker = fscanf(file, "%s", line);
                firstVec = atoi(strtok(line, "/"));
                firstTex = atoi(strtok(NULL, " "));
                vertices[vertsCounter++] = vertRef[firstVec];
                tex_coords[texCounter++] = texRef[firstTex];

                EOFTracker = fscanf(file, "%s", line);
                vertices[vertsCounter++] = vertRef[atoi(strtok(line, "/"))];
                tex_coords[texCounter++] = texRef[atoi(strtok(NULL, " "))];

                EOFTracker = fscanf(file, "%s", line);
                lastVec = atoi(strtok(line, "/"));
                lastTex = atoi(strtok(NULL, " "));
                vertices[vertsCounter++] = vertRef[lastVec];
                tex_coords[texCounter++] = texRef[lastTex];
        }
        else 
        {
            vertices[vertsCounter++] = vertRef[firstVec];
            tex_coords[texCounter++] = texRef[firstTex];
            vertices[vertsCounter++] = vertRef[lastVec];
            tex_coords[texCounter++] = texRef[lastTex];
            vertices[vertsCounter++] = vertRef[atoi(strtok(line, "/"))];
            tex_coords[texCounter++] = texRef[atoi(strtok(NULL, " "))];
        }
    }
    fclose(file);

    //scale city
    float minX, minY, maxZ;

    minX = vertices[0].x;
    minY = vertices[0].y;
    maxZ = vertices[0].z;

    for (int i=0; i<num_vertices; i++)
    {
        if (vertices[i].x < minX)
            minX = vertices[i].x;

        if (vertices[i].y < minY)
            minY = vertices[i].y;

        if (vertices[i].z > maxZ)
            maxZ = vertices[i].z;
    }

    for (int i=0; i<num_vertices; i++)
    {
        vertices[i].x = (vertices[i].x - minX)*100;
        vertices[i].y = (vertices[i].y - minY)*100;
        vertices[i].z = (vertices[i].z - maxZ)*100;
    }

    vec4 floor1 = {-1000.0, 0.0, -1000.0, 1.0};
    vec4 floor2 = {-1000.0, 0.0, 1000.0, 1.0};
    vec4 floor3 = {1000.0, 0.0, 1000.0, 1.0};
    vec4 floor4 = {1000.0, 0.0, -1000.0, 1.0};

    vertices[num_vertices] = floor1;
    colors[num_vertices++] = green;
    vertices[num_vertices] = floor2;
    colors[num_vertices++] = green;
    vertices[num_vertices] = floor3;
    colors[num_vertices++] = green;
    vertices[num_vertices] = floor1;
    colors[num_vertices++] = green;
    vertices[num_vertices] = floor3;
    colors[num_vertices++] = green;
    vertices[num_vertices] = floor4;
    colors[num_vertices++] = green;

    num_vertices+=3;    //for map marker triangle

    projection_matrix = matMult(projection_matrix, frustum(left, right, bottom, top, near, far));
    model_view = look_at(eye, at, up);

    printf("w: walk forward \nd: walk backward \na: move left \nd: move right\n");
    printf("i: look up \nk: look down \nj: turn left \nl: turn right\n");
    printf("m: map \ne: walk\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(1000,100);
    glutCreateWindow("Project 3");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
