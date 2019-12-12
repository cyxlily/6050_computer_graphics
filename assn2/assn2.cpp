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
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream> 
using namespace std;
#define WIDTH 500
#define HEIGHT 500
#define  GLUT_WHEEL_UP 3       
#define  GLUT_WHEEL_DOWN 4
//static int point_buf = 2;//default 2, triangle 3
//static int xpoints[3];//[0]:lastest point
//static int ypoints[3];

//static int mouse_count = 0;

//static int display_flag = 1;
//static float color_buf[3]={1,1,1};
static float color[3]={1,1,1};

/***************************************************************************/
static double bbox[6] = {0,0,0,0,0,0};//xmin,xmax,ymin,ymax,zmin,zmax
static double prop_s = 0.25;
static int v_mode = 0;//default basic viewing
static int t_mode = 1;//default translation
static int z_mode = 0;//default not z buffer
static double t_inc = 0.1;//window proportation
static double t_t[3] = {0,0,0};//translation x,y,z
static double t_r[3] = {0,0,0};//rotation degree
static double t_degree = 10;//max 360
static double pi = 3.1415926 / 180;
static double t_s = 1;//scaling


static int degree_x = 90;
static int degree_y = 90;
static int oldPosY = -1;
static int oldPosX = -1;
//***************************************************************************/
struct Homo{
    double X;
    double Y;
    double Z;
    double W;
};
struct Vertex{
	double X;
	double Y;
	double Z;
};
struct Texture{
	double TU;
	double TV;
};
struct Normal{
	double NX;
	double NY;
	double NZ;
};
struct Face{
	int V[3];
	int T[3];
	int N[3];
};
class Model
{
public:
	vector<Vertex> V;
	vector<Texture> VT;
	vector<Normal> VN;
	vector<Face> F;
	vector<Vertex> C;//key points
};
Model m_pic;

//***************************************************************************/
void read_obj(const char* filename)
{
	//printf("loading %s",filename);
	ifstream ifs(filename);//cube bunny Eight
	string s;
	Face *f;
	Vertex *v;
	Normal *vn;
	Texture *vt;
	while(getline(ifs,s))
	{
		//cout << s << endl;
		if(s.length()<2)continue;
		if(s[0]=='v'){
			if(s[1]=='t'){//vt 0.581151 0.979929 texture
				istringstream in(s);
				vt=new Texture();
				string head;
				in>>head>>vt->TU>>vt->TV;
				//printf("Texture: %f %f \n",vt->TU,vt->TV);          
				m_pic.VT.push_back(*vt);
			}else if(s[1]=='n'){//vn 0.637005 -0.0421857 0.769705 normal
				istringstream in(s);
				vn=new Normal();
				string head;
				in>>head>>vn->NX>>vn->NY>>vn->NZ;
				//printf("Normal: %f %f %f \n",vn->NX,vn->NY,vn->NZ);
				m_pic.VN.push_back(*vn);
			}else{//v -53.0413 158.84 -135.806 vertex
				istringstream in(s);
				v=new Vertex();
				string head;
				in>>head>>v->X>>v->Y>>v->Z;
				//printf("Vertex: %f %f %f \n",v->X,v->Y,v->Z);
				m_pic.V.push_back(*v);
			}
		}
		else if(s[0]=='f'){//f 2443//2656 2442//2656 2444//2656 face
			for(int k=s.size()-1;k>=0;k--){
				if(s[k]=='/')s[k]=' ';
			}
			istringstream in(s);
			f=new Face();
			string head;
			in>>head;
			int i=0;
			while(i<3)
			{
				if(m_pic.V.size()!=0)
				{
					in>>f->V[i];
					f->V[i]-=1;
				}
				if(m_pic.VT.size()!=0)
				{
					in>>f->T[i];
					f->T[i]-=1;
				}
				if(m_pic.VN.size()!=0)
				{
					in>>f->N[i];
					f->N[i]-=1;
				}
				i++;
			}
			//printf("Face: %d %d %d %d %d %d %d %d %d \n",f->V[0],f->V[1],f->V[2],f->T[0],f->T[1],f->T[2],f->N[0],f->N[1],f->N[2]);
			m_pic.F.push_back(*f);
		}
	}
	double x,y,z;
	bbox[0] = m_pic.V[0].X;
	bbox[1] = m_pic.V[0].X;
	bbox[2] = m_pic.V[0].Y;
	bbox[3] = m_pic.V[0].Y;
	bbox[4] = m_pic.V[0].Z;
	bbox[5] = m_pic.V[0].Z;
	for(int i=1;i<int(m_pic.V.size());i++)
	{
		x=m_pic.V[i].X;
		y=m_pic.V[i].Y;
		z=m_pic.V[i].Z;
		if(x < bbox[0])bbox[0]=x;//xmin
		if(x > bbox[1])bbox[1]=x;//xmax
		if(y < bbox[2])bbox[2]=y;//ymin
		if(y > bbox[3])bbox[3]=y;//ymax
		if(z < bbox[4])bbox[4]=z;//zmin
		if(z > bbox[5])bbox[5]=z;//zmax`
	}
	printf("Bounding box: xmin: %f ,xmax: %f ,ymin: %f ,ymax: %f ,zmin: %f ,zmax: %f \n", bbox[0], bbox[1], bbox[2], bbox[3], bbox[4], bbox[5]);
	v=new Vertex(); 
	v->X = (bbox[0]+bbox[1])/2;
	v->Y = (bbox[2]+bbox[3])/2;
	v->Z = (bbox[4]+bbox[5])/2;
	m_pic.C.push_back(*v);
}

