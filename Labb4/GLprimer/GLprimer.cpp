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
#include "TriangleSoup.hpp"

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
    TriangleSoup myShape;

    GLint location_time;

    GLfloat MV[16];
    GLint location_MV;

    GLfloat P[16];
    GLint location_P;

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
    myShape.createBox(0.5,0.2,0.2);

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
    Utilities::mat4identity(MV);
    location_MV = glGetUniformLocation(myShader.programID, "MV");

    Utilities::mat4identity(P);
    location_P = glGetUniformLocation(myShader.programID, "P");


    glEnable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* ---- Main loop ---- */
    while(!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize( window, &width, &height );
        glViewport( 0, 0, width, height ); // The entire window

        Utilities::displayFPS(window);

        // Set the clear color and depth, and clear the buffers for drawing
        glClearColor(0.0f, 0.9f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* ---- Rendering code should go here ---- */
        time = (float)glfwGetTime();
        glUseProgram(myShader.programID);
        glUniform1f(location_time, time);

        glUniformMatrix4fv(location_P, 1, GL_FALSE, P);
        glUniformMatrix4fv(location_MV, 1, GL_FALSE, MV);

        /* ---- Transformationer ---- */

        myShape.render();

        Utilities::mat4identity(MV);

        Utilities::mat4roty(P, time*pi/6);
        Utilities::mat4mult(P, MV, MV);

        Utilities::mat4translate(P,0.0, -0.5, -1.0);
        Utilities::mat4mult(P, MV, MV);

        Utilities::mat4rotx(P, pi/6);
        Utilities::mat4mult(P, MV, MV);



        Utilities::mat4perspective(P, pi/3, 1.0, 0.1, 100.0);
        Utilities::mat4mult(P, MV, MV);



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



