/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1

    Instructions:
        See main.cpp for more instructions.

        This file contains the OpenGL portion of the main window.
**************************************************************************/

#ifndef __GLWidget_h__
#define __GLWidget_h__

#include "include_gl.h"
#include "GLState.h"
#include "GLShape.h"
#include <QtOpenGL/QtOpenGL>

// Before transformed, this class displays a unit square centered at the
// origin.
class UnitSquare : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
	// Use two triangles to create the square.
        GLfloat square_vertices[][2] =
        {
            { -0.5, -0.5 },  // Triangle 1
            {  0.5, -0.5 },
            {  0.5,  0.5 },
            { -0.5, -0.5 },  // Triangle 2
            {  0.5,  0.5 },
            { -0.5,  0.5 },
        };

        initialize(
	    shader_input_location,
            reinterpret_cast<const GLfloat *>(square_vertices),
            /*num_vertices=*/6,
	    GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

// Before transformed, this class displays a penguin head centered at the
// origin.
class PenguinHead : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
	// Use two triangles to create the square and three triangles for the sides
        GLfloat vertices[][2] =
        {
            { -0.5, -0.25 },  // Square: Triangle 1
            {  0.5, -0.25 },
            {  0.5,  0.5 },
            { -0.5, -0.25 },  // Square: Triangle 2
            {  0.5,  0.5 },
            { -0.5,  0.5 },
			{ -0.5, -0.25 },  // Left: Triangle 3
            { -0.5,  0.5 },
            { -0.75, -0.25 },
			{  0.5,  0.5 },  // Right: Triangle 4
            {  0.5, -0.25 },
            {  0.75, -0.25 },
			{  0.5,  0.5 },  // Top: Triangle 5
            { -0.5,  0.5 },
            { -0.25, 0.75 },
        };

        initialize(
	    shader_input_location,
            reinterpret_cast<const GLfloat *>(vertices),
            /*num_vertices=*/15,
	    GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

// Before transformed, this class displays a penguin body centered at the
// origin.
class PenguinBody : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
	// Use two triangles to create the square and three triangles for the sides
        GLfloat vertices[][2] =
        {
            { -0.75, 1.0 },  // Square: Triangle 1
            { -0.75, -1.0 },
            {  0.75,  1.0 },
            {  0.75,  1.0 },  // Square: Triangle 2
            { -0.75, -1.0 },
            {  0.75, -1.0 },

			{ -0.75, 1.0 },  // Left: Triangle 3
            { -0.75, -1.0 },
            { -1.0, -0.5 },
			{  0.75,  1.0 },  // Right: Triangle 4
            {  0.75, -1.0 },
            {  1.0, -0.5 },

        };

        initialize(
	    shader_input_location,
            reinterpret_cast<const GLfloat *>(vertices),
            /*num_vertices=*/12,
	    GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

// Before transformed, this class displays a penguin wing centered at the
// origin.
class PenguinWing : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
	// Use two triangles to create the square and three triangles for the sides
        GLfloat vertices[][2] =
        {
            { -0.5, 0.5 },  // Triangle 1
            {  0.5, 0.5 },
            { -0.25, -0.5 },
            {  0.5, 0.5 },  // Triangle 2
            { -0.25, -0.5 },
			{  0.25, -0.5 }, 
        };

        initialize(
	    shader_input_location,
            reinterpret_cast<const GLfloat *>(vertices),
            /*num_vertices=*/6,
	    GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

// Before transformed, this class displays a penguin beak centered at the
// origin.
class PenguinBeak : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
	// Use two triangles to create the square and three triangles for the sides
        GLfloat vertices[][2] =
        {
            { -0.5, -0.25 },  // Triangle 1
            {  0.5, -0.25 },
            { -0.5, 0 },
            {  0.5, -0.25 }, // Triangle 2
            { -0.5, 0 },
			{  0.5, 0.5 }, 
        };

        initialize(
	    shader_input_location,
            reinterpret_cast<const GLfloat *>(vertices),
            /*num_vertices=*/6,
	    GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

// Before transformed, this class displays a unit circle centered at the
// origin.
class UnitCircle : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location, int num_circle_segments)
    {
        // We will draw a circle as a triangle fan.  We are careful to send
	// the second coordinate twice to properly close the circle.
        //        3     2     1
        //         +----+----+
        //        / \   |   /
        //       /   \  |  /
        //      /     \ | /
        //     /       \|/
        //   4+---------+ 0
        //        ...
        std::vector<GLfloat> circle_vertices;
        circle_vertices.push_back(0.0);
        circle_vertices.push_back(0.0);
        for (int i=0; i<=num_circle_segments; ++i)
        {
            double angle = (2 * M_PI * i) / num_circle_segments;
            circle_vertices.push_back(cos(angle));
            circle_vertices.push_back(sin(angle));
        }

        initialize(
	    shader_input_location,
            &circle_vertices[0],
            num_circle_segments + 1,
	    GL_TRIANGLE_FAN);
    }
};

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    // These values control the bounds to display on the joint angle sliders.
    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Add different ranges for the different joints.  Use these ranges
    //   when constructing sliders and when animating joints.
    //////////////////////////////////////////////////////////////////////////
    static const int HEAD_MIN = -45;
    static const int HEAD_MAX = 45;
    static const int WING_MIN = -60;
    static const int WING_MAX = 60;
    static const int LEG_MIN = -45;
    static const int LEG_MAX = 45;
    static const int FOOT_MIN = -45;
    static const int FOOT_MAX = 45;
	static const int JAW_MIN = -2;
	static const int JAW_MAX = 2;

	static const int HEIGHT_MIN = -100;
	static const int HEIGHT_MAX = 100;
	static const int WIDTH_MIN = -200;
	static const int WIDTH_MAX = 200;

    GLWidget(QWidget *parent=NULL);

public slots:
    // This method is called when the user changes the joint angle slider.

	void setHeadAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        head_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

	void setWingAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        wing_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

	void setLeg1Angle(int angle)
    {
        // This method is called when the user changes the slider value.
        leg1_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

	void setLeg2Angle(int angle)
    {
        // This method is called when the user changes the slider value.
        leg2_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

	void setFoot1Angle(int angle)
    {
        // This method is called when the user changes the slider value.
        foot1_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

	void setFoot2Angle(int angle)
    {
        // This method is called when the user changes the slider value.
        foot2_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

	void setJawHeight(int height)
    {
        // This method is called when the user changes the slider value.
        jaw_height = height;

        // Call update() to trigger a redraw.
        update();
    }

	void setHeight(int h)
    {
        // This method is called when the user changes the slider value.
        height = h;

        // Call update() to trigger a redraw.
        update();
    }

	void setWidth(int w)
    {
        // This method is called when the user changes the slider value.
        width = w;

        // Call update() to trigger a redraw.
        update();
    }

    void onPressAnimate(int is_animating)
    {
        // This method is called when the user changes the animation checkbox.
        m_is_animating = (bool)is_animating;
        m_animation_frame = 0;
        update();
    }

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void timerEvent(QTimerEvent *event);

private:
    GLTransformStack &transformStack()
    { return m_gl_state.transformStack(); }

private:
    GLState m_gl_state;
    bool m_is_animating;
    int m_animation_frame;
    UnitSquare m_unit_square;
    UnitCircle m_unit_circle;
	
    //////////////////////////////////////////////////////////////////////////
    // TODO: Add additional joint parameters.
    //////////////////////////////////////////////////////////////////////////
	PenguinHead m_penguin_head;
	PenguinBody m_penguin_body;
	PenguinWing m_penguin_wing;
	PenguinBeak m_penguin_beak;

	double head_angle;
	double wing_angle;
	double leg1_angle;
	double leg2_angle;
	double foot1_angle;
	double foot2_angle;
	double jaw_height;
	double height;
	double width;

};

#endif
