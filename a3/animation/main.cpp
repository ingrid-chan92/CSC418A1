/***********************************************************
             CSC418, FALL 2009
 
                 penguin.cpp
                 author: Mike Pratscher
                 based on code by: Eron Steger, J. Radulovich

		Main source file for assignment 2
		Uses OpenGL, GLUT and GLUI libraries
  
    Instructions:
        Please read the assignment page to determine 
        exactly what needs to be implemented.  Then read 
        over this file and become acquainted with its 
        design. In particular, see lines marked 'README'.
		
		Be sure to also look over keyframe.h and vector.h.
		While no changes are necessary to these files, looking
		them over will allow you to better understand their
		functionality and capabilites.

        Add source code where it appears appropriate. In
        particular, see lines marked 'TODO'.

        You should not need to change the overall structure
        of the program. However it should be clear what
        your changes do, and you should use sufficient comments
        to explain your code.  While the point of the assignment
        is to draw and animate the character, you will
        also be marked based on your design.

***********************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "keyframe.h"
#include "timer.h"
#include "vector.h"
#include "particle.h"
#include "Util.h"

#define X .525731112119133606 
#define Z .850650808352039932


// *************** GLOBAL VARIABLES *************************


const float PI = 3.14159;

const float SPINNER_SPEED = 0.1;


// --------------- USER INTERFACE VARIABLES -----------------

// Window settings
int windowID;				// Glut window ID (for display)
int Win[2];					// window (x,y) size

GLUI* glui_joints;			// Glui window with joint controls
GLUI* glui_keyframe;		// Glui window with keyframe controls
GLUI* glui_render;			// Glui window for render style
GLUI* glui_lightsource;			// Glui window for light source

char msg[256];				// String used for status message
GLUI_StaticText* status;	// Status message ("Status: <msg>")


// ---------------- ANIMATION VARIABLES ---------------------

// Camera settings
bool updateCamZPos = false;
int  lastX = 0;
int  lastY = 0;
const float ZOOM_SCALE = 0.01;

GLdouble camXPos =  0.0;
GLdouble camYPos =  0.0;
GLdouble camZPos = -7.5;

const GLdouble CAMERA_FOVY = 60.0;
const GLdouble NEAR_CLIP   = 0.1;
const GLdouble FAR_CLIP    = 1000.0;

// Render settings
enum { WIREFRAME, SOLID, OUTLINED };	// README: the different render styles
int renderStyle = WIREFRAME;			// README: the selected render style

// Material properties
enum { NORMAL, METALLIC, MATTE };
int materialProp = NORMAL;

// Animation settings
int animate_mode = 0;			// 0 = no anim, 1 = animate

// Keyframe settings
const char filenameKF[] = "keyframes.txt";	// file for loading / saving keyframes

Keyframe* keyframes;			// list of keyframes

int maxValidKeyframe   = 0;		// index of max VALID keyframe (in keyframe list)
const int KEYFRAME_MIN = 0;
const int KEYFRAME_MAX = 32;	// README: specifies the max number of keyframes

// Frame settings
char filenameF[128];			// storage for frame filename

int frameNumber = 0;			// current frame being dumped
int frameToFile = 0;			// flag for dumping frames to file

const float DUMP_FRAME_PER_SEC = 24.0;		// frame rate for dumped frames
const float DUMP_SEC_PER_FRAME = 1.0 / DUMP_FRAME_PER_SEC;

// Time settings
Timer* animationTimer;
Timer* frameRateTimer;

const float TIME_MIN = 0.0;
const float TIME_MAX = 10.0;	// README: specifies the max time of the animation
const float SEC_PER_FRAME = 1.0 / 60.0;

// Joint settings

// README: This is the key data structure for
// updating keyframes in the keyframe list and
// for driving the animation.
//   i) When updating a keyframe, use the values
//      in this data structure to update the
//      appropriate keyframe in the keyframe list.
//  ii) When calculating the interpolated pose,
//      the resulting pose vector is placed into
//      this data structure. (This code is already
//      in place - see the animate() function)
// iii) When drawing the scene, use the values in
//      this data structure (which are set in the
//      animate() function as described above) to
//      specify the appropriate transformations.
Keyframe* joint_ui_data;

// README: To change the range of a particular DOF,
// simply change the appropriate min/max values below
const float FULL_TRANSLATE_X_MIN = -5.0;
const float FULL_TRANSLATE_X_MAX =  5.0;
const float FULL_TRANSLATE_Y_MIN = -5.0;
const float FULL_TRANSLATE_Y_MAX =  5.0;
const float FULL_TRANSLATE_Z_MIN = -5.0;
const float FULL_TRANSLATE_Z_MAX =  5.0;
const float FULL_ROTATE_X_MIN    = -180.0;
const float FULL_ROTATE_X_MAX    =  180.0;
const float FULL_ROTATE_Y_MIN    = -180.0;
const float FULL_ROTATE_Y_MAX    =  180.0;
const float FULL_ROTATE_Z_MIN    = -180.0;
const float FULL_ROTATE_Z_MAX    =  180.0;
const float CHAR_SCALE_X_MIN     =  0.0;
const float CHAR_SCALE_X_MAX     =  1.5;
const float CHAR_SCALE_Y_MIN     =  0.0;
const float CHAR_SCALE_Y_MAX     =  1.5;
const float CHAR_SCALE_Z_MIN     =  0.0;
const float CHAR_SCALE_Z_MAX     =  1.5;
const float ROOT_SCALE_X_MIN     =  0.0;
const float ROOT_SCALE_X_MAX     =  1.5;
const float ROOT_SCALE_Y_MIN     =  0.0;
const float ROOT_SCALE_Y_MAX     =  1.5;
const float ROOT_SCALE_Z_MIN     =  0.0;
const float ROOT_SCALE_Z_MAX     =  1.5;
const float EYE_BLINK_MIN        =  0.1;
const float EYE_BLINK_MAX        =  1.0;
const float EYE_SIZE_MIN         =  0.8;
const float EYE_SIZE_MAX         =  1.2;  
const float FOOT_PITCH_MIN       = -45.0;
const float FOOT_PITCH_MAX       =  45.0;
const float FOOT_YAW_MIN         = -180.0;
const float FOOT_YAW_MAX         =  180.0;
const float LIGHT_TRANS_Z_MIN	 =   0.0;
const float LIGHT_TRANS_Z_MAX	 =  40.0;
const float LIGHT_ROTATE_Z_MIN	 = -180.0;
const float LIGHT_ROTATE_Z_MAX 	 =  180.0; 

// Particle initialization
ParticleEffect* snow;

// Stores quadratic objects
GLUquadricObj *qobj; 

// ***********  FUNCTION HEADER DECLARATIONS ****************


// Initialization functions
void initDS();
void initGlut(int argc, char** argv);
void initGlui();
void initGl();


// Callbacks for handling events in glut
void reshape(int w, int h);
void animate();
void display(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);


// Functions to help draw the object
Vector getInterpolatedJointDOFS(float time);
void drawCube();
void normalize(GLfloat *a);
void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r);
void drawsphere(int ndiv, float radius);


void drawBody();
void drawHead();
void drawEye(float k, float r, float h);
void drawBeak();
void drawArm();
void drawHand();
void drawLeg();
void drawFoot();

// Helper functions for display
void lighting();
void drawBlob();


// Image functions
void writeFrame(char* filename, bool pgm, bool frontBuffer);


// ******************** FUNCTIONS ************************



// main() function
// Initializes the user interface (and any user variables)
// then hands over control to the event handler, which calls 
// display() whenever the GL window needs to be redrawn.
int main(int argc, char** argv)
{

    // Process program arguments
    if(argc != 3) {
        printf("Usage: demo [width] [height]\n");
        printf("Using 640x480 window by default...\n");
        Win[0] = 640;
        Win[1] = 480;
    } else {
        Win[0] = atoi(argv[1]);
        Win[1] = atoi(argv[2]);
    }


    // Initialize data structs, glut, glui, and opengl
	initDS();
    initGlut(argc, argv);
    initGlui();
    initGl();

	// Initialize the snow
    snow->initParticles();

    // Invoke the standard GLUT main event loop
    glutMainLoop();

    return 0;         // never reached
}


// Create / initialize global data structures
void initDS()
{
	keyframes = new Keyframe[KEYFRAME_MAX];
	for( int i = 0; i < KEYFRAME_MAX; i++ )
		keyframes[i].setID(i);

	animationTimer = new Timer();
	frameRateTimer = new Timer();
	joint_ui_data  = new Keyframe();
	snow = new ParticleEffect();
	qobj = gluNewQuadric();
}


// Initialize glut and create a window with the specified caption 
void initGlut(int argc, char** argv)
{
	// Init GLUT
	glutInit(&argc, argv);

    // Set video mode: double-buffered, color, depth-buffered
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create window
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Win[0],Win[1]);
    windowID = glutCreateWindow(argv[0]);

    // Setup callback functions to handle events
    glutReshapeFunc(reshape);	// Call reshape whenever window resized
    glutDisplayFunc(display);	// Call display whenever new frame needed
	glutMouseFunc(mouse);		// Call mouse whenever mouse button pressed
	glutMotionFunc(motion);		// Call motion whenever mouse moves while button pressed
}


// Load Keyframe button handler. Called when the "load keyframe" button is pressed
void loadKeyframeButton(int)
{
	// Get the keyframe ID from the UI
	int keyframeID = joint_ui_data->getID();

	// Update the 'joint_ui_data' variable with the appropriate
	// entry from the 'keyframes' array (the list of keyframes)
	*joint_ui_data = keyframes[keyframeID];

	// Sync the UI with the 'joint_ui_data' values
	glui_joints->sync_live();
	glui_keyframe->sync_live();
	glui_lightsource->sync_live();

	// Let the user know the values have been loaded
	sprintf(msg, "Status: Keyframe %d loaded successfully", keyframeID);
	status->set_text(msg);
}

// Update Keyframe button handler. Called when the "update keyframe" button is pressed
void updateKeyframeButton(int)
{
	// Get the keyframe ID from the UI
	int keyframeID = joint_ui_data->getID();

	// Update the 'maxValidKeyframe' index variable
	// (it will be needed when doing the interpolation)
	if (keyframeID > maxValidKeyframe)
		maxValidKeyframe = keyframeID;

	// Update the appropriate entry in the 'keyframes' array
	// with the 'joint_ui_data' data
	keyframes[keyframeID] = *joint_ui_data;	

	// Let the user know the values have been updated
	sprintf(msg, "Status: Keyframe %d updated successfully", keyframeID);
	status->set_text(msg);
}

// Load Keyframes From File button handler. Called when the "load keyframes from file" button is pressed
//
// ASSUMES THAT THE FILE FORMAT IS CORRECT, ie, there is no error checking!
//
void loadKeyframesFromFileButton(int)
{
	// Open file for reading
	FILE* file = fopen(filenameKF, "r");
	if( file == NULL )
	{
		sprintf(msg, "Status: Failed to open file %s", filenameKF);
		status->set_text(msg);
		return;
	}

	// Read in maxValidKeyframe first
	fscanf(file, "%d", &maxValidKeyframe);

	// Now read in all keyframes in the format:
	//    id
	//    time
	//    DOFs
	//
	for( int i = 0; i <= maxValidKeyframe; i++ )
	{
		fscanf(file, "%d", keyframes[i].getIDPtr());
		fscanf(file, "%f", keyframes[i].getTimePtr());

		for( int j = 0; j < Keyframe::NUM_JOINT_ENUM; j++ )
			fscanf(file, "%f", keyframes[i].getDOFPtr(j));
	}

	// Close file
	fclose(file);

	// Let the user know the keyframes have been loaded
	sprintf(msg, "Status: Keyframes loaded successfully");
	status->set_text(msg);
}

// Save Keyframes To File button handler. Called when the "save keyframes to file" button is pressed
void saveKeyframesToFileButton(int)
{
	// Open file for writing
	FILE* file = fopen(filenameKF, "w");
	if( file == NULL )
	{
		sprintf(msg, "Status: Failed to open file %s", filenameKF);
		status->set_text(msg);
		return;
	}

	// Write out maxValidKeyframe first
	fprintf(file, "%d\n", maxValidKeyframe);
	fprintf(file, "\n");

	// Now write out all keyframes in the format:
	//    id
	//    time
	//    DOFs
	//
	for( int i = 0; i <= maxValidKeyframe; i++ )
	{
		fprintf(file, "%d\n", keyframes[i].getID());
		fprintf(file, "%f\n", keyframes[i].getTime());

		for( int j = 0; j < Keyframe::NUM_JOINT_ENUM; j++ )
			fprintf(file, "%f\n", keyframes[i].getDOF(j));

		fprintf(file, "\n");
	}

	// Close file
	fclose(file);

	// Let the user know the keyframes have been saved
	sprintf(msg, "Status: Keyframes saved successfully");
	status->set_text(msg);
}

// Animate button handler.  Called when the "animate" button is pressed.
void animateButton(int)
{
  // synchronize variables that GLUT uses
  glui_keyframe->sync_live();

  // toggle animation mode and set idle function appropriately
  if( animate_mode == 0 )
  {
	// start animation
	frameRateTimer->reset();
	animationTimer->reset();

	animate_mode = 1;
	GLUI_Master.set_glutIdleFunc(animate);

	// Let the user know the animation is running
	sprintf(msg, "Status: Animating...");
	status->set_text(msg);
  }
  else
  {
	// stop animation
	animate_mode = 0;
	GLUI_Master.set_glutIdleFunc(NULL);

	// Clear the snow and reinitialize
	snow->clearParticles();

	// Let the user know the animation has stopped
	sprintf(msg, "Status: Animation stopped");
	status->set_text(msg);
  }
}

// Render Frames To File button handler. Called when the "Render Frames To File" button is pressed.
void renderFramesToFileButton(int)
{
	// Calculate number of frames to generate based on dump frame rate
	int numFrames = int(keyframes[maxValidKeyframe].getTime() * DUMP_FRAME_PER_SEC) + 1;

	// Generate frames and save to file
	frameToFile = 1;
	for( frameNumber = 0; frameNumber < numFrames; frameNumber++ )
	{
		// Get the interpolated joint DOFs
		joint_ui_data->setDOFVector( getInterpolatedJointDOFS(frameNumber * DUMP_SEC_PER_FRAME) );

		// Let the user know which frame is being rendered
		sprintf(msg, "Status: Rendering frame %d...", frameNumber);
		status->set_text(msg);

		// Render the frame
		display();
	}
	frameToFile = 0;

	// Let the user know how many frames were generated
	sprintf(msg, "Status: %d frame(s) rendered to file", numFrames);
	status->set_text(msg);
}

// Quit button handler.  Called when the "quit" button is pressed.
void quitButton(int)
{
  gluDeleteQuadric(qobj);
  exit(0);
}

// Initialize GLUI and the user interface
void initGlui()
{
	GLUI_Panel* glui_panel;
	GLUI_Spinner* glui_spinner;
	GLUI_RadioGroup* glui_radio_group;

    GLUI_Master.set_glutIdleFunc(NULL);


	// Create GLUI window (joint controls) ***************
	//
	glui_joints = GLUI_Master.create_glui("Joint Control", 0, Win[0]+12, 0);

	// Create controls to specify root position and orientation
	glui_panel = glui_joints->add_panel("Root");

	// Intialize scaling
	joint_ui_data->setDOF(Keyframe::ROOT_SCALE_X, 1.0);
	joint_ui_data->setDOF(Keyframe::ROOT_SCALE_Y, 1.0);
	joint_ui_data->setDOF(Keyframe::ROOT_SCALE_Z, 1.0);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_TRANSLATE_X));
	glui_spinner->set_float_limits(FULL_TRANSLATE_X_MIN, FULL_TRANSLATE_X_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_TRANSLATE_Y));
	glui_spinner->set_float_limits(FULL_TRANSLATE_Y_MIN, FULL_TRANSLATE_Y_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_TRANSLATE_Z));
	glui_spinner->set_float_limits(FULL_TRANSLATE_Z_MIN, FULL_TRANSLATE_Z_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_ROTATE_X));
	glui_spinner->set_float_limits(FULL_ROTATE_X_MIN, FULL_ROTATE_X_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_ROTATE_Y));
	glui_spinner->set_float_limits(FULL_ROTATE_Y_MIN, FULL_ROTATE_Y_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_ROTATE_Z));
	glui_spinner->set_float_limits(FULL_ROTATE_Z_MIN, FULL_ROTATE_Z_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "scale x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_SCALE_X));
	glui_spinner->set_float_limits(ROOT_SCALE_X_MIN, ROOT_SCALE_X_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "scale y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_SCALE_Y));
	glui_spinner->set_float_limits(ROOT_SCALE_Y_MIN, ROOT_SCALE_Y_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "scale z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_SCALE_Z));
	glui_spinner->set_float_limits(ROOT_SCALE_Z_MIN, ROOT_SCALE_Z_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_joints->add_column(false);

    // Create controls to specify char position and orientation
	glui_panel = glui_joints->add_panel("Char");
	// Initialize scaling
	joint_ui_data->setDOF(Keyframe::CHAR_SCALE_X, 1.0);
	joint_ui_data->setDOF(Keyframe::CHAR_SCALE_Y, 1.0);
	joint_ui_data->setDOF(Keyframe::CHAR_SCALE_Z, 1.0);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_TRANSLATE_X));
	glui_spinner->set_float_limits(FULL_TRANSLATE_X_MIN, FULL_TRANSLATE_X_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_TRANSLATE_Y));
	glui_spinner->set_float_limits(FULL_TRANSLATE_Y_MIN, FULL_TRANSLATE_Y_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_TRANSLATE_Z));
	glui_spinner->set_float_limits(FULL_TRANSLATE_Z_MIN, FULL_TRANSLATE_Z_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_ROTATE_X));
	glui_spinner->set_float_limits(FULL_ROTATE_X_MIN, FULL_ROTATE_X_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_ROTATE_Y));
	glui_spinner->set_float_limits(FULL_ROTATE_Y_MIN, FULL_ROTATE_Y_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_ROTATE_Z));
	glui_spinner->set_float_limits(FULL_ROTATE_Z_MIN, FULL_ROTATE_Z_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "scale x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_SCALE_X));
	glui_spinner->set_float_limits(CHAR_SCALE_X_MIN, CHAR_SCALE_X_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "scale y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_SCALE_Y));
	glui_spinner->set_float_limits(CHAR_SCALE_Y_MIN, CHAR_SCALE_Y_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "scale z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::CHAR_SCALE_Z));
	glui_spinner->set_float_limits(CHAR_SCALE_Z_MIN, CHAR_SCALE_Z_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Create controls for the sled
	glui_joints->add_column(false);
	glui_panel = glui_joints->add_panel("Sled");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::SLED_ROTATE_X));
	glui_spinner->set_float_limits(FULL_ROTATE_X_MIN, FULL_ROTATE_X_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::SLED_ROTATE_Y));
	glui_spinner->set_float_limits(FULL_ROTATE_Y_MIN, FULL_ROTATE_Y_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::SLED_ROTATE_Z));
	glui_spinner->set_float_limits(FULL_ROTATE_Z_MIN, FULL_ROTATE_Z_MAX, GLUI_LIMIT_WRAP);


	glui_joints->add_column(false);
	// Initialize foot angle
	joint_ui_data->setDOF(Keyframe::R_FOOT_YAW, 45.0);
	joint_ui_data->setDOF(Keyframe::L_FOOT_YAW, -45.0);

	// Create controls to specify right foot
	glui_panel = glui_joints->add_panel("Right foot");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "foot pitch:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_FOOT_PITCH));
	glui_spinner->set_float_limits(FOOT_PITCH_MIN, FOOT_PITCH_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "foot yaw:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_FOOT_YAW));
	glui_spinner->set_float_limits(FOOT_YAW_MIN, FOOT_YAW_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Create controls to specify left foot
	glui_panel = glui_joints->add_panel("Left foot");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "foot pitch:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_FOOT_PITCH));
	glui_spinner->set_float_limits(FOOT_PITCH_MIN, FOOT_PITCH_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "foot yaw:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_FOOT_YAW));
	glui_spinner->set_float_limits(FOOT_YAW_MIN, FOOT_YAW_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Initialize eye size
	joint_ui_data->setDOF(Keyframe::EYE_BLINK, 1.0);
	joint_ui_data->setDOF(Keyframe::EYE_SIZE, 1.0);

	glui_panel = glui_joints->add_panel("Eye");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "eye blink:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::EYE_BLINK));
	glui_spinner->set_float_limits(EYE_BLINK_MIN, EYE_BLINK_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "eye size:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::EYE_SIZE));
	glui_spinner->set_float_limits(EYE_SIZE_MIN, EYE_SIZE_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);


	//
    // ***************************************************


    // Create GLUI window (lightsource controls) ************
    //
    glui_lightsource = GLUI_Master.create_glui("Light Source Control", 0, 530, Win[1]+64);

	glui_panel = glui_lightsource->add_panel("");
    joint_ui_data->setDOF(Keyframe::LIGHT_TRANS_Z, 20.0);

    glui_spinner = glui_lightsource->add_spinner_to_panel(glui_panel, "Translate along z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::LIGHT_TRANS_Z));
    glui_spinner->set_float_limits(LIGHT_TRANS_Z_MIN, LIGHT_TRANS_Z_MAX, GLUI_LIMIT_CLAMP);
    glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_lightsource->add_spinner_to_panel(glui_panel, "Rotate around z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::LIGHT_ROTATE_Z));
    glui_spinner->set_float_limits(LIGHT_ROTATE_Z_MIN, LIGHT_ROTATE_Z_MAX, GLUI_LIMIT_WRAP);
    glui_spinner->set_speed(SPINNER_SPEED);


	// Create GLUI window (keyframe controls) ************
	//
	glui_keyframe = GLUI_Master.create_glui("Keyframe Control", 0, 0, Win[1]+64);

	// Create a control to specify the time (for setting a keyframe)
	glui_panel = glui_keyframe->add_panel("", GLUI_PANEL_NONE);
	glui_spinner = glui_keyframe->add_spinner_to_panel(glui_panel, "Time:", GLUI_SPINNER_FLOAT, joint_ui_data->getTimePtr());
	glui_spinner->set_float_limits(TIME_MIN, TIME_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Create a control to specify a keyframe (for updating / loading a keyframe)
	glui_keyframe->add_column_to_panel(glui_panel, false);
	glui_spinner = glui_keyframe->add_spinner_to_panel(glui_panel, "Keyframe ID:", GLUI_SPINNER_INT, joint_ui_data->getIDPtr());
	glui_spinner->set_int_limits(KEYFRAME_MIN, KEYFRAME_MAX-1, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_keyframe->add_separator();

	// Add buttons to load and update keyframes
	// Add buttons to load and save keyframes from a file
	// Add buttons to start / stop animation and to render frames to file
	glui_panel = glui_keyframe->add_panel("", GLUI_PANEL_NONE);
	glui_keyframe->add_button_to_panel(glui_panel, "Load Keyframe", 0, loadKeyframeButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Load Keyframes From File", 0, loadKeyframesFromFileButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Start / Stop Animation", 0, animateButton);
	glui_keyframe->add_column_to_panel(glui_panel, false);
	glui_keyframe->add_button_to_panel(glui_panel, "Update Keyframe", 0, updateKeyframeButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Save Keyframes To File", 0, saveKeyframesToFileButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Render Frames To File", 0, renderFramesToFileButton);

	glui_keyframe->add_separator();

	// Add status line
	glui_panel = glui_keyframe->add_panel("");
	status = glui_keyframe->add_statictext_to_panel(glui_panel, "Status: Ready");

	// Add button to quit
	glui_panel = glui_keyframe->add_panel("", GLUI_PANEL_NONE);
	glui_keyframe->add_button_to_panel(glui_panel, "Quit", 0, quitButton);
	//
	// ***************************************************


	// Create GLUI window (render controls) ************
	//
	glui_render = GLUI_Master.create_glui("Render Control", 0, 367, Win[1]+64);

	// Create control to specify the render style
	glui_panel = glui_render->add_panel("Render Style");
	glui_radio_group = glui_render->add_radiogroup_to_panel(glui_panel, &renderStyle);
	glui_render->add_radiobutton_to_group(glui_radio_group, "Wireframe");
	glui_render->add_radiobutton_to_group(glui_radio_group, "Solid");
	glui_render->add_radiobutton_to_group(glui_radio_group, "Solid w/ outlines");

	// Create control to specify the material options
	glui_panel = glui_render->add_panel("Material Properties");
        glui_radio_group = glui_render->add_radiogroup_to_panel(glui_panel, &materialProp);
        glui_render->add_radiobutton_to_group(glui_radio_group, "Normal");
        glui_render->add_radiobutton_to_group(glui_radio_group, "Metallic");
        glui_render->add_radiobutton_to_group(glui_radio_group, "Matte");
	//
        // ***************************************************

	// Tell GLUI windows which window is main graphics window
	glui_joints->set_main_gfx_window(windowID);
	glui_keyframe->set_main_gfx_window(windowID);
	glui_render->set_main_gfx_window(windowID);
	glui_lightsource->set_main_gfx_window(windowID);
}


// Performs most of the OpenGL intialization
void initGl(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    // Sets the background colour
    glClearColor(0.529411f,0.80784f,1.0f,1.0f);
}


// Calculates the interpolated joint DOF vector
// using Catmull-Rom interpolation of the keyframes
Vector getInterpolatedJointDOFS(float time)
{
	// Need to find the keyframes bewteen which
	// the supplied time lies.
	// At the end of the loop we have:
	//    keyframes[i-1].getTime() < time <= keyframes[i].getTime()
	//
	int i = 0;
	while( i <= maxValidKeyframe && keyframes[i].getTime() < time )
		i++;

	// If time is before or at first defined keyframe, then
	// just use first keyframe pose
	if( i == 0 )
		return keyframes[0].getDOFVector();

	// If time is beyond last defined keyframe, then just
	// use last keyframe pose
	if( i > maxValidKeyframe )
		return keyframes[maxValidKeyframe].getDOFVector();

	// Need to normalize time to (0, 1]
	time = (time - keyframes[i-1].getTime()) / (keyframes[i].getTime() - keyframes[i-1].getTime());

	// Get appropriate data points and tangent vectors
	// for computing the interpolation
	Vector p0 = keyframes[i-1].getDOFVector();
	Vector p1 = keyframes[i].getDOFVector();

	Vector t0, t1;
	if( i == 1 )							// special case - at beginning of spline
	{
		t0 = keyframes[i].getDOFVector() - keyframes[i-1].getDOFVector();
		t1 = (keyframes[i+1].getDOFVector() - keyframes[i-1].getDOFVector()) * 0.5;
	}
	else if( i == maxValidKeyframe )		// special case - at end of spline
	{
		t0 = (keyframes[i].getDOFVector() - keyframes[i-2].getDOFVector()) * 0.5;
		t1 = keyframes[i].getDOFVector() - keyframes[i-1].getDOFVector();
	}
	else
	{
		t0 = (keyframes[i].getDOFVector()   - keyframes[i-2].getDOFVector()) * 0.5;
		t1 = (keyframes[i+1].getDOFVector() - keyframes[i-1].getDOFVector()) * 0.5;
	}

	// Return the interpolated Vector
	Vector a0 = p0;
	Vector a1 = t0;
	Vector a2 = p0 * (-3) + p1 * 3 + t0 * (-2) + t1 * (-1);
	Vector a3 = p0 * 2 + p1 * (-2) + t0 + t1;

	return (((a3 * time + a2) * time + a1) * time + a0);
}


// Callback idle function for animating the scene
void animate()
{
	// Only update if enough time has passed
	// (This locks the display to a certain frame rate rather
	//  than updating as fast as possible. The effect is that
	//  the animation should run at about the same rate
	//  whether being run on a fast machine or slow machine)
	if( frameRateTimer->elapsed() > SEC_PER_FRAME )
	{
		// Tell glut window to update itself. This will cause the display()
		// callback to be called, which renders the object (once you've written
		// the callback).
		glutSetWindow(windowID);
		glutPostRedisplay();

		// Restart the frame rate timer
		// for the next frame
		frameRateTimer->reset();
	}
}


// Handles the window being resized by updating the viewport
// and projection matrices
void reshape(int w, int h)
{
	// Update internal variables and OpenGL viewport
	Win[0] = w;
	Win[1] = h;
	glViewport(0, 0, (GLsizei)Win[0], (GLsizei)Win[1]);

    // Setup projection matrix for new window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(CAMERA_FOVY, (GLdouble)Win[0]/(GLdouble)Win[1], NEAR_CLIP, FAR_CLIP);
}


// Set up the lighting
void lighting()
{
    // Light variables
    GLfloat light_specular[] = {0.0, 0.5, 1.0, 1.0};
    GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

    // Change the light position depending on the users' input
    GLfloat light_position[] = {8 * cos(joint_ui_data->getDOF(Keyframe::LIGHT_ROTATE_Z)), 
				8 * sin(joint_ui_data->getDOF(Keyframe::LIGHT_ROTATE_Z)), 
				joint_ui_data->getDOF(Keyframe::LIGHT_TRANS_Z), 1.0};

    // Setup the lights
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Default material property to normal
    float mat_specular[4]={0.297254f,0.30829f,0.306678f,1.0f};
    float mat_diffuse[4]={0.466666f,0.74151f,0.466666f,1.0f};
    float mat_ambient[4]={0.3f,0.38725f,0.3745f,1.0f};
    float mat_shininess[]={0.1 * 128};

    // Change the material properties depending on the style chosen
    if (materialProp == METALLIC )
    {
		memcpy(mat_specular, (const float [4]){0.8f,0.8f,0.8f,1.0f}, 4*sizeof(float));
        memcpy(mat_diffuse, (const float [4]){0.2f,0.2f,0.2f,1.0f}, 4*sizeof(float));
        memcpy(mat_ambient, (const float [4]){0.3f,0.3f,0.3f,1.0f}, 4*sizeof(float));
        memcpy(mat_shininess, (const float [1]){0.4 * 128}, sizeof(float));
    }
    else if (materialProp == MATTE)
    {
		memcpy(mat_specular, (const float [4]){0.1f,0.1f,0.1f,1.0f}, 4*sizeof(float));
        memcpy(mat_diffuse, (const float [4]){0.9f,0.9f,0.9f,1.0f}, 4*sizeof(float));
        memcpy(mat_ambient, (const float [4]){0.3f,0.3f,0.3f,1.0f}, 4*sizeof(float));
        memcpy(mat_shininess, (const float [1]){0.25 * 128}, sizeof(float));
    } 

    // Set the material properties
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
}

void setMat(float a1, float a2, float a3, float d1, float d2, float d3,
			float s1, float s2, float s3, float shiny)
{
    float mat_specular[4]={s1,s2,s3,1.0f};
    float mat_diffuse[4]={d1,d2,d3,1.0f};
    float mat_ambient[4]={a1,a2,a3,1.0f};
    float mat_shininess[]={shiny * 128};

    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
}


// Draw the character
void drawBlob()
{
	lighting();
	// Put into stack so can move/rotate the character
	glPushMatrix();
		// Allow for moving/translating of the character
		glTranslatef(joint_ui_data->getDOF(Keyframe::CHAR_TRANSLATE_X),
					 joint_ui_data->getDOF(Keyframe::CHAR_TRANSLATE_Y),
					 joint_ui_data->getDOF(Keyframe::CHAR_TRANSLATE_Z));
		glRotatef(joint_ui_data->getDOF(Keyframe::CHAR_ROTATE_X), 1.0, 0.0, 0.0);
		glRotatef(joint_ui_data->getDOF(Keyframe::CHAR_ROTATE_Y), 0.0, 1.0, 0.0);
		glRotatef(joint_ui_data->getDOF(Keyframe::CHAR_ROTATE_Z), 0.0, 0.0, 1.0);
		glScalef(joint_ui_data->getDOF(Keyframe::CHAR_SCALE_X),
				 joint_ui_data->getDOF(Keyframe::CHAR_SCALE_Y),
				 joint_ui_data->getDOF(Keyframe::CHAR_SCALE_Z));
		glColor3f(0.0f, 0.2f, 0.4f);

		// Create the body for the character (sphere)
		glutSolidSphere(0.5f, 30, 30);	

		// Draw the feet
		glPushMatrix();
			glTranslatef(0.0f, -0.48f, 0.0f);

			// Left foot
			glPushMatrix();
				glTranslatef(-0.26f, 0.0f, 0.0f);
				glRotatef(joint_ui_data->getDOF(Keyframe::L_FOOT_PITCH), 0.0, 0.0, 1.0);
                glRotatef(joint_ui_data->getDOF(Keyframe::L_FOOT_YAW), 0.0, 1.0, 0.0);
				glScalef(2.0f, 1.3f, 1.5f);
				// Draw foot and move joint
				glPushMatrix();
					glTranslatef(-0.04f, 0.0f, 0.0f);
					glutSolidSphere(0.1f, 20, 20);
				glPopMatrix();
			glPopMatrix();
			// Right foot
			glPushMatrix();
				glTranslatef(0.26f, 0.0f, 0.0f);
				glRotatef(joint_ui_data->getDOF(Keyframe::R_FOOT_PITCH), 0.0, 0.0, 1.0);
                glRotatef(joint_ui_data->getDOF(Keyframe::R_FOOT_YAW), 0.0, 1.0, 0.0);
				glScalef(2.0f, 1.3f, 1.5f);
				// Draw foot and move joint
				glPushMatrix();
					glTranslatef(0.04f, 0.0f, 0.0f);
					glutSolidSphere(0.1f, 20, 20);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		// Draw eyes
		// Stop lighting from affecting eyes (to reduce creepiness)
		glDisable (GL_LIGHTING);
    	glDisable (GL_LIGHT0);
		glPushMatrix();
			glTranslatef(0.0, 0.07, 0.31);
			float eye_size = joint_ui_data->getDOF(Keyframe::EYE_SIZE);
			glScalef(eye_size, eye_size*joint_ui_data->getDOF(Keyframe::EYE_BLINK), 1.0f);
			// Left eye
			glColor3f(1.0, 1.0, 1.0);
			glPushMatrix();
				glTranslatef(-0.13, 0.0, 0.0);
				glutSolidSphere(0.2, 20, 20);
				// Draw the pupil
				glColor3f(0.0, 0.0, 0.0);
				glPushMatrix();
					glTranslatef(-0.03, 0.01, 0.16);
					glutSolidSphere(0.07, 20, 20);
				glPopMatrix();
			glPopMatrix();
			// Right eye
			glColor3f(1.0, 1.0, 1.0);
			glPushMatrix();
				glTranslatef(0.13, 0.0, 0.0);
				glutSolidSphere(0.2, 20, 20);
				// Draw the pupil
				glColor3f(0.0, 0.0, 0.0);
				glPushMatrix();
					glTranslatef(0.03, 0.01, 0.16);
					glutSolidSphere(0.07, 20, 20);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void drawSled()
{
	// Set lighting and material of the sled
	lighting();
	setMat(0.0,0.0,0.0,0.5,0.0,0.0,0.7,0.6,0.6,0.25);
	// Put into stack so can move/rotate the character
	glPushMatrix();
		//glTranslatef(2.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, -0.3f, 0.0f);
		glRotatef(joint_ui_data->getDOF(Keyframe::SLED_ROTATE_X), 1.0, 0.0, 0.0);
		glRotatef(joint_ui_data->getDOF(Keyframe::SLED_ROTATE_Y), 0.0, 1.0, 0.0);
		glRotatef(joint_ui_data->getDOF(Keyframe::SLED_ROTATE_Z), 0.0, 0.0, 1.0);
		
		// Back of sled
		glPushMatrix();
			gluCylinder(qobj, 0.25f, 0.25f, 1.2f, 10, 2);
		
			// Draw the first cap
			gluQuadricOrientation(qobj, GLU_INSIDE);
			gluDisk(qobj, 0.0f, 0.25f, 10, 1.0f);
			glTranslatef(0.0f, 0.0f, 1.2f);

			// Draw the second cap
			gluQuadricOrientation(qobj, GLU_OUTSIDE);
			gluDisk(qobj, 0.0f, 0.25f, 10, 1.0f);
		glPopMatrix();

		// Bottom of the sled
		glPushMatrix();
			// NOTE: Normalizing necessary for material to be displayed correctly
			glEnable(GL_NORMALIZE);
			glTranslatef(0.6f, -0.19f, 0.6f);
			glScalef(0.65f, 0.05f, 0.6f);
			drawCube();
		glPopMatrix();

	glPopMatrix();

	glDisable(GL_NORMALIZE);
	glDisable (GL_LIGHTING);
    glDisable (GL_LIGHT0);
}

void drawSkybox() {
	float D = 50.0f;

	// Load textures for skybox (NEEDS CONFIGURATION
	GLUint sky_right = raw_texture_load("sample.bmp", 204, 204);
	GLUint sky_front = raw_texture_load("sample.bmp", 204, 204);
	GLUint sky_left = raw_texture_load("sample.bmp", 204, 204);
	GLUint sky_back = raw_texture_load("sample.bmp", 204, 204);
	GLUint sky_up = raw_texture_load("sample.bmp", 204, 204);
	GLUint sky_down = raw_texture_load("sample.bmp", 204, 204);

 	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	 
		// Sides 
		glBindTexture(GL_TEXTURE_2D, sky_right);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
			glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
			glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
			glTexCoord2f(0,1); glVertex3f(-D,+D,-D);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, sky_front);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
			glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
			glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
			glTexCoord2f(0,1); glVertex3f(+D,+D,-D);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, sky_left);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3f(+D,-D,+D);
			glTexCoord2f(1,0); glVertex3f(-D,-D,+D);
			glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
			glTexCoord2f(0,1); glVertex3f(+D,+D,+D);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, sky_back);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
			glTexCoord2f(1,0); glVertex3f(-D,-D,-D);
			glTexCoord2f(1,1); glVertex3f(-D,+D,-D);
			glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
		glEnd();
		 
		// Top and Bottom
		glBindTexture(GL_TEXTURE_2D, sky_up);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3f(-D,+D,-D);
			glTexCoord2f(1,0); glVertex3f(+D,+D,-D);
			glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
			glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, sky_down);
		glBegin(GL_QUADS);
			glTexCoord2f(1,1); glVertex3f(+D,-D,-D);
			glTexCoord2f(0,1); glVertex3f(-D,-D,-D);
			glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
			glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
		glEnd();
	 
	glDisable(GL_TEXTURE_2D); 

}

// display callback
//
// README: This gets called by the event handler
// to draw the scene, so this is where you need
// to build your scene -- make your changes and
// additions here. All rendering happens in this
// function. For Assignment 2, updates to the
// joint DOFs (joint_ui_data) happen in the
// animate() function.
void display(void)
{
    // Clear the screen with the background colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    glEnable (GL_DEPTH_TEST);

    // Setup the model-view transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// Specify camera transformation
	glTranslatef(camXPos, camYPos, camZPos);


	// Get the time for the current animation step, if necessary
	if( animate_mode )
	{
		float curTime = animationTimer->elapsed();

		if( curTime >= keyframes[maxValidKeyframe].getTime() )
		{
			// Restart the animation
			animationTimer->reset();
			curTime = animationTimer->elapsed();
		}

		// Get the interpolated joint DOFs
		joint_ui_data->setDOFVector( getInterpolatedJointDOFS(curTime) );

		// Update user interface
		joint_ui_data->setTime(curTime);
		glui_keyframe->sync_live();

		// Reinitialize any "dead" snow, and update the position of "alive" snow
		snow->setParticles();
		snow->updateParticles();
	}


	// Enable textures and bind our particle
	glPushMatrix();
		glTranslatef(joint_ui_data->getDOF(Keyframe::ROOT_TRANSLATE_X),
					 joint_ui_data->getDOF(Keyframe::ROOT_TRANSLATE_Y),
					 joint_ui_data->getDOF(Keyframe::ROOT_TRANSLATE_Z));
		glRotatef(joint_ui_data->getDOF(Keyframe::ROOT_ROTATE_X), 1.0, 0.0, 0.0);
		glRotatef(joint_ui_data->getDOF(Keyframe::ROOT_ROTATE_Y), 0.0, 1.0, 0.0);
		glRotatef(joint_ui_data->getDOF(Keyframe::ROOT_ROTATE_Z), 0.0, 0.0, 1.0);
		glScalef(joint_ui_data->getDOF(Keyframe::ROOT_SCALE_X),
				 joint_ui_data->getDOF(Keyframe::ROOT_SCALE_Y),
				 joint_ui_data->getDOF(Keyframe::ROOT_SCALE_Z));
		
		drawBlob();
		drawSled();
		drawSkybox();

		// Render the snow
		snow->renderParticles();
	glPopMatrix();


    // Execute any GL functions that are in the queue just to be safe
    glFlush();

	// Dump frame to file, if requested
	if( frameToFile )
	{
		sprintf(filenameF, "frame%03d.ppm", frameNumber);
		writeFrame(filenameF, false, false);
	}

    // Now, show the frame buffer that we just drew into.
    // (this prevents flickering).
    glutSwapBuffers();
}

// Handles mouse button pressed / released events
void mouse(int button, int state, int x, int y)
{
	// If the RMB is pressed and dragged then zoom in / out
	if( button == GLUT_RIGHT_BUTTON )
	{
		if( state == GLUT_DOWN )
		{
			lastX = x;
			lastY = y;
			updateCamZPos = true;
		}
		else
		{
			updateCamZPos = false;
		}
	}
}


// Handles mouse motion events while a button is pressed
void motion(int x, int y)
{
	// If the RMB is pressed and dragged then zoom in / out
	if( updateCamZPos )
	{
		// Update camera z position
		camZPos += (x - lastX) * ZOOM_SCALE;
		lastX = x;

		// Redraw the scene from updated camera position
		glutSetWindow(windowID);
		glutPostRedisplay();
	}
}

// Draw a unit cube, centered at the current location
// README: Helper code for drawing a cube
void drawCube()
{
	glBegin(GL_QUADS);
		// draw front face
		glNormal3f( 0.0,  0.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f( 1.0, -1.0, 1.0);
		glVertex3f( 1.0,  1.0, 1.0);
		glVertex3f(-1.0,  1.0, 1.0);

		// draw back face
		glNormal3f( 0.0,  0.0, -1.0);
		glVertex3f( 1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0,  1.0, -1.0);
		glVertex3f( 1.0,  1.0, -1.0);

		// draw left face
		glNormal3f(-1.0,  0.0,  0.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0,  1.0);
		glVertex3f(-1.0,  1.0,  1.0);
		glVertex3f(-1.0,  1.0, -1.0);

		// draw right face
		glNormal3f( 1.0,  0.0,  0.0);
		glVertex3f( 1.0, -1.0,  1.0);
		glVertex3f( 1.0, -1.0, -1.0);
		glVertex3f( 1.0,  1.0, -1.0);
		glVertex3f( 1.0,  1.0,  1.0);

		// draw top
		glNormal3f( 0.0,  1.0,  0.0);
		glVertex3f(-1.0,  1.0,  1.0);
		glVertex3f( 1.0,  1.0,  1.0);
		glVertex3f( 1.0,  1.0, -1.0);
		glVertex3f(-1.0,  1.0, -1.0);

		// draw bottom
		glNormal3f( 0.0, -1.0,  0.0);
		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f( 1.0, -1.0, -1.0);
		glVertex3f( 1.0, -1.0,  1.0);
		glVertex3f(-1.0, -1.0,  1.0);
	glEnd();
}

///////////////////////////////////////////////////////////
//
// BELOW ARE FUNCTIONS FOR GENERATING IMAGE FILES (PPM/PGM)
//
///////////////////////////////////////////////////////////

void writePGM(char* filename, GLubyte* buffer, int width, int height, bool raw=true)
{
	FILE* fp = fopen(filename,"wt");

	if( fp == NULL )
	{
		printf("WARNING: Can't open output file %s\n",filename);
		return;
	}

	if( raw )
	{
		fprintf(fp,"P5\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			fwrite(&buffer[y*width],sizeof(GLubyte),width,fp);
			/*
			for(int x=0;x<width;x++)
			{
				fprintf(fp,"%c",int(buffer[x+y*width];
			}
			*/
		}
	}
	else
	{
		fprintf(fp,"P2\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			for(int x=0;x<width;x++)
			{
				fprintf(fp, "%d ", *((int*)(&buffer[x+y*width]))); //fprintf(fp,"%d ",int(&buffer[x+y*width]));
			}
			fprintf(fp,"\n");
		}
	}

	fclose(fp);
}

