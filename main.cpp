
#include "main.h"
#ifdef WIN32
#define ssize_t SSIZE_T
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cmath>
#include <sstream>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <GLUT/glut.h>
#include "SimpleImage.h"

// file locations
std::string vertexShader;
std::string fragmentShader;

SimpleShaderProgram *shader;

using namespace std;


using namespace std;

struct Vertex {
    float x, y, z;
    
    Vertex(float x_, float y_, float z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    
    void operator=(const Vertex& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
};

struct textureCoordinate {
    float x, y;
    
    textureCoordinate(float x_, float y_) {
        x = x_;
        y = y_;
    }
};

struct face {
    int v1, v2, v3, tc1, tc2, tc3, vn1, vn2, vn3;
    
    //just vertices face
    face(int v1_, int v2_, int v3_) {
        v1 = v1_;
        v2 = v2_;
        v3 = v3_;
    }
    
    //vertices, texture coordinates, and vertex normal face
    face(int v1_, int v2_, int v3_, int tc1_, int tc2_, int tc3_,
       int vn1_,int vn2_, int vn3_) {
        v1 = v1_;
        v2 = v2_;
        v3 = v3_;
        
        tc1 = tc1_;
        tc2 = tc2_;
        tc3 = tc3_;
        
        vn1 = vn1_;
        vn2 = vn2_;
        vn3 = vn3_;
    }
};

//create static data vectors
static vector<Vertex> vertexData; //vertices
static vector<textureCoordinate> textureData; //texture coordindates
static vector<Vertex> vertexNormalData; //vertex normals
static vector<face> faceData; //faces


//create static orbit variables
static int oldX, oldY;
static float theta =0.0f, phi=0.0f;

Vertex& findNormal(int v1, int v2, int v3) {
    //v2-v1 x v3-v1
    float a,b,c;
    
    //first expression
    a = vertexData[v2].x - vertexData[v1].x;
    b = vertexData[v2].y - vertexData[v1].y;
    c = vertexData[v2].z - vertexData[v1].z;
    Vertex e1(a, b, c);
    
    //second expression
    float d,e,f;
    d = vertexData[v3].x - vertexData[v1].x;
    e = vertexData[v3].y - vertexData[v1].y;
    f = vertexData[v3].z - vertexData[v1].z;
    Vertex e2(d, e, f);
    
    //calculate cross product
    float x, y, z;
    x = e1.y*e2.z - e1.z*e2.y;
    y = e1.z*e2.x - e1.x*e2.z;
    z = e1.x*e2.y - e1.y*e2.x;
    
    float length = sqrtf(x*x + y*y + z*z);
    Vertex result(x/length, y/length, z/length);
    
    return result;
}

void DrawWithShader() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glEnable(GL_DEPTH_TEST);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //discards inivisible polygons during rendering
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //set light source
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    //light position looks down z axis at z=-0.5
    GLfloat light_ambient[] = { 0.0, 0.0, -.5, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    //set material property
    GLfloat red[] = {1.f, 0.f, 0.f, 1.f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, red);
    
    //set front faces to counterclockwise orientation i.e. front facing
    glFrontFace(GL_CCW);

    shader->Bind();
    
    //points must have z coordinates within the interval [-1 - -5] to be visible
    glBegin(GL_TRIANGLES);
    
    //render faces
    for (int i = 0; i < faceData.size(); ++i) {

        face face = faceData[i];
        
        //vertices
        int v1 = face.v1-1;
        float x1, y1, z1;
        x1 = vertexData[v1].x;
        y1 = vertexData[v1].y;
        z1 = vertexData[v1].z;
        
        int v2 = face.v2-1;
        float x2, y2, z2;
        x2 = vertexData[v2].x;
        y2 = vertexData[v2].y;
        z2 = vertexData[v2].z;
        
        int v3 = face.v3-1;
        float x3, y3, z3;
        x3 = vertexData[v3].x;
        y3 = vertexData[v3].y;
        z3 = vertexData[v3].z;
        
        //render first vertex
        //plot vertex
        glVertex3f(x1, y1, z1-2.0);
        
        //render second vertex
        
        //plot vertex
        glVertex3f(x2, y2, z2-2.0);
        
        //render third vertex
        
        //plot vertex
        glVertex3f(x3, y3, z3-2.0);
    }
    glEnd();
    
    glFlush(); //forces execution, clears buffer

    shader->UnBind();
}

void DisplayCallback(){
    DrawWithShader();
    glutSwapBuffers();
}

void ReshapeCallback(int w, int h){
    glViewport(0, 0, w, h);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(30.0f, (float)w/(float)h, 0.1f, 100000.f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void KeyCallback(unsigned char key, int x, int y)
{
    switch (key) {
            //i is zoom in
        case 'i':
        glTranslated(0.0, 0.0, 0.1);
        break;

            //o is zoom out
        case 'o':
        glTranslated(0.0, 0.0, -.1);
        break;

            //W, A, Z, S make arrow pad
        case 'w':
        glTranslated(0.0, 1.0, 0.0);
        break;

        case 'a':
        glTranslated(-1.0, 0.0, 0.0);
        break;

        case 'z':
        glTranslated(0.0, -1.0, 0.0);
        break;

        case 's':
        glTranslated(1.0, 0.0, 0.0);
        break;

            //r for rotate right
        case 'r':
        glRotated(5.0, 0.0, 0.0, 10.0);
        break;

            //l for rotate left
        case 'l':
        glRotated(-5.0, 0.0, 0.0, 10.0);
        break;
    }
    glutPostRedisplay();
}

void orbit(int button, int state, int x, int y) {

    theta += (x-oldX)*0.0001f;
    phi   += (y-oldY)*0.0001f;
    
    oldX = x;
    oldY = y;
    
    float eyeX = cos(phi)*sin(theta);
    float eyeY = sin(phi)*sin(theta);
    float eyeZ = -1 + cos(theta);
    
    gluLookAt(eyeX, eyeY, eyeZ, 0, 0, -1, 0,1,0);
    
    glutPostRedisplay();
}

void Setup(const char* fileName)
{
    //these commands ensure that drawing commands affect the projection matrix, rather than the model view matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //sets the near plane at 1 and a far plane at 5
    //parameters (left, right, bottom, top, near, far)
    glFrustum(-1.0,1.0,-1.0,1.0,1.0,50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //sets the camera center at (0, 0, 0), looking down at (0, 0, -1) --> negative z-axis
    gluLookAt(0,0,0,0,0,-1,0,1,0);
    
    shader = new SimpleShaderProgram();
    shader->LoadVertexShader(vertexShader);
    shader->LoadFragmentShader(fragmentShader);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    //load data
    ifstream input;
    input.open(fileName);
    
    if(!input) {
        cout << fileName << " cannot be opened" << endl;
        exit(0);
    }
    
    string line;
    while(getline(input, line)) {
        //if this is a vertex line
        if (line.substr(0, 2) == "v ") {
            istringstream is(line.substr(2));
            
            //parse 3 vertices, push onto vertexData vector
            float a, b, c;
            
            is >> a;
            is >> b;
            is >> c;
            
            Vertex v(a, b, c);
            
            //instead of adding 4th w vertex, set conditions to include w = 1.0f after every 3 vertices
            vertexData.push_back(v);
        }
        //else if this is a texture coordinate line
        else if(line.substr(0,3) == "vt ") {
            istringstream is(line.substr(3));
            
            //parse coordinates and push on textureData vector
            float a, b;
            
            is >> a;
            is >> b;
            
            textureCoordinate tC(a,b);
            
            textureData.push_back(tC);
        }
        //else if this is a vertex normal line
        else if(line.substr(0,3) == "vn ") {
            istringstream is(line.substr(2));
            
            //parse 3 vertices, push onto vertexData vector
            float a, b, c;
            
            is >> a;
            is >> b;
            is >> c;
            
            Vertex v(a, b, c);
            
            //instead of adding 4th w vertex, set conditions to include w = 1.0f after every 3 vertices
            vertexNormalData.push_back(v);
        }
        //else if this is a face information line
        else if (line.substr(0, 2) == "f ") {
            istringstream is(line.substr(2));
            
            //parse face info and push on faceData vector
            int v1, v2, v3, tc1, tc2, tc3, vn1, vn2, vn3;
            char c1, d1, c2, d2, c3, d3;
            
            is >> v1;
            
            //check if next character is /
            if (is.peek() == '/') {
                //FIRST VERTEX INFO
                //discard /
                is >> c1;
                
                //store texture coordinate
                is >> tc1;
                
                //discard /
                is >> d1;
                
                //store vertex normal
                is >> vn1;
                
                //SECOND VERTEX INFO
                //store second vertex
                is >> v2;
                
                //discard /
                is >> c2;
                
                //store texture coordinate
                is >> tc2;
                
                //discard /
                is >> d2;
                
                //store vertex normal
                is >> vn2;
                
                //THIRD VERTEX INFO
                //store third vertex
                is >> v3;
                
                //discard /
                is >> c3;
                
                //store texture coordinate
                is >> tc3;
                
                //discard /
                is >> d3;
                
                //store vertex normal
                is >> vn3;
                
                face face(v1, v2, v3, tc1, tc2, tc3, vn1, vn2, vn3);
                faceData.push_back(face);
                
            }
            else {
                is >> v2;
                is >> v3;
                face face(v1, v2, v3);
                faceData.push_back(face);
            }
        }
        //ignore all other lines including comments and vertex normal data
        else { }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -10.f);
}

int main(int argc, char** argv){
    if(!(argc == 4)){
        printf("usage: ./hw5 <obj file> <vertex shader> <fragment shader> \n");
        return 0;
    }
    
    vertexShader   = std::string(argv[2]);
    fragmentShader = std::string(argv[3]);
    
    // Initialize GLUT.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(640, 480);
    const char* title = argv[1];
    glutCreateWindow(title);
    
    //
    // Initialize GLEW.
    //
#if !defined(__APPLE__) && !defined(__linux__)
    glewInit();
    if(!GLEW_VERSION_2_0) {
        printf("Your graphics card or graphics driver does\n"
         "\tnot support OpenGL 2.0, trying ARB extensions\n");
        
        if(!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
            printf("ARB extensions don't work either.\n");
            printf("\tYou can try updating your graphics drivers.\n"
             "\tIf that does not work, you will have to find\n");
            printf("\ta machine with a newer graphics card.\n");
            exit(1);
        }
    }
#endif
    
    Setup(title);
    
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyCallback);
    glutMouseFunc(orbit);
    glutIdleFunc(DisplayCallback);
    
    glutMainLoop();
    return 0;
}


// phong.vert before uniform data
// /*3 component vector for x, y, z coordinates of vertex position and normal
//  * attribute denotes parameters passed to this shader which will be provided PER vertex i.e. many times during a rendering call
//  */
// attribute vec3 position;
// attribute vec3 normal;

// /*4x4 matrices for projection, modelview, and normal
//  * uniform denotes parameters that are passed to this shader program. these will not change from one execution of this shader to the next within the same rendering call
//  */
// //uniform mat4 projection, modelview, normalMatrix;
// //THESE SHOULD BE gl_ProjectionMatrix, etc.

// /* global variables to be defined and passed to frag shader
// * varying denotes parameters that provide an interface between vert and frag shader i.e. vert shader passes these variables to frag shader */
// varying vec3 color;

// /* define vertex position and the interpolated normal */
// vec3 vertexPosition;
// vec3 normalInterpolated;

// /* define light position and 3 types of color for Phong model */
// vec3 lightPosition = vec3(0.0, 0.0, -.5);
// vec3 ambientColor = vec3(0.1, 0.0, 0.0); //light red for ambient color
// vec3 diffuseColor = vec3(1.0, 0.0, 0.0); //red color for diffuse color
// vec3 specularColor = vec3(1.0, 1.0, 1.0); //white color for specular reflection

// //set the shineness of our scene
// float shine = 16.0;

// void main()
// {
//     // homogenize vertex position using vec4 constructor with w=1.0
//     vec4 inputPositionH = vec4(position, 1.0);
    
//     //position vector with model view -- homogenized
//     vec4 vertexPositionH = modelview*inputPositionH;
    
//     // calculate position with projection, modelview and homogenized coordinate for vector
//     gl_Position = projection*modelview*inputPositionH;
    
//     /*calculate interpolated normal vector
//     * dehomogenize the vector from construction vec3 from the normal matrix with a w=0.0 value
//     */
//     vec4 normalDH = vec4(normal, 0.0);
//     normalInterpolated = vec3(normalMatrix*normalDH);

//         //let's normalize our interpolated normal vector first using OpenGL's normalize method
//     vec3 normal = normalize(normalInterpolated);
    
//     /* we can calculate the direction of the light by normalizing the difference between the light
//      * position and the vertex position
//      */
//     vec3 lightDirection = normalize(lightPosition - vertexPosition);
    
//     /* next we need to calculate the lambertian reflectance:
//      * this number defines the diffusely reflecting surface. 
//      * Note that the apparent brightness is the same regardless
//      * of the observer's angle of view. We can calculate this by
//      * taking the dot product of the of the lightDirection and
//      * the normal vector. Further, we must clamp this dot product
//      * because they may be negative, which we can do with the max() method
//      */
//     float lambertian = dot(lightDirection, normal); //calculate the lambertian reflectance
//     lambertian = max(lambertian, 0.0); //this clamps the value if it is negative
    
//     //set the default specular value to 0
//     float specular = 0.0;
    
//     /* a lambertian value greater than 0 implies that this fragment has some
//      * reflection from our light source
//      */
//     if (lambertian > 0.0) {
//         //calculate V, the normalized direction from the point on the surface towards the viewer
//         vec3 viewingDirection = normalize(-vertexPosition);
        
//             /* first calculate R, the normalized direction to which a light ray
//              * from the light source to this surface point would reflect. We can
//              * use the reflect() method with both L (light direction) and N (normal vector)
//              * to find R (reflection)
//              */
//             vec3 R = reflect(-lightDirection, normal);
            
//             /* now we need to compute the specular angle. The dot product of R (reflection) and the
//              * viewing direction. Again, this dot product must be campled because it may be negative
//              */
//             float specularAngle = dot(R, viewingDirection); //calculate specular angle
//             specularAngle = max(specularAngle, 0.0); //this clamps the value if it is negative
            
//             /* we can now use the specularAngle and our preset shine float to calculate specular
//              * in the equation this is (R*V)^shine
//              */
//             specular = pow(specularAngle, shine/4.0); //could divide shine by 4 ***********
//         }
    
//     // now we can determine the color using all of our calculated values for Phong modeling
//     color = ambientColor + (lambertian*diffuseColor) + (specular*specularColor);
// }