/***************************************************************************/

void init_scene()
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //Background color
	glShadeModel(GL_SMOOTH);
	glOrtho(-WIDTH/2,WIDTH/2,-HEIGHT/2,HEIGHT/2,-1.0,1.0);
} 

/***************************************************************************/

void write_pixel_color(int x, int y, double c1, double c2, double c3)//draw pixel with color
                                         /* Turn on the pixel found at x,y */
{
	glColor3f (c1, c2, c3);                 
	glBegin(GL_POINTS);
	glVertex3i( x, y, 0);
	glEnd();
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

void write_triangle(int x1, int y1, int x2, int y2, int x3, int y3, double c1, double c2, double c3)
{
	write_line_midpoint(x1,y1,x2,y2,c1,c2,c3); 
	write_line_midpoint(x2,y2,x3,y3,c1,c2,c3);
	write_line_midpoint(x1,y1,x3,y3,c1,c2,c3); 
}

void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, double c1, double c2, double c3)
{
	write_line_midpoint(x1,y1,x2,y2,c1,c2,c3); 
	write_line_midpoint(x2,y2,x3,y3,c1,c2,c3);
	write_line_midpoint(x1,y1,x3,y3,c1,c2,c3); 
}

//***************************************************************************/
Homo car2homo(Vertex v)
{
	Homo h;
	h.X = v.X;
	h.Y = v.Y;
	h.Z = v.Z;
	h.W = 1;
	return h;
}

Vertex homo2car(Homo h)
{
	Vertex v;
	v.X = h.X/h.W;
	v.Y = h.Y/h.W;
	v.Z = h.Z/h.W;
	return v;
}

Homo transformation(Homo h, const double t[4][4])
{
	Homo new_h;
	new_h.X = t[0][0]* h.X + t[0][1]* h.Y + t[0][2]* h.Z + t[0][3]* h.W;
	new_h.Y = t[1][0]* h.X + t[1][1]* h.Y + t[1][2]* h.Z + t[1][3]* h.W;
	new_h.Z = t[2][0]* h.X + t[2][1]* h.Y + t[2][2]* h.Z + t[2][3]* h.W;
 	new_h.W = t[3][0]* h.X + t[3][1]* h.Y + t[3][2]* h.Z + t[3][3]* h.W; 
	return new_h;
}

