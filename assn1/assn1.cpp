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
#include <string.h>
#include <ctime>

#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

#define WIDTH 500
#define HEIGHT 500

static int point_buf = 2;//default 2, triangle 3
static int xpoints[3];//[0]:lastest point
static int ypoints[3];

static int mouse_count = 0;
static int mode = 1;//default draw line
static int display_flag = 1;
static float color_buf[3]={1,1,1};
static float color[3]={1,1,1};

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

void random_color()//get random color
{
	int N = 3;
	srand(time(NULL));
	color[0]=rand()%(N+1)/(float)(N+1);
	color[1]=rand()%(N+1)/(float)(N+1);
	color[2]=rand()%(N+1)/(float)(N+1);
}

void write_pixel_color(int x, int y, double c1, double c2, double c3)//draw pixel with color
                                         /* Turn on the pixel found at x,y */
{

        glColor3f (c1, c2, c3);                 
        glBegin(GL_POINTS);
           glVertex3i( x, y, 0);
        glEnd();	
}

void write_horizontal(int x, int y, double intensity)//draw horizontal line
{

    for (x=0;x<=499;x+=1)
    {
        write_pixel(x,y,intensity);
    }	
}

void write_vertical(int x, int y, double intensity)//draw vertical line
{
    for (y=0;y<=499;y+=1)
    {
        write_pixel(x,y,intensity);
    }	
}

