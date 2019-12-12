# computer_graphics
common computer_graphics
> Homework of 6050 Computer Graphics

This repository contains:

1. assn1: a C program to draw colorful lines, rectangles, triangles, ellipses and circles with DDA and midpoint algorithm.

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
$ cd assn1
$ make
$ ./assn1
```

## Usage

1. The default color is white, you can choose color from the color bar at the bottom left corner of the window and draw the next shape with this color.

2. Press "l" to draw lines. The default mode is the "line" mode, you can draw lines between two click points. Later, You can press "l" key to switch to the "line" mode.

![image](https://github.com/cyxlily/computer_graphics/blob/master/assn1/line.png)

3. Press "d" to clear the window. The shapes in the current window will be cleaned.

4. Press "r" to draw rectangles. By pressing "r" key, you can switch to the "rectangle" mode and draw rectangles. The two points represent two opposing corners of the rectangle.

![image](https://github.com/cyxlily/computer_graphics/blob/master/assn1/rectangle.png)

5. Press "t" to draw triangles. Press "t" key, and you can switch to the "triangle" mode. The three mouse clicks are the vertices of the triangle.

![image](https://github.com/cyxlily/computer_graphics/blob/master/assn1/triangle.png)

6. Press "e" to draw ellipses. The "e" key represent the "ellipse" mode. The two points represent the opposing corners of the rectangles that determin the ellipse.

![image](https://github.com/cyxlily/computer_graphics/blob/master/assn1/ellipse.png)

7. Press "c" to draw circles. The "c" key represent the "circle" mode. The first is the center and the next point decide the radius.

![image](https://github.com/cyxlily/computer_graphics/blob/master/assn1/circle.png)

