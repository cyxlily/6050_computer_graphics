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

static int x_first = 0;
static int y_first = 0;
static int x_last = 0;
static int y_last = 0;
static int mouse_count = 0;

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
    float m,x_inc,y_inc,x,y;
    m=1.0*(y2-y1)/(x2-x1);
    //printf(" m is %f\n", m);
    if(abs(m)<1)
    {
        if(x2<x1)
        {
            int c = x1;
            x1 = x2;
            x2 = c;
            int d = y1;
            y1 = y2;
            y2 = d;
        } 
        y_inc = m;
        //printf(" y_inc is %f\n", y_inc);
        for (x=x1,y=y1;x<=x2;x++,y+=y_inc)
        {
            write_pixel(round(x),round(y),intensity);
        }
    }
    else
    {
        if(y2<y1)
        {
            int c = x1;
            x1 = x2;
            x2 = c;
            int d = y1;
            y1 = y2;
            y2 = d;
        }
        x_inc = 1/m;
        //printf(" y_inc is %f\n", y_inc);
        for (x=x1,y=y1;y<=y2;x+=x_inc,y++)
        {
            write_pixel(round(x),round(y),intensity);
        }
    }	
}

void write_line_midpoint(int x1, int y1, int x2, int y2, double intensity)
{
	float m,d;
	int x,y,a,b;
	m=1.0*(y2-y1)/(x2-x1);
        //printf(" m is %f\n", m);
	if(abs(m)<1)
	{
		if(x2<x1)
        	{
            		int c = x1;
            		x1 = x2;
            		x2 = c;
            		int d = y1;
            		y1 = y2;
            		y2 = d;
        	} 
		int sig_y = (y2>y1)?1:(-1);
		a=y2-y1;b=x1-x2;
        	d=1.0*(a+sig_y*b/2);
		for (x=x1,y=y1;x<=x2;x++)
		{
			write_pixel(x,y,intensity);
			if(sig_y*d>0)
			{
				//printf(" d is %+f\n", d);
				y+=sig_y;
				d=d+a+sig_y*b;
			}
			else
			{
				d=d+a;
			}
		}
	}
	else
	{
		if(y2<y1)
       		{
            		int c = x1;
            		x1 = x2;
           	 	x2 = c;
            		int d = y1;
            		y1 = y2;
            		y2 = d;
        	}
		int sig_x = (x2>x1)?1:(-1);
		a=y2-y1;b=x1-x2;
		d=1.0*(sig_x*a/2+b);
		for (x=x1,y=y1;y<=y2;y++)
		{
			write_pixel(x,y,intensity);
			if(sig_x*d>0)
			{
				//printf(" d is %+f\n", d);
				d=d+b;
			}
			else
			{
				x+=sig_x;
				d=d+sig_x*a+b;
			}
		}
	}	
}

void write_circle_midpoint(int x1, int y1, int x2, int y2, double intensity)
{
	float d,r;
	int x,y;
    r=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    //printf(" sqrt r is %f\n", sqrt(r));
    d=5/4-r;
	for (x=r,y=0;y<=r/sqrt(2);y++)
	{
		write_pixel(x1+x,y1+y,intensity);
		write_pixel(x1+y,y1+x,intensity);
		write_pixel(x1+-y,y1+x,intensity);
		write_pixel(x1+-x,y1+y,intensity);
		write_pixel(x1+-x,y1-y,intensity);
		write_pixel(x1+-y,y1-x,intensity);
		write_pixel(x1+y,y1-x,intensity);
		write_pixel(x1+x,y1-y,intensity);
		if(d<0)
		{
			d=d+2*y+3;
		}
		else
		{
			d=d+2*(y-x)+5;
			x--;
		}
	}
}

void write_ellipse_midpoint(int x1, int y1, int x2, int y2, double intensity)
{
	float d1,d2,rx,ry,xo,yo;
	int x,y;
	xo=(x1+x2)/2;
	yo=(y1+y2)/2;
	rx=abs(x2-x1)/2;
	ry=abs(y2-y1)/2;
    d1=rx*rx-rx*ry*ry+ry*ry/4;
	for(x=rx,y=0;ry*ry*x>=rx*rx*y;y++)
	{
		write_pixel(xo+x,yo+y,intensity);
		write_pixel(xo-x,yo+y,intensity);
		write_pixel(xo-x,yo-y,intensity);
		write_pixel(xo+x,yo-y,intensity);
		if(d1<0)
		{
			d1=d1+2*rx*rx*y+3*rx*rx;
		}
		else
		{
			d1=d1-2*ry*ry*x+2*ry*ry+2*rx*rx*y+3*rx*rx;
			x--;
		}
	}
	
	d2=ry*ry-ry*rx*rx+rx*rx/4;
	for(x=0,y=ry;ry*ry*x<rx*rx*y;x++)
	{
		write_pixel(xo+x,yo+y,intensity);
		write_pixel(xo-x,yo+y,intensity);
		write_pixel(xo-x,yo-y,intensity);
		write_pixel(xo+x,yo-y,intensity);
		if(d2<0)
		{
			d2=d2+2*ry*ry*x+3*ry*ry;
			
		}
		else
		{
			d2=d2-2*rx*rx*y+2*rx*rx+2*ry*ry*x+3*ry*ry;
			y--;
		}
	}
}

void write_ellipse_midpoint_test(int x1, int y1, int x2, int y2, double intensity)
{
	float d1,rx,ry,xo,yo;
	int x,y;
	xo=(x1+x2)/2;
	yo=(y1+y2)/2;
	rx=abs(x2-x1)/2;
	ry=abs(y2-y1)/2;
    d1=rx*rx-rx*ry*ry+ry*ry/4;
	x=rx;y=0;
	do
	{
		write_pixel(xo+x,yo+y,intensity);
		write_pixel(xo-x,yo+y,intensity);
		write_pixel(xo-x,yo-y,intensity);
		write_pixel(xo+x,yo-y,intensity);
		if(d1<0)
		{
			d1=d1+2*rx*rx*y+3*rx*rx;
		}
		else
		{
			d1=d1-2*ry*ry*x+2*ry*ry+2*rx*rx*y+3*rx*rx;
			x--;
		}
		y++;
	}
	while (x>=0);
}

//***************************************************************************/

void display ( void )   // Create The Display Function
{
  if(mouse_count%2==1)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen
  } 

  write_pixel(x_last,y_last,1.0);//<-you can get rid of this call if you like
  // CALL YOUR CODE HERE
  //write_horizontal(x_last,y_last,0.5);
  //write_vertical(x_last,y_last,0.5);
  if(x_first!=x_last || y_first!=y_last)
  {
    //printf(" x_first,y_first,x_last,y_last is (%d,%d,%d,%d)\n", x_first,y_first,x_last,y_last);
    //write_line_DDA(x_first,y_first,x_last,y_last,0.5);
    //write_line_midpoint(x_first,y_first,x_last,y_last,0.5);
    //write_circle_midpoint(x_first,y_first,x_last,y_last,0.5);
	write_ellipse_midpoint(x_first,y_first,x_last,y_last,0.5);
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
		mouse_count++;
		//printf(" mouse count is %d\n", mouse_count);
		if(mouse_count%2==1)
		{
			x_first = x;
			y_first = y;
		}
		x_last = x;
		y_last = y;
	}	
	oldx = x;
	oldy = y;

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