void write_line_DDA(int x1, int y1, int x2, int y2, double intensity)//draw line between (x1,y1) and(x2,y2) with DDA algorithm
{
    float m,x_inc,y_inc,x,y;
    m=1.0*(y2-y1)/(x2-x1);//slope
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

void write_line_midpoint(int x1, int y1, int x2, int y2, double c1, double c2, double c3)//draw line with midpoint algorithm
{
	float m,d;
	int x,y,a,b;
	m=1.0*(y2-y1)/(x2-x1);//slope
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
        	} //make sure x1<x2,else exchange
		int sig_y = (y2>y1)?1:(-1);
		a=y2-y1;b=x1-x2;
        	d=1.0*(a+sig_y*b/2);
		for (x=x1,y=y1;x<=x2;x++)
		{
			write_pixel_color(x,y,c1,c2,c3);
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
        	}//make sure y1<y2,else exchange
		int sig_x = (x2>x1)?1:(-1);
		a=y2-y1;b=x1-x2;
		d=1.0*(sig_x*a/2+b);
		for (x=x1,y=y1;y<=y2;y++)
		{
			write_pixel_color(x,y,c1,c2,c3);
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

void write_rectangle(int x1, int y1, int x2, int y2, double c1, double c2, double c3)//draw rectangle
{
	int xmin,ymin,xmax,ymax,x,y;
	xmin = (x1<x2)?x1:x2;
	xmax = (x2>x1)?x2:x1;
	ymin = (y1<y2)?y1:y2;
	ymax = (y2>y1)?y2:y1;
	for (x=xmin;x<=xmax;x+=1)
    {
        write_pixel_color(x,ymin,c1,c2,c3);
		write_pixel_color(x,ymax,c1,c2,c3);
    }
	for (y=ymin;y<=ymax;y+=1)
    {
        write_pixel_color(xmin,y,c1,c2,c3);
		write_pixel_color(xmax,y,c1,c2,c3);
    }
}

void write_circle_midpoint(int x1, int y1, int x2, int y2, double c1, double c2, double c3)//draw circle with midpoint algorithm
{
	float d,r;
	int x,y;
    r=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    //printf(" sqrt r is %f\n", sqrt(r));
    d=5/4-r;
	for (x=r,y=0;y<=r/sqrt(2);y++)
	{
		write_pixel_color(x1+x,y1+y,c1,c2,c3);
		write_pixel_color(x1+y,y1+x,c1,c2,c3);
		write_pixel_color(x1+-y,y1+x,c1,c2,c3);
		write_pixel_color(x1+-x,y1+y,c1,c2,c3);
		write_pixel_color(x1+-x,y1-y,c1,c2,c3);
		write_pixel_color(x1+-y,y1-x,c1,c2,c3);
		write_pixel_color(x1+y,y1-x,c1,c2,c3);
		write_pixel_color(x1+x,y1-y,c1,c2,c3);
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

void write_ellipse_midpoint(int x1, int y1, int x2, int y2, double c1, double c2, double c3)//draw ellipse with midpoint algorithm
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
		write_pixel_color(xo+x,yo+y,c1,c2,c3);
		write_pixel_color(xo-x,yo+y,c1,c2,c3);
		write_pixel_color(xo-x,yo-y,c1,c2,c3);
		write_pixel_color(xo+x,yo-y,c1,c2,c3);
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
		write_pixel_color(xo+x,yo+y,c1,c2,c3);
		write_pixel_color(xo-x,yo+y,c1,c2,c3);
		write_pixel_color(xo-x,yo-y,c1,c2,c3);
		write_pixel_color(xo+x,yo-y,c1,c2,c3);
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

void color_bar()
{
	int x,y;
	float i,j,k;
	float color_inc = 0.1;
	int count = 0;
	y=10;
    for(i=1;i>0;i-=color_inc,y++)
	{
		x=10;
		for(j=1;j>0;j-=color_inc,x++)
		{
			for(k=1;k>0;k-=color_inc,count++,x++)
			{
				write_pixel_color(x,y,i,j,k);
				//printf("***x: %d,y: %d,c1: %f,c2: %f,c3: %f***",x,y,i,j,k);

			}
            x--;
		}
	}
	//printf("***x: %d,y: %d,c1: %f,c2: %f,c3: %f***",x,y,i,j,k);
}

//***************************************************************************/

void display ( void )   // Create The Display Function
{
	color_bar();
	if(display_flag==0){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen
		color_bar();
	}
	else{
		//write_pixel(xpoints[0],ypoints[0],1.0);//<-you can get rid of this call if you like
		// CALL YOUR CODE HERE
		//write_horizontal(x_last,y_last,0.5);
		//write_vertical(x_last,y_last,0.5);
//printf(" x_first,y_first,x_last,y_last is (%d,%d,%d,%d)\n", x_first,y_first,x_last,y_last);
		//write_line_DDA(x_first,y_first,x_last,y_last,0.5);
		switch ( mode ) {
			case 1://line
				if(xpoints[0]!=xpoints[1] || ypoints[0]!=ypoints[1])
				{
					write_line_midpoint(xpoints[1],ypoints[1],xpoints[0],ypoints[0],color[0],color[1],color[2]);
				}
				break;
			case 2://rectangle
				if(xpoints[0]!=xpoints[1] || ypoints[0]!=ypoints[1])
				{
					write_rectangle(xpoints[1],ypoints[1],xpoints[0],ypoints[0],color[0],color[1],color[2]);
				}
				break;
			case 3://triangles
				if(xpoints[2]!=xpoints[1] || ypoints[2]!=ypoints[1])
				{
					write_line_midpoint(xpoints[1],ypoints[1],xpoints[2],ypoints[2],color[0],color[1],color[2]);
				}
				if(xpoints[0]!=xpoints[1] || ypoints[0]!=ypoints[1])
				{
					write_line_midpoint(xpoints[0],ypoints[0],xpoints[1],ypoints[1],color[0],color[1],color[2]);
					write_line_midpoint(xpoints[0],ypoints[0],xpoints[2],ypoints[2],color[0],color[1],color[2]);
				}
				break;
			case 4://ellipse
				if(xpoints[0]!=xpoints[1] || ypoints[0]!=ypoints[1])
				{
					write_ellipse_midpoint(xpoints[1],ypoints[1],xpoints[0],ypoints[0],color[0],color[1],color[2]);
				}
				break;
			case 5://circle
				if(xpoints[0]!=xpoints[1] || ypoints[0]!=ypoints[1])
				{
					write_circle_midpoint(xpoints[1],ypoints[1],xpoints[0],ypoints[0],color[0],color[1],color[2]);
				}
				break;
		}

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
	int color_x1,color_x2;
	
	y *= -1;  //align y with mouse
	y += 500; //ignore 
	mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
	if (mag > 20) {
		display_flag=1;
		if(x>=10&&x<=110&&y>=10&&y<=20)//in color bar
		{
			color_buf[0]=1-1.0*(y-10)/10;
			color_x1=(x-10)%10;
			color_x2=(x-10-color_x1)/10;
			color_buf[1]=1-1.0*color_x2/10;
			color_buf[2]=1-1.0*color_x1/10;//10,1,1;11,1,0.9;20,0.9,1;21,0.9,0.9
			printf(" color is (%f,%f,%f)\n", color_buf[0],color_buf[1],color_buf[2]);
			//printf("refer x,y is (%d,%d)\n", x,y);
		}
		else
		{
			//printf(" oldx,oldy is (%d,%d)\n", oldx,oldy);
			printf(" x,y is (%d,%d)\n", x,y);
			mouse_count++;
			color[0]=color_buf[0];
			color[1]=color_buf[1];
			color[2]=color_buf[2];
			//printf(" mouse count is %d\n", mouse_count);
		
			if(point_buf==2)
			{
				if(mouse_count%2==1)
				{
					xpoints[1] = x;
					ypoints[1] = y;
				}
				xpoints[0] = x;
				ypoints[0] = y;
			}
			if(point_buf==3)
			{
				if(mouse_count%3==1)
				{
					xpoints[2] = x;
					ypoints[2] = y;
					xpoints[1] = x;
					ypoints[1] = y;
				}
				if(mouse_count%3==2)
				{
					xpoints[1] = x;
					ypoints[1] = y;
				}
				xpoints[0] = x;
				ypoints[0] = y;
			}
		}

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
			mouse_count = 0;
			break;
		case 'l'://line
			mode = 1;
			mouse_count = 0;
			point_buf = 2;
			memset(xpoints,0,3*sizeof(int));
			memset(ypoints,0,3*sizeof(int));
			//random_color();
			break;
		case 'r'://rectangle
			mode = 2;
			mouse_count = 0;
			point_buf = 2;
			memset(xpoints,0,3*sizeof(int));
			memset(ypoints,0,3*sizeof(int));
			//random_color();
			break;
		case 't'://triangle
			mode = 3;
			mouse_count = 0;
			point_buf = 3;
			memset(xpoints,0,3*sizeof(int));
			memset(ypoints,0,3*sizeof(int));
			//random_color();
			break;
		case 'e'://ellipse
			mode = 4;
			mouse_count = 0;
			point_buf = 2;
			memset(xpoints,0,3*sizeof(int));
			memset(ypoints,0,3*sizeof(int));
			//random_color();
			break;
		case 'c'://circle
			mode =5;
			mouse_count = 0;
			point_buf = 2;
			memset(xpoints,0,3*sizeof(int));
			memset(ypoints,0,3*sizeof(int));
			//random_color();
			break;
		case 'd'://clear screen
			display_flag = 0;
			mouse_count = 0;
			memset(xpoints,0,3*sizeof(int));
			memset(ypoints,0,3*sizeof(int));
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
	//random_color();
	glutDisplayFunc     ( display );  
	glutIdleFunc	    ( display );
	glutMouseFunc       ( mouse );
	glutKeyboardFunc    ( keyboard );
        					      
    init_window();				             //create_window
						       		
	glutMainLoop        ( );                 // Initialize The Main Loop
}


