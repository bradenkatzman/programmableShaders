In this project I implemented GPU code by writing a vertex shader. The vertex data is processed in phong.vert and the resulting color from the Phong Reflection Model is passed to phong.frag to set the final color. The result is an extremely realistic looking reflection model. Pictures of the rendering can be found in .png files capsule1-capsule4 and show the many advantages to using this model when programatically creating reflectance.

This program will take a large .obj file and render a mesh using vertices, vertex normals, texture coordinates, and faces. Here are the transformations that can be applied to the object by the user

**In Part3 only, use the PHONG.VERT vertex shader

**In Part4 only, use the PHONG1.VERT vertex shader. Further, a 4th parameter must be supplied, the color of the object to be rendered. The options include (note this must be entered in the exact same way - lowercase - or the program will default to a gray color):
		green
		red
		blue
		limegreen

Clicking the Mouse: this will orbit the camera around the object, leaving the object in place.

Keyboard Calls:

'i' will zoom in on the object

'o' will zoom out on the object

***** W,A,Z,S form a NWSE directional pad for moving the object *****

'w' will move the object north

'a' will move the object west

'z' will move the object south

'z' will move the object east

****** R, L will rotate the object itself while keep the camera and light source in place *****

'r' will rotate the object right

'l' will rotate the object left

THE FOLLOWING TRANSFORMATIONS ARE ONLY AVAILABLE IN THE PART4MAIN FILE

****** N,M will change the shine factor on the object i.e. make the reflection brighter or lower *******

'n' will lower the reflection

'm' will increase the reflection

****** 1,2 will change the geometry of the object by stretching and shrinking the object

'1' will shrink the object

'2' will stretch the object