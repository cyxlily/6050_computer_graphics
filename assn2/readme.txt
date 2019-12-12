6050 Computer Graphics
assn2
Yuxin Cui

Contents:

	It's a C++ program to view and transform 3D models. It can load vertices and faces from the .obj file. It has three transformation mode: translation, rotation and scaling and two view mode: orthographic and perspective.
 

Environment:

	$ sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

Compile:

	$ make

	$ ./assn2

Usage:

	1. The model are moved to the origin, and displayed about 0.25 of the window. You can change the model obj file in "read_obj("model1.obj");", remember to recompile it.

	2. The default transformation mode is "Translation". Press "a" to move to the left, "d" to move to the right, "w" to move to the top and "s" to move to the bottom. Later, You can press "t" key to switch to the "Translation" mode. The speed of the transform is 0.1 of the model.

	3. Press "r" to rotate the model. Press "a" to rotate left around the y axis, "d" to rotate right around the y axis, "w" to rotate top around the x axis and "s" to rotate bottom around the x axis.

	4. Press "e" to scale model. Press "a" or "d" key to scale the model bigger, and "w" or "s" key to scale the model smaller.
    
	5. Press "v" to change the view mode. The default view is orthographic, you can change the view mode to one-point perspective.

	6. Press "o" to restore original model and default modes.