Homo translation(Homo h,double dx,double dy,double dz)
{
	Homo new_h;
	double t[4][4] = {{1,0,0,dx},{0,1,0,dy},{0,0,1,dz},{0,0,0,1}};
	new_h = transformation(h,t); 
	return new_h;
}

Homo rotation(Homo h,double rdx,double rdy,double rdz)//rotate about x,y,z axis
{
	Homo h_rx,h_ry,new_h;
	//rotate
	double rx[4][4] = {{1,0,0,0},{0,cos(rdx),-sin(rdx),0},{0,sin(rdx),cos(rdx),0},{0,0,0,1}};
	double ry[4][4] = {{cos(rdy),0,sin(rdy),0},{0,1,0,0},{-sin(rdy),0,cos(rdy),0},{0,0,0,1}};
	double rz[4][4] = {{cos(rdz),-sin(rdz),0,0},{sin(rdz),cos(rdz),0,0},{0,0,1,0},{0,0,0,1}};
	h_rx = transformation(h,rx);
	h_ry = transformation(h_rx,ry);
	new_h = transformation(h_ry,rz);
	return new_h;
}

Homo scaling(Homo h,double sx,double sy,double sz)//scale about origins
{
	Homo new_h;
	//scale
	double s[4][4] = {{sx,0,0,0},{0,sy,0,0},{0,0,sz,0},{0,0,0,1}};
	new_h = transformation(h,s);
	return new_h;
}

Homo ortho(Homo h)//Orthographic
{
	double M[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,0,0},{0,0,0,1}};
	Homo new_h;
	new_h = transformation(h,M); 
	return new_h;
}

Homo persp(Homo h, double d)//scale about model center
{
	double Mp[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,-1/d,0}};
	Homo new_h;
	h = translation(h,0,0,-d-1);
	new_h = transformation(h,Mp); 
	//new_h = translation(h,0,0,d+1);
	return new_h;
}

Vertex trans_point(Vertex v)
{
	Homo h;
	Vertex new_v;
	h = car2homo(v);
	//move to origin
	h = translation(h,-(bbox[0]+bbox[1])/2,-(bbox[2]+bbox[3])/2,-(bbox[4]+bbox[5])/2); 
	//normalize to 1
	h = scaling(h,2/(bbox[1]-bbox[0]),2/(bbox[3]-bbox[2]),2/(bbox[5]-bbox[4])); 
	
	//scale
	h = scaling(h,t_s,t_s,t_s);
	//rotate
	h = rotation(h,t_r[0],t_r[1],t_r[2]);
	//translation
	h = translation(h,t_t[0],t_t[1],t_t[2]);
	
	//view
	//ortho
	//view
	if(v_mode==0)h = ortho(h);
	else if(v_mode==1)h = persp(h, 8);
	
	//fit window to display
	h = scaling(h,prop_s*WIDTH/2,prop_s*HEIGHT/2,1);
	
	new_v = homo2car(h);
	return new_v;
}

//***************************************************************************/

void draw_coordinates(void) { 
    write_pixel_color(0,0,1,0,0); 
    write_circle_midpoint(0, 0, 3, 0, 1, 0, 0);
}

void draw_obj()
{
	Vertex v1,v2,v3;
	//Vertex v0
	//v0 = trans_point(m_pic.C[0]);
	//write_circle_midpoint(v0.X,v0.Y,v0.X+2,v0.Y,0,1,0);//green
	for(int i=0;i<int(m_pic.F.size());i++)
	{
		v1 = trans_point(m_pic.V[m_pic.F[i].V[0]]);
		//write_circle_midpoint(v1.X,v1.Y,v1.X+3,v1.Y,0,0,1);//blue vertex
		
		v2 = trans_point(m_pic.V[m_pic.F[i].V[1]]);
		//write_circle_midpoint(v2.X,v2.Y,v2.X+3,v2.Y,0,0,1);
		
		v3 = trans_point(m_pic.V[m_pic.F[i].V[2]]);
		//write_circle_midpoint(v3.X,v3.Y,v3.X+3,v3.Y,0,0,1);
		if(z_mode==0)write_triangle(v1.X,v1.Y,v2.X,v2.Y,v3.X,v3.Y,color[0], color[1], color[2]);//white line
		else if(z_mode == 1)fill_triangle(v1.X,v1.Y,v2.X,v2.Y,v3.X,v3.Y,color[0], color[1], color[2]);//draw color
	}
}

