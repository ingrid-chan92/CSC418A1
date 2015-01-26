/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1

    Instructions:
        See main.cpp for more instructions.

        This file contains the class OpenGL portion of the main window.
**************************************************************************/

#include "GLWidget.h"
#include <iostream>

static QGLFormat createQGLFormat()
{
    // Return a QGLFormat object that tells Qt information about what
    // OpenGL context we would like.
    QGLFormat format(QGL::SampleBuffers);
    if (supportsNewOpenGL())
    {
	// Choose a version of OpenGL that supports vertex array objects and
	// tell it that we do not want support for deprecated functions.
        format.setVersion(3, 3);
        format.setProfile(QGLFormat::CoreProfile);
    }
    return format;
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(createQGLFormat(), parent)
    , m_is_animating(false)
    , m_animation_frame(0)
    , m_joint_angle(0)
{
    // Start a timer that will call the timerEvent method every 50ms.
    startTimer(/*milliseconds=*/50);
}

void GLWidget::initializeGL()
{
    m_gl_state.initializeGL();

    // To aid with troubleshooting, print out which version of OpenGL we've
    // told the driver to use.
    std::cout << "Using OpenGL: " << glGetString(GL_VERSION) << std::endl;

    // Copy the data for the shapes we'll draw into the video card's memory.
    m_unit_square.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    m_unit_circle.initialize(
	m_gl_state.VERTEX_POSITION_SHADER_LOCATION,
	/*num_circle_segments=*/100);
	
	// INITIALIZE PENGUIN SHAPES
	m_penguin_head.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
	m_penguin_body.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
	m_penguin_wing.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
	m_penguin_beak.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);

    // Tell OpenGL what color to fill the background to when clearing.
    glClearColor(/*red=*/0.7f, /*green=*/0.7f, /*blue=*/0.9f, /*alpha=*/1.0f);

}

void GLWidget::resizeGL(int width, int height)
{
    // Respond to the window being resized by updating the viewport and
    // projection matrices.

    checkForGLErrors();

    // Setup projection matrix for new window
    m_gl_state.setOrthographicProjectionFromWidthAndHeight(width, height);

    // Update OpenGL viewport and internal variables
    glViewport(0, 0, width, height);
    checkForGLErrors();
}

void GLWidget::timerEvent(QTimerEvent *)
{
    // Respond to a timer going off telling us to update the animation.
    if (!m_is_animating)
        return;

    // increment the frame number.
    m_animation_frame++;

    // Update joint angles.
    const double joint_rot_speed = 0.1;
    double joint_rot_t =
        (sin(m_animation_frame * joint_rot_speed) + 1.0) / 2.0;
    m_joint_angle = joint_rot_t * JOINT_MIN + (1 - joint_rot_t) * JOINT_MAX;

    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function to animate the character's joints
    //   Note: Nothing should be drawn in this function!
    //////////////////////////////////////////////////////////////////////////

    // Tell this widget to redraw itself.
    update();
}

