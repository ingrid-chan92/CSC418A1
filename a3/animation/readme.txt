IMPORTANT NOTES
-------------------------------------------------------------
 - Must set a keyframe or things disappear when you start/stop the animation... Will dela with that later since we have a workaround


SO FAR
-------------------------------------------------------------
- Rendered character (drawBlob)
- Rendered sled (drawSled)
	- Both rendered at origin for now...
- Can translate/rotate/scale whole character
	- Can move feet (up/down, side to side)
	- Can "blink" and increase/decrease eye size (word of warning - it gets a little wonky when increaseing it too much)
- Can rotate sled (So far, I haven't thought of a need to translate/scale it but we can always add it in later)
- Can translate/rotate/scale the characer and sled together (and I guess that will be a big part of the animation)

- Lighting - it's still from A2 so we'll have to change it. The only modification I made was to set it relatively far away on the z-axis so that whatever's at the origin gets the light head-on
	- Will definitely have to change lighting() function... Not the most modular code atm =_=
	- Added setMat() function to change the material quickly


GENERAL APPROACH
-------------------------------------------------------------
- Ideas from the meeting:
	- Snow
	- Camera panning
	- Sled ride
	- First person view at times
	- Cute ball sledding (at the end, turns around and a bunch of balls are coming down as well)
- Vague ideas I had while working on this
	- Maybe make it a bit like how a game-character must feel? (Based on how the character looks, maybe Kirby's cousin =P) -- so for the first scene, maybe have the blob fall down from the sky. I'm pretty sure we don't have the time to make it really convincing but we could add subtle things like that
	- To start the avalanche of balls, maybe the character could crash into a tree? Though it might be better to just have it suddenly realize it's being "chased". 

- So far - mostly doing whatever I can think of on the spot so ._.

- I think the next biggest thing to render is the background... How to do this is a whole nother question (Possibly render a sliver of a mountain with trees/other things scattered? But I don't think the given code cuts things off at the edge of the window so I'm not sure how slow it will be, depending on how much stuff there is)
    - After the background, I think we'll have the bulk of what's needed for animating most of it

- Then we'll have the ending scene 
	- If just realizes balls are coming after - we'll have to decide how blob notices 
	    - Maybe happens to turn around while on the sled ride
	    - Maybe ends the sled ride and turns around to see them coming 
	- If we choose to crash into a tree, we should (but not necessary) render a broken sled. Possibly make the scene shake a little before blob turns around
	- When blob turns around
		- We could (if strapped for time) just end with a look of horror
		- Else, we could have him run behind a tree/rock to have the snowballs tumble down around him and end with a look of somewhat relief
		- Something else? 


TODO
-------------------------------------------------------------
- Background
	- Figure out how to do this in general... We'll need to be able to move down the mountain and pan the camera, as well as change to first person view (so we should be able to look "downhill")
	- Make trees
	- Maybe make rocks for variety? Really only if we have extra time
- Animating down the hill
	- Will have to change the snow (speed, direction falling) to emulate sledding
	- If we have the time - little bits of snow coming from the bottom of the sled to give a more realistic "moving" feeling
- Lighting
	- Probably need more than one light.....
	- I'm not sure what exactly is needed... We could just add them as we encounter the need?
	- Will need to add the ability to save the lighting effects when animating!
- Snow
	- Currently, the snow falling is restricted by constants denoting the window size. Instead, use the projection matrix on the window size to find out the actual edges
	- Could also make the snow flake die when they hit the "ground" instead of the arbitrarily set bottom
- Ending scene
	- Depending on which ending - possibly a broken sled
	- Possibly a mouth for blob (need moar horrified features)
	- ???
- ???


SKYBOX
-------------------------------------------------------------
Sources: http://www.davidwparker.com/2012/12/26/opengl-screencast-video-21-skybox/

SNOW
-------------------------------------------------------------
- Most of the code for the snow is implemented in particle.h and particle.cpp
- Calls to make the snow work in "main.cpp" can be found by finding on "snow"
	- Snow is set in "initDS" 
	- Initializing of the snow called in "main"
	- Clearing of the snow called in "animateButton"
	- Reviving/Updating/Rendering snow called in "display"
	
- Used the SOIL library to read the texture... I stole the src files only to reduce space

- Got a lot of help from (to declare in the Checklist?):
	https://78462f86-a-306c325b-s-sites.googlegroups.com/a/fullonsoftware.co.uk/www/snippets/content/OpenGL_-_Particle_System_Tutorial_-_Blending_-_PointSprites.pdf?attachauth=ANoY7cpiAk85m-br_0j-ARyFYkA-YToaXVHkTgoVCFnHzGYnbvFIxnt2efJgz4JKfVVzQbedGMhWkQUR8CDSYYyEAx91quNgUcOuG-7Pc50OOgdPC5SGgVYfGFlqnLPJj9RlLveE6ScdKeb9WSGOB5sxQ6kLgPEeFXa2vjzRqLRknL-IBLq_xlp8j_mx3pvSI9Nb500TRSwGKxoGNoKefNKrd6Jmv6O-7wS99TW5IeY5DOR9zZvjeTfLm233dj9KDa4IktTHxCbxPdkT00B3PUhY1BgRFR-tNRoXf8NUWgGZSPvIy_fk-eI%3D&attredirects=0
