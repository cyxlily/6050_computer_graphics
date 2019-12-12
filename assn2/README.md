# computer_graphics
computer_graphics
> Homework of 6050 Computer Graphics

This repository contains:

assn2: a C++ program to view and transform 3D models. It can load vertices and faces from the .obj file. It has three transformation mode: translation, rotation and scaling and two view mode: orthographic and perspective.

## Table of Contents

- [Install](#install)
- [Compile](#compile)
- [Usage](#usage)

## Install

This project uses [OpenGl](https://www.opengl.org/) on Ubuntu Linux.

```sh
$ sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```
## Compile

This is a C programmer. You can compile and run it as below:

```sh
$ cd assn2
$ make
$ ./assn2
```

## Usage

1. The model are moved to the origin, and displayed about 0.25 of the window. You can change the model obj file in "read_obj("model1.obj");", remember to recompile it.

![image](https://github.com/cyxlily/6050_computer_graphics/blob/master/assn2/line_mode.PNG)

2. The default transformation mode is "Translation". Press "a" to move to the left, "d" to move to the right, "w" to move to the top and "s" to move to the bottom. Later, You can press "t" key to switch to the "Translation" mode. The speed of the transform is 0.1 of the model.

![image](https://github.com/cyxlily/6050_computer_graphics/blob/master/assn2/translate.PNG)

3. Press "r" to rotate the model. Press "a" to rotate left around the y axis, "d" to rotate right around the y axis, "w" to rotate top around the x axis and "s" to rotate bottom around the x axis.

![image](https://github.com/cyxlily/6050_computer_graphics/blob/master/assn2/rotate.PNG)

4. Press "e" to scale model. Press "a" or "d" key to scale the model bigger, and "w" or "s" key to scale the model smaller.

![image](https://github.com/cyxlily/6050_computer_graphics/blob/master/assn2/scale.PNG)

5. Press "v" to change the view mode. The default view is orthographic, you can change the view mode to one-point perspective.

![image](https://github.com/cyxlily/6050_computer_graphics/blob/master/assn2/perspective.PNG)

6. Press "o" to restore original model and default modes.

