SNOW
- Most of the code for the snow is implemented in particle.h and particle.cpp
- Calls to make the snow work in "main.cpp" can be found by finding on "snow"
	- Initializing/Clearing of the snow called in "animateButton"
	- Reviving/Updating/Rendering snow called in "display"
	- snow is set in "initDS"
- Used the SOIL library to read the texture... I stole the src files only to reduce space

- Got a lot of help from (to declare in the Checklist?):
	https://78462f86-a-306c325b-s-sites.googlegroups.com/a/fullonsoftware.co.uk/www/snippets/content/OpenGL_-_Particle_System_Tutorial_-_Blending_-_PointSprites.pdf?attachauth=ANoY7cpiAk85m-br_0j-ARyFYkA-YToaXVHkTgoVCFnHzGYnbvFIxnt2efJgz4JKfVVzQbedGMhWkQUR8CDSYYyEAx91quNgUcOuG-7Pc50OOgdPC5SGgVYfGFlqnLPJj9RlLveE6ScdKeb9WSGOB5sxQ6kLgPEeFXa2vjzRqLRknL-IBLq_xlp8j_mx3pvSI9Nb500TRSwGKxoGNoKefNKrd6Jmv6O-7wS99TW5IeY5DOR9zZvjeTfLm233dj9KDa4IktTHxCbxPdkT00B3PUhY1BgRFR-tNRoXf8NUWgGZSPvIy_fk-eI%3D&attredirects=0

- TODO: Currently, the snow falling is restricted by constants denoting the window size. Instead, use the projection matrix on the window size to find out the actual edges