void GLWidget::paintGL()
{
    // This method gets called by the event handler to draw the scene, so
    // this is where you need to build your scene -- make your changes and
    // additions here.  All rendering happens in this function.

    checkForGLErrors();

    // Clear the screen with the background colour.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

    // Setup the model-view transformation matrix stack.
    transformStack().loadIdentity();
    checkForGLErrors();

    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function draw the scene.  This should include function
    //   calls to pieces that apply the appropriate transformation matrices
    //   and render the individual body parts.
    //////////////////////////////////////////////////////////////////////////


    // Note that successive transformations are applied *before* the previous
    // ones.

    // Push the current transformation matrix on the stack
    transformStack().pushMatrix();

		// Penguin body
		transformStack().pushMatrix();
			transformStack().scale(100.0f, 100.0f);
			m_gl_state.setColor(0.0, 1.0, 1.0);
			m_penguin_body.draw();
		transformStack().popMatrix();

		// Penguin head
		transformStack().pushMatrix();
			transformStack().scale(125.0f, 125.0f);
			transformStack().translate(0.0, 1.0f);
			m_gl_state.setColor(0.0, 0.0, 1.0);

			// Rotate along the hinge
        	transformStack().rotateInDegrees(m_joint_angle);

			m_penguin_head.draw();
		transformStack().popMatrix();

		// Penguin wing
		transformStack().pushMatrix();
			transformStack().scale(75.0f, 100.0f);			

			// Rotate along the hinge
			transformStack().translate(0.0, 0.5);
        	transformStack().rotateInDegrees(m_joint_angle);
			transformStack().translate(0.0, -0.5);

			m_gl_state.setColor(0.0, 0.0, 1.0);
			m_penguin_wing.draw();
		transformStack().popMatrix();

		// Penguin beak
		transformStack().pushMatrix();	
			transformStack().scale(100.0f, 75.0f);		
			transformStack().translate(-1.0, 2.0);
			m_gl_state.setColor(1.0, 0.0, 1.0);
			m_penguin_beak.draw();
		transformStack().popMatrix();

		// Penguin jaw
		transformStack().pushMatrix();	
			transformStack().scale(100.0, 10.0);			
			transformStack().translate(-1.0, 12.0);            		
            m_gl_state.setColor(1.0, 0.0, 1.0);
	    	m_unit_square.draw();
        transformStack().popMatrix();

		// Penguin leg1
		transformStack().pushMatrix();
			transformStack().translate(-3.5, -3.5);
			transformStack().rotateInDegrees(m_joint_angle);            

			transformStack().scale(25.0, 100.0);       	
			transformStack().translate(0.0, -0.5);
            m_gl_state.setColor(1.0, 0.0, 1.0);

	    	m_unit_square.draw();
        transformStack().popMatrix();

		// Penguin leg2
		transformStack().pushMatrix();
        	transformStack().rotateInDegrees(m_joint_angle);

            transformStack().scale(25.0, 100.0);
			transformStack().translate(1.25, -1.25);
            m_gl_state.setColor(1.0, 0.0, 1.0);

	    	m_unit_square.draw();
        transformStack().popMatrix();

		// Penguin foot1
		transformStack().pushMatrix();
        	transformStack().rotateInDegrees(m_joint_angle);

			transformStack().scale(100.0, 25.0);
			transformStack().translate(-0.75, -6.5);
            m_gl_state.setColor(0.0, 0.0, 1.0);

	    	m_unit_square.draw();
        transformStack().popMatrix();

		// Penguin foot2
		transformStack().pushMatrix();
        	transformStack().rotateInDegrees(m_joint_angle);
	
			transformStack().scale(100.0, 25.0);
			transformStack().translate(0.75, -6.5);
			m_gl_state.setColor(0.0, 0.0, 1.0);

	    	m_unit_square.draw();
        transformStack().popMatrix();


/*
		// Draw our hinged object
		const float BODY_WIDTH = 60.0f;
		const float BODY_LENGTH = 100.0f;
		const float ARM_LENGTH = 100.0f;
		const float ARM_WIDTH = 20.0f;
	

        // Draw the 'body'
        transformStack().pushMatrix();
            // Scale square to size of body
            transformStack().scale(BODY_WIDTH, BODY_LENGTH);

            // Set the colour to green
            m_gl_state.setColor(0.0, 1.0, 0.0);

            // Draw the square for the body
	    m_unit_square.draw();
        transformStack().popMatrix();

        // Draw the 'arm'

        // Move the arm to the joint hinge
        transformStack().translate(0.0, -BODY_LENGTH/2 + ARM_WIDTH);

        // Rotate along the hinge
        transformStack().rotateInDegrees(m_joint_angle);

        // Scale the size of the arm
        transformStack().scale(ARM_WIDTH, ARM_LENGTH);

        // Move to center location of arm, under previous rotation
        transformStack().translate(0.0, -0.5);

        // Draw the square for the arm
        m_gl_state.setColor(1.0, 0.0, 0.0);
	m_unit_square.draw();
*/



    // Retrieve the previous state of the transformation stack
    transformStack().popMatrix();


    // Execute any GL functions that are in the queue just to be safe
    glFlush();
    checkForGLErrors();
}

