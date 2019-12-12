6050 Computer Graphics
assn3
Yuxin Cui

Contents:

    It's a C++ program to render 3D models. It can load vertices and faces from the .obj file. A face with more than 3 sides can be divided into a set of triangles(model1_4vertex.obj). It has two type of rendering mode: flat/phong smoothing shading and bump mapping.
 

Environment:

    $ sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

Compile:

    $ make

    $ ./assn3

Usage:

    1. The model are moved to the origin, and normalized to (-1,1). The display screen is 5*5 size and z=2. The view mode is orthographic. The light sourse is at (-4,0,4). You can change the model obj file in "read_obj("model2.obj");", remember to recompile it.

    2. The default smoothing shading mode is "flat smoothing". You can see sharp borders as the model2_basic.jpg. 
    
    3. Press "x" to switch to the "phong smoothing". You can see smooth faces in the model2_task1_phong_smoothing.jpg. Later, You can press "x" again to switch  back to the "flat" mode.

    4. Press "b" to switch to "bump mapping" mode. In this mode, dimples appear in the center of each triangular face, as shown in the model2_task2_bump.jpg.