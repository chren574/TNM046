/*
 * A C++ framework for OpenGL programming in TNM046 for MT1 2014.
 *
 * This is a small, limited framework, designed to be easy to use
 * for students in an introductory computer graphics course in
 * the first year of a M Sc curriculum. It uses custom code
 * for some things that are better solved by external libraries
 * like GLEW and GLM, but the emphasis is on simplicity and
 * readability, not generality.
 * For the window management, GLFW 3.0 is used for convenience.
 * The framework should work in Windows, MacOS X and Linux.
 * Some Windows-specific stuff for extension loading is still
 * here. GLEW could have been used instead, but for clarity
 * and minimal dependence on other code, we rolled our own extension
 * loading for the things we need. That code is short-circuited on
 * platforms other than Windows. This code is dependent only on
 * the GLFW and OpenGL libraries. OpenGL 3.3 or higher is required.
 *
 * Author: Stefan Gustavson (stegu@itn.liu.se) 2013-2014
 * This code is in the public domain.
 */
#include "Shader.hpp"
#include "Utilities.hpp"

// File and console I/O for logging and error reporting
#include <iostream>
#include <cstdio>
#include <cmath>

// In MacOS X, tell GLFW to include the modern OpenGL headers.
// Windows does not want this, so we make this Mac-only.
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>

void createVertexBuffer(int location, int dimensions, const float *data, int datasize) {

	GLuint bufferID;

	// Generate buffer, activate it and copy the data
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, datasize, data, GL_STATIC_DRAW);
	// Tell OpenGL how the data is stored in our buffer
	// Attribute location (must match layout(location=#) statement in shader)
	// Number of dimensions (3 -> vec3 in the shader, 2-> vec2 in the shader),
	// type GL_FLOAT, not normalized, stride 0, start at element 0
	glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, NULL);
	// Enable the attribute in the currently bound VAO
	glEnableVertexAttribArray(location);
}

void createIndexBuffer(const unsigned int *data, int datasize) {

	GLuint bufferID;

	// Generate buffer, activate it and copy the data
	glGenBuffers(1, &bufferID);
    // Activate (bind) the index buffer and copy data to it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize, data, GL_STATIC_DRAW);
}
/*
 * main(argc, argv) - the standard C++ entry point for the program
 */
