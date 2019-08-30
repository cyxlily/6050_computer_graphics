//
//		          Programming Assignment #1 
//
//			        Victor Zordan
//		
//		
//
/***************************************************************************/

                                                   /* Include needed files */
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

#define WIDTH 500
#define HEIGHT 500

int x_last,y_last;
int oldx,oldy;

/***************************************************************************/

void init_window()
                 /* Clear the image area, and set up the coordinate system */
{

        					       /* Clear the window */
        glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
        glOrtho(0,WIDTH,0,HEIGHT,-1.0,1.0);
}

/***************************************************************************/

void write_pixel(int x, int y, double intensity)
                                         /* Turn on the pixel found at x,y */
{

        glColor3f (intensity, intensity, intensity);                 
        glBegin(GL_POINTS);
           glVertex3i( x, y, 0);
        glEnd();	
}

void write_horizontal(int x, int y, double intensity)
{

        for (x=0;x<=499;x+=1)
	{
		write_pixel(x,y,intensity);
	}	
}

void write_vertical(int x, int y, double intensity)
{

        for (y=0;y<=499;y+=1)
	{
		write_pixel(x,y,intensity);
	}	
}

void write_line_DDA(int x1, int y1, int x2, int y2, double intensity)
{
	float m,x_inc,y_inc;
	int x,y;
	m=1.0*(y2-y1)/(x2-x1);
	x=x1;y=y1;
        //printf(" m is %f\n", m);
	if(abs(m)<1)
	{
		x_inc = 1;
		y_inc = m;
        	for (x=x1;x<=x2;x+=x_inc,y+=y_inc)
		{
			write_pixel((int)x,(int)y,intensity);
		}
	}
	else
	{
		x_inc = 1/m;
		y_inc = 1;
		for (y=y1;y<=y2;x+=x_inc,y+=y_inc)
		{
			write_pixel((int)x,(int)y,intensity);
		}
	}	
}

void write_line_midpoint(int x1, int y1, int x2, int y2, double intensity)
{
	float m,d;
	int x,y,a,b;
	m=1.0*(y2-y1)/(x2-x1);
	x=x1;y=y1;
	a=y1-y2;b=x2-x1;
        //printf(" m is %f\n", m);
	if(abs(m)<1)
	{
        	d=1.0*(b/2+a);
		for (x=x1;x<=x2;x++)
		{
			write_pixel(x,y,intensity);
			if(d<0)
			{
				//printf(" d is %+f\n", d);
				y++;
				d=d+a+b;
			}
			else
			{
				d=d+a;
			}
			
		}
	}
	else
	{
		d=1.0*(a/2+b);
		for (y=y1;y<=y2;y++)
		{
			write_pixel(x,y,intensity);
			if(d<0)
			{
				//printf(" d is %+f\n", d);
				d=d+b;
			}
			else
			{
				x++;
				d=d+a+b;
			}
		}
	}	
}

//***************************************************************************/

void display ( void )   // Create The Display Function
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen 

  write_pixel(x_last,y_last,1.0);//<-you can get rid of this call if you like
  // CALL YOUR CODE HERE
  //write_horizontal(x_last,y_last,0.5);
  //write_vertical(x_last,y_last,0.5);
  if(oldx!=x_last && oldy!=y_last)
  {
    //printf("display oldx,oldy is (%d,%d)\n", oldx,oldy);
    //printf("display x_last,y_last is (%d,%d)\n", x_last,y_last);
    //write_line_DDA(oldx,oldy,x_last,y_last,0.5);
    write_line_midpoint(oldx,oldy,x_last,y_last,0.5);
  }

  glutSwapBuffers();                                      // Draw Frame Buffer 
}

/***************************************************************************/
void mouse(int button, int state, int x, int y)
{
/* This function I finessed a bit, the value of the printed x,y should
   match the screen, also it remembers where the old value was to avoid multiple
   readings from the same mouse click.  This can cause problems when trying to
   start a line or curve where the last one ended */
        static int oldx = 0;
        static int oldy = 0;
	int mag;
	
	y *= -1;  //align y with mouse
	y += 500; //ignore 
	mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
	if (mag > 20) {
		//printf(" oldx,oldy is (%d,%d)\n", oldx,oldy);
		printf(" x,y is (%d,%d)\n", x,y);
	}
	oldx = x;
	oldy = y;
	x_last = x;
	y_last = y;
}
 
/***************************************************************************/
void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{

	switch ( key ) {
		case 27:              // When Escape Is Pressed...
			exit ( 0 );   // Exit The Program
			break;        
	        case '1':             // stub for new screen
		        printf("New screen\n");
			break;
		default:       
			break;
	}
}
/***************************************************************************/

int main (int argc, char *argv[])
{
/* This main function sets up the main loop of the program and continues the
   loop until the end of the data is reached.  Then the window can be closed
   using the escape key.						  */
	
	glutInit            ( &argc, argv ); 
       	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
	glutInitWindowSize  ( 500,500 ); 
	glutCreateWindow    ( "Computer Graphics" ); 
	glutDisplayFunc     ( display );  
	glutIdleFunc	    ( display );
	glutMouseFunc       ( mouse );
	glutKeyboardFunc    ( keyboard );
        					      
        init_window();				             //create_window
						       		
	glutMainLoop        ( );                 // Initialize The Main Loop
}


