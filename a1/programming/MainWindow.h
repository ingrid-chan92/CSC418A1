/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1
  
  
    Instructions:
        See main.cpp for more instructions.

        This file contains the class for the main window of the program.
**************************************************************************/

#ifndef __MainWindow_h__
#define __MainWindow_h__

#include <QtGui/QtGui>
#include "GLWidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow()
    {
        // Create a GLWidget to hold the OpenGL viewport.
        m_gl_widget = new GLWidget();

        // Create a checkbox to turn animation on and off, and make it
        // call GLWidget::onPressAnimate when checked.
        m_animate_checkbox = new QCheckBox("Animate", this);
        connect(
            m_animate_checkbox, SIGNAL(stateChanged(int)),
            m_gl_widget, SLOT(onPressAnimate(int)));

        // Create a button to quit the program.
        m_quit_button = new QPushButton("Quit", this);
        connect(
            m_quit_button, SIGNAL(clicked(bool)),
            this, SLOT(onPressQuit(bool)));

        m_main_layout = new QVBoxLayout();
        m_main_layout->addWidget(m_gl_widget);

        // Create a slider to control the joint angle, and make it call
        // GLWidget::setJointAngle when the slider value changes.

        //////////////////////////////////////////////////////
        // TODO: Add additional joint sliders here
        //////////////////////////////////////////////////////

		head_slider = create_joint_angle_slider(
	    "Head", GLWidget::HEAD_MIN, GLWidget::HEAD_MAX);
        connect(
            head_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setHeadAngle(int)));

		wing_slider = create_joint_angle_slider(
	    "Wing", GLWidget::WING_MIN, GLWidget::WING_MAX);
        connect(
            wing_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setWingAngle(int)));

		leg1_slider = create_joint_angle_slider(
	    "Leg 1", GLWidget::LEG_MIN, GLWidget::LEG_MAX);
        connect(
            leg1_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setLeg1Angle(int)));

		foot1_slider = create_joint_angle_slider(
	    "Foot 1", GLWidget::FOOT_MIN, GLWidget::FOOT_MAX);
        connect(
            foot1_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setFoot1Angle(int)));

		leg2_slider = create_joint_angle_slider(
	    "Leg 2", GLWidget::LEG_MIN, GLWidget::LEG_MAX);
        connect(
            leg2_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setLeg2Angle(int)));

		foot2_slider = create_joint_angle_slider(
	    "Foot 2", GLWidget::FOOT_MIN, GLWidget::FOOT_MAX);
        connect(
            foot2_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setFoot2Angle(int)));

		jaw_slider = create_joint_angle_slider(
	    "Jaw", GLWidget::JAW_MIN, GLWidget::JAW_MAX);
        connect(
            jaw_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setJawHeight(int)));

		height_slider = create_joint_angle_slider(
	    "Height", GLWidget::HEIGHT_MIN, GLWidget::HEIGHT_MAX);
        connect(
            height_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setHeight(int)));

		width_slider = create_joint_angle_slider(
	    "Width", GLWidget::WIDTH_MIN, GLWidget::WIDTH_MAX);
        connect(
            width_slider, SIGNAL(valueChanged(int)),
            m_gl_widget, SLOT(setWidth(int)));


        m_main_layout->addWidget(m_animate_checkbox);
        m_main_layout->addWidget(m_quit_button);
        setLayout(m_main_layout);
	
		head_slider->setValue(0);
		wing_slider->setValue(0);

        setWindowTitle("CSC418/2504 Assignment 1");
    }

public slots:
    void onPressQuit(bool)
    {
        exit(0);
    }

private:
    QSlider *create_joint_angle_slider(
	const char *label, int min_angle, int max_angle)
    {
        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(min_angle, max_angle);
        slider->setSingleStep(1);
        slider->setPageStep(5);
        slider->setTickInterval(5);
        slider->setTickPosition(QSlider::TicksBelow);

	QBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(new QLabel(label));
	layout->addWidget(slider);
	m_main_layout->addLayout(layout);

        return slider;
    }

    GLWidget *m_gl_widget;
    QCheckBox *m_animate_checkbox;
    QPushButton *m_quit_button;
    QVBoxLayout *m_main_layout;

	QSlider *head_slider;
	QSlider *wing_slider;
	QSlider *leg1_slider;
	QSlider *leg2_slider;
	QSlider *foot1_slider;
	QSlider *foot2_slider;
	QSlider *jaw_slider;

	QSlider *height_slider;
	QSlider *width_slider;
};

#endif