int main(int argc, char *argv[])
{

    using namespace std;

    const float pi = 3.14;

    int width, height;
    float time;

    Shader myShader;
    GLint location_time;

    // Vertex coordinates (x,y,z) for three vertices
    GLuint colorBufferID; // Vertex colors
    GLuint vertexArrayID, vertexBufferID, indexBufferID;

    GLfloat T[16];
    GLint location_T;

    GLfloat M[16];
    GLint location_M = -1;
//
//    const GLfloat vertex_array_data[] =
//    {
//        -1.0f, -1.0f, 0.0f,  // First vertex, xyz
//        1.0f, -1.0f, 0.0f,  // Second vertex, xyz
//        //0.0f,  1.0f, 0.0f,   // Third vertex, xyz
//        1.0f,  1.0f, 0.0f,   // forth vertex, xyz
//        -1.0f,  1.0f, 0.0f,   // forth vertex, xyz
//    };

    const GLfloat color_array_data[] =
    {
        1.0f, 0.0f, 0.0f,  // Red
        1.0f, 0.0f, 0.0f,  // Red
        1.0f, 0.0f, 0.0f,  // Red
        0.0f, 1.0f, 0.0f,  // Green
        0.0f, 1.0f, 0.0f,  // Green
        0.0f, 1.0f, 0.0f,  // Green
        0.0f, 0.0f, 1.0f,  // Blue
        0.0f, 0.0f, 1.0f,  // Blue
    };

//    const GLuint index_array_data[] =
//    {
//        0,1,2,
//        0,3,2,
//    };

    //Cube
    const GLfloat vertex_array_data_cube[] =
    {
        -1.0f, -1.0f, -1.0f,  // vertex V0
        -1.0f, 1.0f, -1.0f,  // vertex V1
        1.0f, 1.0f, -1.0f,  // vertex V2
        1.0f, -1.0f, -1.0f,  // vertex V3
        -1.0f, -1.0f, 1.0f,  // vertex V4
        -1.0f, 1.0f, 1.0f,  // vertex V5
        1.0f, 1.0f, 1.0f,  // vertex V6
        1.0f, -1.0f, 1.0f  // vertex V7
    };

    const GLuint index_array_data_cube[] =
    {

        0,5,1,  //negativ x
        0,4,5,  //negativ x

        2,6,7,  //positiv x
        2,7,3,  //positiv x

        0,3,7,  //negativ y
        0,7,4,  //negativ y

        1,5,6,  //positiv y
        1,6,2,  //positiv y

        0,1,2,  //negativ z
        0,2,3,  //negativ z

        4,7,6,  //positiv z
        4,6,5,  //positiv z
    };


    const GLFWvidmode *vidmode;  // GLFW struct to hold information about the display
    GLFWwindow *window;    // GLFW struct to hold information about the window

    // Initialise GLFW
    glfwInit();

    // Determine the desktop size
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Make sure we are getting a GL context of at least version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Exclude old legacy cruft from the context. We don't need it, and we don't want it.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a square window (aspect 1:1) to fill half the screen height
    window = glfwCreateWindow(vidmode->height/2, vidmode->height/2, "GLprimer", NULL, NULL);
    if (!window)
    {
        cout << "Unable to open window. Terminating." << endl;
        glfwTerminate(); // No window was opened, so we can't continue in any useful way
        return -1;
    }

    // Make the newly created window the "current context" for OpenGL
    // (This step is strictly required, or things will simply not work)
    glfwMakeContextCurrent(window);

    //Laddar in anvndbara funktioner
    Utilities::loadExtensions();

    myShader.createShader("vertex.glsl", "fragment.glsl");

    // Generate 1 Vertex array object, put the resulting identifier in vertexArrayID
    glGenVertexArrays(1, &vertexArrayID);
    // Activate the vertex array object
    glBindVertexArray(vertexArrayID);

    // Create the vertex buffer objects for attribute locations 0 and 1
    // (the list of vertex coordinates and the list of vertex colors).
    createVertexBuffer(0, 3, vertex_array_data_cube, sizeof(vertex_array_data_cube));   //Kub
    createVertexBuffer(1, 3, color_array_data, sizeof(color_array_data));
    //createVertexBuffer(0, 3, vertex_array_data, sizeof(vertex_array_data));             //Tiangel

    // Create the index buffer object (the list of triangles).
    //createIndexBuffer(index_array_data, sizeof(index_array_data));                      //Triangel
    createIndexBuffer(index_array_data_cube, sizeof(index_array_data_cube));          //Kub

    // Deactivate the vertex array object again to be nice
    glBindVertexArray(0);

    // Show some useful information on the GL context
    cout << "GL vendor:       " << glGetString(GL_VENDOR) << endl;
    cout << "GL renderer:     " << glGetString(GL_RENDERER) << endl;
    cout << "GL version:      " << glGetString(GL_VERSION) << endl;
    cout << "Desktop size:    " << vidmode->width << "x" << vidmode->height << " pixels" << endl;

    // Get window size. It may start out different from the requested
    // size, and will change if the user resizes the window.
    glfwGetWindowSize( window, &width, &height );

    // Set viewport. This is the pixel rectangle we want to draw into.
    glViewport(0, 0, width, height ); // The entire window

    glfwSwapInterval(0); // Do not wait for screen refresh between frames


    location_time = glGetUniformLocation(myShader.programID, "time");
    if(location_time != -1){
        cout << "Unable to locate variable 'time' in shader!" << endl;
    }

    location_T = glGetUniformLocation(myShader.programID, "T");
    location_M = glGetUniformLocation(myShader.programID, "M");

    //printf("M is %d\n", location_M);
    //Utilities::mat4mult(Utilities::mat4rotx(M,45.0),Utilities::mat4roty(M,pi/6),M);
    //Utilities::mat4identity(M);
    //Utilities::mat4scale(M, 2);

    Utilities::mat4identity(T);

/*
    printf("Matrix:\n");
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[0], M[4], M[8], M[12]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[1], M[5], M[9], M[13]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[2], M[6], M[10], M[14]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[3], M[7], M[11], M[15]);
    printf("\n");
*/
    glEnable(GL_CULL_FACE);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize( window, &width, &height );
        glViewport( 0, 0, width, height ); // The entire window

        Utilities::displayFPS(window);

        // Set the clear color and depth, and clear the buffers for drawing
        glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* ---- Rendering code should go here ---- */
        time = (float)glfwGetTime();
        glUseProgram(myShader.programID);
        glUniform1f(location_time, time);

        /* ---- Transformationer ---- */
        Utilities::mat4identity(M);

        Utilities::mat4scale(T, 0.1);
        Utilities::mat4mult(T,M,M);

        Utilities::mat4roty(T,time*pi/6);
        Utilities::mat4mult(T,M,M);

        Utilities::mat4translate(T, 0.3, 0, 0);
        Utilities::mat4mult(T,M,M);

        Utilities::mat4roty(T,time*pi/6);
        Utilities::mat4mult(T,M,M);

        Utilities::mat4rotx(T,pi/4);
        Utilities::mat4mult(T,M,M);

        glUniformMatrix4fv(location_M, 1, GL_FALSE, M);

        // Activate the vertex array object we want to draw (we may have several)
        glBindVertexArray(vertexArrayID);

        // Draw our triangle with 3 vertices.
        // When the last argument of glDrawElements is NULL, it means
        // "use the previously bound index buffer". (This is not obvious.)
        // The index buffer is part of the VAO state and is bound with it.
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

        // Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers(window);

        // Poll events (read keyboard and mouse input)
        glfwPollEvents();

        // Exit if the ESC key is pressed (and also if the window is closed).
        if(glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

    }

    // Close the OpenGL window and terminate GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}



