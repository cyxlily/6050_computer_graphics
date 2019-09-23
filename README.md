# computer_graphics
common computer_graphics
> Homework of 6050 Computer Graphics

This repository contains:

1. assn1: draw lines and circles with DDA and midpoint algorithm.

## Table of Contents

- [Install](#install)
- [Usage](#usage)
	- [Generator](#generator)
- [Example Readmes](#example-readmes)

## Install

This project uses [OpenGl](https://www.opengl.org/) on Ubuntu Linux.

```sh
$ sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

## Usage

This is a C programmer. You can compile and run it as below:

```sh
$ cd assn1
$ make
$ ./assn1
```

The default mode is the "line" mode, you can draw lines between two points. Later, You can press "l" key to switch to the "line" mode.

By pressing "r" key, you can switch to the "rectangle" mode and draw rectangles. The two points represent two opposing corners of the rectangle.

Press "t" key, and you can switch to the "triangle" mode. The three mouse clicks are the vertices of the triangle.

The "e" key represent the "ellipse" mode. The two points represent the opposing corners of the rectangles that determin the ellipse.

The "c" key represent the "circle" mode. The first is the center and the next point decide the radius.

Press "d" to clear the window.


