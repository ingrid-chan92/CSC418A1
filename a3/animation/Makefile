##############################################################################
#
#  Sample Makefile for C/C++ applications for the UTSC environment.
#    Works for single and multiple file programs.
#
##############################################################################

# Define Subdirectory for SOIL
SUB_DIR	      = SOIL/

# Define C compiler
CC            = gcc

# Define C++ compiler
CCC	          = /usr/bin/g++

# Define C compiler options
CFLAGS        = -Wall -c -g -w #rmv -w

# Define C++ compiler options
CCCFLAGS      = -Wall -c -g -w #rmv -w

# Define C/C++ pre-processor options
CPPFLAGS      = -I./ -I/u/csc418h/include/fall05/include 

# Define location of OpenGL and GLU libraries along with their lib names
GL_LIBS       = -L/u/csc418h/include/fall05/lib -lGLU -lGL -lglut -lglui

# Define location of X Windows libraries,  and the X11 library names
XLIBS         = -L/usr/X11R6/lib -L/usr/lib -lX11

# Define the location of the destination directory for the executable file
DEST	      = .

# Define flags that should be passed to the linker
LDFLAGS	      =

# Define libraries to be linked with
LIBS	      = $(GL_LIBS) $(GLUT_LIBS) -lm $(XLIBS) -ldl

# Define linker
LINKER	      = g++

# Define all object files to be the same as CPPSRCS but with all the .cpp and .c suffixes replaced with .o
OBJ           = $(CPPSRCS:.cpp=.o) $(CSRCS:.c=.o)

# Define name of target executable
PROGRAM	          = main

# Define all C source files here
CSRCS         = $(SUB_DIR)image_DXT.c $(SUB_DIR)image_helper.c $(SUB_DIR)stb_image_aug.c $(SUB_DIR)SOIL.c

# Define all C++ source files here
CPPSRCS       = main.cpp vector.cpp particle.cpp Util.cpp Terrain.cpp

##############################################################################
# Define additional rules that make should know about in order to compile our
# files.                                        
##############################################################################

# Define default rule if Make is run without arguments
all : $(PROGRAM)

# Define rule for compiling all C++ files
%.o : %.cpp
	$(CCC) $(CCCFLAGS) $(CPPFLAGS) $*.cpp
	
# Define rule for compiling all C files
%.o : $(SUB_DIR)%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $*.c
	
# Define rule for creating executable
$(PROGRAM) :	$(OBJ)
		@echo -n "Loading $(PROGRAM) ... "
		$(LINKER) $(LDFLAGS) $(OBJ) $(LIBS) -o $(PROGRAM)
		@echo "done"
		
# Define rule to clean up directory by removing all object, temp and core
# files along with the executable
clean :
	@rm -f $(OBJ) *~ core $(PROGRAM)






