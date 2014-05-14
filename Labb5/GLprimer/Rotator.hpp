/* Rotator.hpp */
/* Two classes to perform viewport rotations on mouse and keyboard input with GLFW . */
/* Usage: call init() before the rendering loop, call poll() once per frame,
 * read public members phi and theta to construct a rotation matrix.
 * The suggested composite rotation matrix is RotX(theta)*RotY(phi). */
/* Stefan Gustavson (stefan.gustavson@liu.se) 2014-03-27 */

#ifndef ROTATOR_HPP // Avoid including this header twice
#define ROTATOR_HPP

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif

#include <GLFW/glfw3.h>
#include <cmath>

const float M_PI = 3.14;

class KeyRotator {

public:
	float phi;
	float theta;

private:
	double lastTime;

public:
    void init(GLFWwindow *window);
    void poll(GLFWwindow *window);
};

class MouseRotator {

public:
	float phi;
	float theta;

private:
	double lastX;
	double lastY;
	int lastLeft;
	int lastRight;

public:
    void init(GLFWwindow *window);
    void poll(GLFWwindow *window);
};

#endif // ROTATOR_HPP