void display_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
	draw_obj();
	//draw_coordinates();    
	glutSwapBuffers();
}

/***************************************************************************/
 
void moseMove(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        oldPosX = x; oldPosY = y;
    }
    else if(state == GLUT_UP) {
        if (state == GLUT_UP && button == GLUT_WHEEL_UP) {
            t_s += 0.1;
        }
        if (state == GLUT_UP && button == GLUT_WHEEL_DOWN) {
            t_s -= 0.1;
        }
    }
}
void changeViewPoint(int x, int y)
{
    int temp_x = x - oldPosX;
    int temp_y = y - oldPosY;
    degree_x += temp_x;
    degree_y +=temp_y;
    oldPosX = x;
    oldPosY = y;
}

void myIdle()
{
    glutPostRedisplay();
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
		case 't'://Translation
			t_mode = 1;
			break;
		case 'r'://Rotation
			t_mode = 2;
			break;
		case 'e'://scalE
			t_mode = 3;
			break;
		case 'a':
			if(t_mode == 1)t_t[0] -= t_inc;//Translation: left
			else if(t_mode == 2)t_r[1] -= t_degree*pi;//Rotation: y-
			else if(t_mode == 3)t_s += t_inc;//Scaling: +
			break;
		case 'w':
			if(t_mode == 1)t_t[1] += t_inc;//Translation: up
			else if(t_mode == 2)t_r[0] += t_degree*pi;//Rotation: x+
			else if(t_mode == 3)t_s -= t_inc;//Scaling: -
			break;
		case 's':
			if(t_mode == 1)t_t[1] -= t_inc;//Translation: down
			else if(t_mode == 2)t_r[0] -= t_degree*pi;//Rotation: x-
			else if(t_mode == 3)t_s -= t_inc;//Scaling: -
			break;
		case 'd':
			if(t_mode == 1)t_t[0] += t_inc;//Translation: right
			else if(t_mode == 2)t_r[1] += t_degree*pi;//Rotation: y+
			else if(t_mode == 3)t_s += t_inc;//Scaling: +
			break;
		case 'v'://view
			if(v_mode == 1)v_mode=0;//change to ortho
			else if(v_mode == 0)v_mode=1;//change to perspective
			break;
		case 'z'://z buffer
			if(z_mode == 1)z_mode=0;//change to only line
			else if(z_mode == 0)z_mode=1;//change to z buffer with colored faces
			break;
		case 'o'://return to orginal model
			t_mode = 1;
			v_mode = 0;
			z_mode = 0;
			t_t[0] = 0;t_t[1] = 0;t_t[2] = 0;//translation x,y,z
			t_r[0] = 0;t_r[1] = 0;t_r[2] = 0;//rotation degree
			t_s = 1;
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
	read_obj("model1.obj");
	
	glutInit( &argc, argv ); 
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
	glutInitWindowSize( 500,500 );
	//glutInitWindowPosition(0, 0);
	//glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow( "Computer Graphics" ); 
	init_scene(); 
	glutDisplayFunc(display_scene);  
	//glutReshapeFunc(reshape);
	//glutMouseFunc(moseMove);
	//glutMotionFunc(changeViewPoint);
	glutKeyboardFunc( keyboard );
	glutIdleFunc(myIdle);
	glutMainLoop( );                 // Initialize The Main Loop
}


