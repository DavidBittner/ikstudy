#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "class_arm.h"

Coord mousePos;

void Reshape( GLFWwindow *wind, int width, int height )
{

    glfwMakeContextCurrent( wind );

    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, width, 0, height, 1.0f, 10.0f );
    glMatrixMode( GL_MODELVIEW );

}

void MouseMotionFunc( GLFWwindow *wind, double x, double y )
{

    glfwMakeContextCurrent( wind );

    int w, h;

    glfwGetWindowSize( wind, &w, &h );

    mousePos.x = x;
    mousePos.y = h-y;

}

void DrawMouse()
{

    glPointSize( 10 );

    glBegin( GL_POINTS );
        glVertex2f( mousePos.x, mousePos.y );
    glEnd();

}

int main()
{

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    glfwInit();
    GLFWwindow *window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Inverse Kinematics", nullptr, nullptr );
    glfwMakeContextCurrent( window );

    glfwSetWindowSizeCallback( window, Reshape );
    glfwSetCursorPosCallback( window, MouseMotionFunc );
    Reshape( window, WINDOW_WIDTH, WINDOW_HEIGHT );

    glfwSwapInterval( 1 );

    Arm newArm( Coord( 400, 300 ), 6, 100 );

    mousePos.x = 400;
    mousePos.y = 400;

    glLineWidth( 5 );

    while( !glfwWindowShouldClose( window ) )
    {

        glfwPollEvents();

        glClear( GL_COLOR_BUFFER_BIT );
        glLoadIdentity();

        glTranslatef( 0.0f, 0.0f, -1.0f );
        newArm.calcAngles( Coord( mousePos.x, mousePos.y ) );
        
        DrawMouse();
        newArm.draw(); 

        glfwSwapBuffers( window );

    }

    glfwTerminate();

}