#define RED_OFFSET   0
#define GREEN_OFFSET 1
#define BLUE_OFFSET  2

void writePPM(char* filename, GLubyte* buffer, int width, int height, bool raw=true)
{
	FILE* fp = fopen(filename,"wt");

	if( fp == NULL )
	{
		printf("WARNING: Can't open output file %s\n",filename);
		return;
	}

	if( raw )
	{
		fprintf(fp,"P6\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			for(int x=0;x<width;x++)
			{
				GLubyte* pix = &buffer[4*(x+y*width)];

				fprintf(fp,"%c%c%c",int(pix[RED_OFFSET]),
									int(pix[GREEN_OFFSET]),
									int(pix[BLUE_OFFSET]));
			}
		}
	}
	else
	{
		fprintf(fp,"P3\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			for(int x=0;x<width;x++)
			{
				GLubyte* pix = &buffer[4*(x+y*width)];

				fprintf(fp,"%d %d %d ",int(pix[RED_OFFSET]),
									   int(pix[GREEN_OFFSET]),
									   int(pix[BLUE_OFFSET]));
			}
			fprintf(fp,"\n");
		}
	}

	fclose(fp);
}

void writeFrame(char* filename, bool pgm, bool frontBuffer)
{
	static GLubyte* frameData = NULL;
	static int currentSize = -1;

	int size = (pgm ? 1 : 4);

	if( frameData == NULL || currentSize != size*Win[0]*Win[1] )
	{
		if (frameData != NULL)
			delete [] frameData;

		currentSize = size*Win[0]*Win[1];

		frameData = new GLubyte[currentSize];
	}

	glReadBuffer(frontBuffer ? GL_FRONT : GL_BACK);

	if( pgm )
	{
		glReadPixels(0, 0, Win[0], Win[1],
					 GL_LUMINANCE, GL_UNSIGNED_BYTE, frameData);
		writePGM(filename, frameData, Win[0], Win[1]);
	}
	else
	{
		glReadPixels(0, 0, Win[0], Win[1],
					 GL_RGBA, GL_UNSIGNED_BYTE, frameData);
		writePPM(filename, frameData, Win[0], Win[1]);
	}
}
