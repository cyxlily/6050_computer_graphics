//
//          Programming Assignment #1 
//
//        Victor Zordan
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
#include <map>
using namespace std;
#define WIDTH 500
#define HEIGHT 500
#define  GLUT_WHEEL_UP 3       
#define  GLUT_WHEEL_DOWN 4

/***************************************************************************/
static double pi = 3.1415926 / 180;
static double bbox[6] = {0,0,0,0,0,0};//xmin,xmax,ymin,ymax,zmin,zmax

//static double prop_s = 0.25;
static int smooth_mode = 0;//default flat smoothing mode,1 change to phong smoothing mode
static int bump_mode = 0;//default normal mapping,1 change to bump mapping
static int v_mode = 0;//default basic viewing
static int t_mode = 2;//default rotate
static int z_mode = 0;//default not z buffer
static double t_inc = 0.1;//window proportation
static double t_t[3] = {0,0,0};//translation x,y,z
static double t_r[3] = {0,0,0};//rotation degree
static double t_degree = 10;//max 360
static double t_s = 1;//scaling
static int face_num = 0;
static int vertex_num = 0;

static double bump_gradient = 2;

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
    //int V[3];
    //int T[3];
    //int N[3];
    vector<int> Vi;
    vector<int> Ti;
    vector<int> Ni;
};
class Model
{
public:
    vector<Vertex> V;
    vector<Vertex> C;//key points,center,min,max
    vector<Texture> VT;
    vector<Normal> VN;
    vector<Face> F;//vertex index
    
    vector<Face> F3;//each face has three vertex
    vector<Vertex> Vt;//trans vertex
    vector<Vertex> Ct;//trans key points
    vector<Vertex> Vn;//vertex normal
    vector<Vertex> Fn;//face normal
    vector<Vertex> Fc;//face center
};

struct Pixel{
    Vertex V;//normal coor
    double Z;//z_buffer
    double R;
    double G;
    double B;
    int Fi;//face index
    Vertex I;//intersection
    Vertex N;//normal
};
Model m_pic;

static Pixel all_screen[500][500];
//eye point
static double eye_z = 9;
//display_window
static double display_z = 2;//1
static double d = eye_z-display_z;

static double background_color[3] = {0,0,0};
static double ck[3][3] = {{0.5f,0.5f,0.5f},{1,1,1},{0.5f,0.5f,0.5f}};//kar,kag,kab;kdr,kdg,kdb;ksr,ksg,ksb
static int shininess_a = 10;
static double light_pos[2][3] = {{-4,0,4},{4,0,0}};//use first light sourse
static double cl[2][3][3] = {{{0.5,0,0},{1,1,1},{0.5,0,0}},{{0,0,0.5},{1,1,1},{0,0,0.5}}};//lar,lag,lab;ldr,ldg,ldb;lsr,lsg,lsb

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
                m_pic.Vt.push_back(*v);//init Vertex trans list
                m_pic.Vn.push_back(*v);//init Vertex normal list
            }
        }
        else if(s[0]=='f'){//f 1/1/1 2/2/2 3/3/3 face
            for(int k=s.size()-1;k>=0;k--){
                if(s[k]=='/')s[k]=' ';
            }
            istringstream in(s);
            f=new Face();
            string head;
            in>>head;
            //int i=0;
            int vi,ti,ni;
            while(in>>vi>>ti>>ni)
            {
                //cout<<vi<<ti<<ni<<endl;
                if(m_pic.V.size()!=0)
                {
                    vi-=1;
                    f->Vi.push_back(vi);
                }
                if(m_pic.VT.size()!=0)
                {
                    ti-=1;
                    f->Ti.push_back(ti);
                }
                if(m_pic.VN.size()!=0)
                {
                    ni-=1;
                    f->Ni.push_back(ni);
                }
                
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
    v=new Vertex(); //centre point
    v->X = (bbox[0]+bbox[1])/2;
    v->Y = (bbox[2]+bbox[3])/2;
    v->Z = (bbox[4]+bbox[5])/2;
    m_pic.C.push_back(*v);
    m_pic.Ct.push_back(*v);//init trans key point
    
    v = new Vertex();
    v->X = bbox[0];
    v->Y = bbox[2];
    v->Z = bbox[4];
    m_pic.C.push_back(*v);
    m_pic.Ct.push_back(*v);//init trans key point
    
    v = new Vertex();
    v->X = bbox[1];
    v->Y = bbox[3];
    v->Z = bbox[5];
    m_pic.C.push_back(*v);
    m_pic.Ct.push_back(*v);//init trans key point
    
    
    face_num = int(m_pic.F.size());
    printf("face_num: %d \n",face_num);//12
    vertex_num = int(m_pic.V.size());
    printf("vertex_num: %d \n",vertex_num);//8
}

void triangle_face()
{
    Face *f3;
    Vertex v0;
    v0.X = 0;
    v0.Y = 0;
    v0.Z = 0;
    int f_v_num=0;
    int vi,ti,ni,v1,t1,n1,v2,t2,n2,v3,t3,n3;
    for(int i=0;i<face_num;i++)
    {
        f_v_num = m_pic.F[i].Vi.size();
        //cout<<"face_vertex_number: "<<f_v_num<<endl;
        f3=new Face();
        for(int k=0;k<3;k++)//copy first three
        {
            vi = m_pic.F[i].Vi[k];
            ti = m_pic.F[i].Ti[k];
            ni = m_pic.F[i].Ni[k];
            //cout<<vi<<ti<<ni<<endl;
            f3->Vi.push_back(vi);
            f3->Ti.push_back(ti);
            f3->Ni.push_back(ni);
            //cout<<"copy face"<<endl;
        }
        m_pic.F3.push_back(*f3);
        m_pic.Fn.push_back(v0);//init Face normal list
        m_pic.Fc.push_back(v0);//init Face center list
        
        if(f_v_num>3)
        {
            //cout<<"add face: "<<endl;
            v1 = m_pic.F[i].Vi[0];
            t1 = m_pic.F[i].Ti[0];
            n1 = m_pic.F[i].Ni[0];
            v2 = m_pic.F[i].Vi[2];
            t2 = m_pic.F[i].Ti[2];
            n2 = m_pic.F[i].Ni[2];
            for(int j=3;j<f_v_num;j++)//add face
            {
                f3=new Face();
                v3 = m_pic.F[i].Vi[j];
                t3 = m_pic.F[i].Ti[j];
                n3 = m_pic.F[i].Ni[j];
                
                f3->Vi.push_back(v1);
                f3->Ti.push_back(t1);
                f3->Ni.push_back(n1);
                f3->Vi.push_back(v2);
                f3->Ti.push_back(t2);
                f3->Ni.push_back(n2);
                f3->Vi.push_back(v3);
                f3->Ti.push_back(t3);
                f3->Ni.push_back(n3);
                m_pic.F3.push_back(*f3);
                m_pic.Fn.push_back(v0);//init Face normal list
                m_pic.Fc.push_back(v0);//init Face center list
                
                v2 = v3;
                t2 = t3;
                n2 = n3;
            }
        }
    }
    face_num = int(m_pic.F3.size());
    printf("triangle_face_num: %d \n",face_num);//12 
}
/***************************************************************************/

void init_scene()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); //Background color
    glShadeModel(GL_SMOOTH);
    glOrtho(0,WIDTH,0,HEIGHT,-1.0,1.0);
} 

void init_pixel()//5*5screen,z=1
{
    for(int i1=0;i1<HEIGHT;i1++)//init
    {
        for(int i2=0;i2<WIDTH;i2++)
        {
            all_screen[i1][i2].V.X = double(i2)/50-5;
            all_screen[i1][i2].V.Y = double(i1)/50-5;
            all_screen[i1][i2].V.Z = display_z;
            all_screen[i1][i2].Z = -10000;//z-buffer
            all_screen[i1][i2].R = background_color[0];
            all_screen[i1][i2].G = background_color[1];
            all_screen[i1][i2].B = background_color[2];
            
            all_screen[i1][i2].Fi = -1;//face index 
        }
    } 
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

Homo persp(Homo h)//scale about model center
{
    Homo new_h;
    h = translation(h,0,0,-eye_z);
    double Mp[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,-1/d,0}};
    new_h = transformation(h,Mp); 
    //new_h = translation(h,0,0,d+1);
    return new_h;
}

Vertex normalize_point(Vertex v)
{
    Homo h;
    Vertex new_v;
    h = car2homo(v);
    //move to origin
    h = translation(h,-(bbox[0]+bbox[1])/2,-(bbox[2]+bbox[3])/2,-(bbox[4]+bbox[5])/2); 
    //normalize to 1
    h = scaling(h,2/(bbox[1]-bbox[0]),2/(bbox[3]-bbox[2]),2/(bbox[5]-bbox[4])); 
    new_v = homo2car(h);
    return new_v;
}

Vertex trans_point(Vertex v)
{
    Homo h;
    Vertex new_v;
    h = car2homo(v);

    //scale
    h = scaling(h,t_s,t_s,t_s);
    //rotate
    h = rotation(h,t_r[0],t_r[1],t_r[2]);
    //translation
    h = translation(h,t_t[0],t_t[1],t_t[2]);

    //view
    //ortho
    //view
    //if(v_mode==0)h = ortho(h);
    //else if(v_mode==1)h = persp(h, 8);

    //fit window to display
    //h = scaling(h,prop_s*WIDTH/2,prop_s*HEIGHT/2,1);

    new_v = homo2car(h);
    return new_v;
}

Vertex trans_point_test(Vertex v)
{
    Homo h;
    Vertex new_v;
    h = car2homo(v);

    //scale
    h = scaling(h,t_s,t_s,t_s);
    //rotate
    h = rotation(h,t_r[0],t_r[1],t_r[2]);
    //h = rotation(h,45*pi,45*pi,t_r[2]);
    //translation
    h = translation(h,t_t[0],t_t[1],t_t[2]);

    new_v = homo2car(h);
    return new_v;
}

void normalize_obj()
{
    Vertex v;
    //normalize vertex
    for(int i=0;i<vertex_num;i++)
    {
        v = normalize_point(m_pic.V[i]);
        m_pic.V[i] = v;//update vertex
        //printf("Normal obj: v_point: x: %f,y: %f,z: %f\n",m_pic.V[i].X,m_pic.V[i].Y,m_pic.V[i].Z);
    }
    
    //normalize key point
    Vertex cen_v,min_v,max_v;
    cen_v = normalize_point(m_pic.C[0]);
    m_pic.C[0] = cen_v;
    min_v = normalize_point(m_pic.C[1]);
    m_pic.C[1] = min_v;
    max_v = normalize_point(m_pic.C[2]);
    m_pic.C[2] = max_v;
    printf("Normalize obj: cen_point: x: %f,y: %f,z: %f, min_point: x: %f,y: %f,z: %f, max_point: x: %f,y: %f,z: %f \n",cen_v.X,cen_v.Y,cen_v.Z,min_v.X,min_v.Y,min_v.Z,max_v.X,max_v.Y,max_v.Z);
    
}

void trans_obj()
{
    Vertex v;
    //trans vertex
    for(int i=0;i<vertex_num;i++)
    {
        v = trans_point(m_pic.V[i]);
        m_pic.Vt[i] = v;//update vertex
        //printf("trans obj: v_point: x: %f,y: %f,z: %f\n",m_pic.Vt[i].X,m_pic.Vt[i].Y,m_pic.Vt[i].Z);
    }
    /*
    //trans key point
    Vertex cen_v,min_v,max_v;
    cen_v = trans_point(m_pic.C[0]);
    m_pic.Ct[0] = cen_v;
    min_v = trans_point(m_pic.C[1]);
    m_pic.Ct[1] = min_v;
    max_v = trans_point(m_pic.C[2]);
    m_pic.Ct[2] = max_v;
    printf("Trans obj: cen_point: x: %f,y: %f,z: %f, min_point: x: %f,y: %f,z: %f, max_point: x: %f,y: %f,z: %f \n",cen_v.X,cen_v.Y,cen_v.Z,min_v.X,min_v.Y,min_v.Z,max_v.X,max_v.Y,max_v.Z);
    */
}

void test_obj()
{
    //printf("start test object! \n");
    Vertex v;
    //trans vertex
    for(int i=0;i<vertex_num;i++)
    {
        //printf("after normalize obj: v_point: x: %f,y: %f,z: %f\n",m_pic.V[i].X,m_pic.V[i].Y,m_pic.V[i].Z);
        v = trans_point_test(m_pic.V[i]);
        //printf("after trans obj: v_point: x: %f,y: %f,z: %f\n",v.X,v.Y,v.Z);
        m_pic.Vt[i] = v;//update vertex
        //printf("stroe trans obj: v_point: x: %f,y: %f,z: %f\n",m_pic.Vt[i].X,m_pic.Vt[i].Y,m_pic.Vt[i].Z);
    }
    /*
    //trans key point
    Vertex cen_v,min_v,max_v;
    cen_v = trans_point_test(m_pic.C[0]);
    m_pic.Ct[0] = cen_v;
    min_v = trans_point_test(m_pic.C[1]);
    m_pic.Ct[1] = min_v;
    max_v = trans_point_test(m_pic.C[2]);
    m_pic.Ct[2] = max_v;
    printf("Trans obj: cen_point: x: %f,y: %f,z: %f, min_point: x: %f,y: %f,z: %f, max_point: x: %f,y: %f,z: %f \n",cen_v.X,cen_v.Y,cen_v.Z,min_v.X,min_v.Y,min_v.Z,max_v.X,max_v.Y,max_v.Z);
    */
}
//***************************************************************************/
//vertex opertator
int v_isequal(Vertex v1,Vertex v2)
{
    if(abs(v1.X-v2.X)<1e-6  && abs(v1.Y-v2.Y)<1e-6 && abs(v1.Z-v2.Z)<1e-6) return 1;
    else return 0;
}
double v_abs(Vertex v)
{
    double new_result;
    new_result = sqrt(pow(v.X,2)+pow(v.Y,2)+pow(v.Z,2));
    return new_result;
}
Vertex v_normalize(Vertex v)
{
    Vertex new_v;
    double r;
    r = v_abs(v);
    new_v.X = v.X/r;
    new_v.Y = v.Y/r;
    new_v.Z = v.Z/r;
    return new_v;
}
Vertex v_minus(Vertex v1,Vertex v2)
{
    Vertex new_v;
    new_v.X = v1.X-v2.X;
    new_v.Y = v1.Y-v2.Y;
    new_v.Z = v1.Z-v2.Z;
    return new_v;
}

Vertex v_add(Vertex v1,Vertex v2)
{
    Vertex new_v;
    new_v.X = v1.X+v2.X;
    new_v.Y = v1.Y+v2.Y;
    new_v.Z = v1.Z+v2.Z;
    return new_v;
}

Vertex v_add3(Vertex v1,Vertex v2,Vertex v3)
{
    Vertex new_v;
    new_v.X = v1.X+v2.X+v3.X;
    new_v.Y = v1.Y+v2.Y+v3.Y;
    new_v.Z = v1.Z+v2.Z+v3.Z;
    return new_v;
}

Vertex v_numbermult(Vertex v1,double n)
{
    Vertex new_v;
    new_v.X = v1.X*n;
    new_v.Y = v1.Y*n;
    new_v.Z = v1.Z*n;
    return new_v;
}

Vertex v_numberdiv(Vertex v1,double n)
{
    Vertex new_v;
    new_v.X = v1.X/n;
    new_v.Y = v1.Y/n;
    new_v.Z = v1.Z/n;
    return new_v;
}

double v_dot(Vertex v1,Vertex v2)
{
    double new_result;
    new_result = v1.X*v2.X+v1.Y*v2.Y+v1.Z*v2.Z;
    return new_result;
}

Vertex v_cross(Vertex v1,Vertex v2)//calculate face normal
{
    Vertex new_v;
    new_v.X = v1.Y*v2.Z-v1.Z*v2.Y;
    new_v.Y = v1.Z*v2.X-v1.X*v2.Z;
    new_v.Z = v1.X*v2.Y-v2.X*v1.Y;
    return new_v;
}

Vertex v_halfway(Vertex v1,Vertex v2)
{
    Vertex new_v1,new_v;
    new_v1 = v_add(v1,v2);
    new_v = v_normalize(new_v1);    
    return new_v;
}

double v_det(double m[3][3])
{
    double new_result;
    new_result = m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][0]+m[0][2]*m[1][0]*m[2][1]-m[2][0]*m[1][1]*m[0][2]-m[1][0]*m[0][1]*m[2][2]-m[0][0]*m[2][1]*m[1][2];
    return new_result;
}

Vertex v_hit(Vertex eb,Vertex ec,Vertex rd,Vertex ao)
{
    Vertex new_result;//t,b,r
    double T[3][3] = {{-eb.X,-ec.X,ao.X},{-eb.Y,-ec.Y,ao.Y},{-eb.Z,-ec.Z,ao.Z}};
    double A[3][3] = {{-eb.X,-ec.X,rd.X},{-eb.Y,-ec.Y,rd.Y},{-eb.Z,-ec.Z,rd.Z}};  
    double B[3][3] = {{ao.X,-ec.X,rd.X},{ao.Y,-ec.Y,rd.Y},{ao.Z,-ec.Z,rd.Z}};  
    double C[3][3] = {{-eb.X,ao.X,rd.X},{-eb.Y,ao.Y,rd.Y},{-eb.Z,ao.Z,rd.Z}};  
    new_result.X = v_det(T)/v_det(A);
    new_result.Y = v_det(B)/v_det(A);
    new_result.Z = v_det(C)/v_det(A);
    return new_result;
}

Vertex v_getintersection(Vertex ro,Vertex rd,double t)
{
    Vertex r1,new_v;
    r1 = v_numbermult(rd,t);
    new_v = v_add(ro,r1);
    return new_v;
}
//***************************************************************************/
void update_normal()
{
    //printf("start update normal!\n");
    //computer normal for each face
    Vertex a,b,c,eb_v,ec_v,n_v;
    for(int i=0;i<face_num;i++)//normalized normal
    {
        a = m_pic.Vt[m_pic.F3[i].Vi[0]];
        b = m_pic.Vt[m_pic.F3[i].Vi[1]];
        c = m_pic.Vt[m_pic.F3[i].Vi[2]];
        eb_v = v_minus(b,a);
        ec_v = v_minus(c,a);
        n_v = v_normalize(v_cross(eb_v,ec_v));
        if(n_v.Z<0) n_v = v_numbermult(n_v,-1);//if necessary???????????????????
        m_pic.Fn[i] = n_v;
        //printf("%d face normal,x: %f,y:%f,z: %f \n",i,m_pic.Fn[i].X,m_pic.Fn[i].Y,m_pic.Fn[i].Z);
    }
    
    //computer center for each face
    Vertex cen_v;
    for(int i=0;i<face_num;i++)//normalized normal
    {
        a = m_pic.Vt[m_pic.F3[i].Vi[0]];
        b = m_pic.Vt[m_pic.F3[i].Vi[1]];
        c = m_pic.Vt[m_pic.F3[i].Vi[2]];
        eb_v = v_minus(b,a);
        ec_v = v_minus(c,a);
        double perc = 1.0/3.0;
        cen_v = v_numbermult(v_add3(a,b,c),perc);
        m_pic.Fc[i] = cen_v;
        //printf("%d face center,x: %f,y:%f,z: %f \n",i,m_pic.Fc[i].X,m_pic.Fc[i].Y,m_pic.Fc[i].Z);
    }
    
    
    //computer normal for each vertex
    int vertex_index[vertex_num][face_num];
    for(int i=0;i<vertex_num;i++)
    {
        for(int j=0;j<face_num;j++)
        {
            vertex_index[i][j] = -1;
        }
    }
    
    Vertex tmp_normal,face_normal,list_normal;
    int index;
    int duplicate_flag=0;
    for(int i=0;i<vertex_num;i++)
    {
        //printf("%d vertex point,x: %f,y:%f,z: %f \n",i,m_pic.Vt[i].X,m_pic.Vt[i].Y,m_pic.Vt[i].Z);
        tmp_normal.X = 0;
        tmp_normal.Y = 0;
        tmp_normal.Z = 0;
        for(int j=0;j<face_num;j++)
        {
            for(int k=0;k<3;k++)
            {
                index = m_pic.F3[j].Vi[k];
                if(i==index)//vertex have this face!
                {
                    //printf("belongs to face %d \n",j);
                    face_normal = m_pic.Fn[j];
                    duplicate_flag = 0;
                    for(int h=0;h<face_num;h++)
                    {
                        if(vertex_index[i][h]!=-1)//list exist face h
                        {
                            list_normal = m_pic.Fn[h];
                            if(v_isequal(face_normal,list_normal))
                            {
                                //printf("face dulpicate!face %d is same to face %d \n",j,h);
                                duplicate_flag=1;
                                break;
                            }
                        }
                    }
                    if(duplicate_flag==0)//not duplicate
                    {
                        //printf("unique face,add normal for face %d! \n",j);
                        tmp_normal = v_add(tmp_normal,face_normal);
                        vertex_index[i][j] = j;
                    }
                    break;//one face at most one vertex
                } 
            }
        }
        tmp_normal = v_normalize(tmp_normal);
        m_pic.Vn[i] = tmp_normal;
        //printf("%d vertex normal,x: %f,y:%f,z: %f \n",i,tmp_normal.X,tmp_normal.Y,tmp_normal.Z);
    }
}

Vertex phong_smooth(int f_index,double b,double r)
{
    Vertex A,B,C,na,nb,nc,new_n,nab,nac;
    A = m_pic.V[m_pic.F3[f_index].Vi[0]];
    B = m_pic.V[m_pic.F3[f_index].Vi[1]];
    C = m_pic.V[m_pic.F3[f_index].Vi[2]];
    //printf("vertex1 pos: x: %f,y: %f,z: %f \n",A.X,A.Y,A.Z);
    //printf("vertex2 pos: x: %f,y: %f,z: %f \n",B.X,B.Y,B.Z);
    //printf("vertex3 pos: x: %f,y: %f,z: %f \n",C.X,C.Y,C.Z);
    na = m_pic.Vn[m_pic.F3[f_index].Vi[0]];
    //printf("vertex1 normal: x: %f,y: %f,z: %f \n",na.X,na.Y,na.Z);
    nb = m_pic.Vn[m_pic.F3[f_index].Vi[1]];
    //printf("vertex2 normal: x: %f,y: %f,z: %f \n",nb.X,nb.Y,nb.Z);
    nc = m_pic.Vn[m_pic.F3[f_index].Vi[2]];
    //printf("vertex3 normal: x: %f,y: %f,z: %f \n",nc.X,nc.Y,nc.Z);
    nab = v_minus(nb,na);
    nac = v_minus(nc,na);
    new_n = v_normalize(v_add3(na,v_numbermult(nab,b),v_numbermult(nac,r)));
    return new_n;
}



double blinn_phong_illum(Vertex i,Vertex n,Vertex v,double ka,double la,double kd,double ld,double ks,double ls,double a)
{
    double l;
    Vertex h;
    h = v_halfway(i,v);
    l = kd*ld*v_dot(i,n)+ks*ls*pow(v_dot(h,n),a)+ka*la;
    return l;
}

Vertex comp_bump(Vertex n, int index,Vertex pos)
{
    Vertex new_v,cen_v,d;
    double distance;
    cen_v = m_pic.Fc[index];
    //printf("vertex pos: x: %f,y: %f,z: %f \n",pos.X,pos.Y,pos.Z);
    //printf("center pos: x: %f,y: %f,z: %f \n",cen_v.X,cen_v.Y,cen_v.Z);
    
    distance = v_abs(v_minus(cen_v,pos));
    //printf("distance: %f \n",distance);
    if(distance<0.1)
    {
        d = v_minus(pos,cen_v);
        new_v = v_minus(n,v_numbermult(d,bump_gradient));
    
        new_v = v_normalize(new_v);
    }
    else new_v = n;
    return new_v;
}


//***************************************************************************/

void ray_tracing()//ray tracing
{
    //printf("trans obj:point0: x: %f,y: %f,z: %f \n",all_v[0][0].X,all_v[0][0].Y,all_v[0][0].Z);
    Vertex d_v;
    d_v.X = 0;
    d_v.Y = 0;
    d_v.Z = -1;
    
    Vertex view_v;
    view_v.X = 0;
    view_v.Y = 0;
    view_v.Z = 1;
    
    Vertex all_light[2];//two light source
    for(int t0=0;t0<2;t0++)
    {
        all_light[t0].X = light_pos[t0][0];
        all_light[t0].Y = light_pos[t0][1];
        all_light[t0].Z = light_pos[t0][2];
    }
    Vertex light_v;
    
    Vertex A1,B1,C1,e_v,eb_v,ec_v,ao_v,A2,B2,C2,eb_v2,ec_v2,ao_v2,i_v,shadow_v,n_v;
    double t1,b1,r1,t2,b2,r2,hit_flag2;
    hit_flag2 = 0;
    for(int i1=0;i1<500;i1++)//init
    {
        for(int i2=0;i2<500;i2++)
        {
            e_v.X = all_screen[i1][i2].V.X;//parallel
            e_v.Y = all_screen[i1][i2].V.Y;
            e_v.Z = eye_z;
            for(int i3=0;i3<face_num;i3++)
            {
                //cout<<"check face index: "<<i3<<endl;
                A1 = m_pic.Vt[m_pic.F3[i3].Vi[0]];
                B1 = m_pic.Vt[m_pic.F3[i3].Vi[1]];
                C1 = m_pic.Vt[m_pic.F3[i3].Vi[2]];
                //cout<<"A1: "<<A1.X<<","<<A1.Y<<","<<A1.Z<<endl;
                //cout<<"B1: "<<B1.X<<","<<B1.Y<<","<<B1.Z<<endl;
                //cout<<"C1: "<<C1.X<<","<<C1.Y<<","<<C1.Z<<endl;
                
                eb_v = v_minus(B1,A1);//b-a
                ec_v = v_minus(C1,A1);//c-a
                ao_v = v_minus(A1,e_v);//a-ro
                t1 = v_hit(eb_v,ec_v,d_v,ao_v).X;
                b1 = v_hit(eb_v,ec_v,d_v,ao_v).Y;
                r1 = v_hit(eb_v,ec_v,d_v,ao_v).Z;
                //cout<<"t1: "<<t1<<endl;
                //cout<<"b1: "<<b1<<endl;
                //cout<<"r1: "<<r1<<endl;
                if(t1>0 && b1>=0 && r1>=0 && b1+r1<=1)//hit
                {
                    i_v = v_getintersection(e_v,d_v,t1);
                    if(i_v.Z > all_screen[i1][i2].Z)
                    {
                        //printf("find intersection:face_index: %d, inter_x: %f,inter_y: %f,inter_z: %f \n",i3,i_v.X,i_v.Y,i_v.Z);
                        if(smooth_mode == 0)n_v = m_pic.Fn[i3];
                        //n_v = m_pic.Fn[i3];
                        //printf("flat_smooth normal: x: %f,y: %f,z: %f \n",n_v.X,n_v.Y,n_v.Z);
                        else n_v = phong_smooth(i3,b1,r1);
                        
                        //n_v = phong_smooth(i3,b1,r1);
                        //printf("phong_smooth normal: x: %f,y: %f,z: %f \n",n_v.X,n_v.Y,n_v.Z);
                        //printf("origin normal: x: %f,y: %f,z: %f \n",n_v.X,n_v.Y,n_v.Z);
                        if(bump_mode == 1)n_v = comp_bump(n_v,i3,i_v);
                        //printf("new normal: x: %f,y: %f,z: %f \n",n_v.X,n_v.Y,n_v.Z);
                        
                        all_screen[i1][i2].Z = i_v.Z;
                        all_screen[i1][i2].R = 0;
                        all_screen[i1][i2].G = 0;
                        all_screen[i1][i2].B = 0;
                        
                        all_screen[i1][i2].Fi = i3;//face index
                        all_screen[i1][i2].I = i_v;//intersection
                        all_screen[i1][i2].N = n_v;//normal
                    }
                }
            }
        }
    }//find nearst face
    //cout<<"start rendering!"<<endl;
    int i3 = 0;
    for(int i1=0;i1<500;i1++)//init
    {
        for(int i2=0;i2<500;i2++)
        {           
            if(all_screen[i1][i2].Fi!=-1)//see face
            { 
                //cout<<"see face!!"<<endl;
                i_v = all_screen[i1][i2].I;
                i3 = all_screen[i1][i2].Fi;
                n_v = all_screen[i1][i2].N;
                //printf("normal: x: %f,y: %f,z: %f \n",n_v.X,n_v.Y,n_v.Z);
                for(int i4=0;i4<1;i4++)//for each light
                {
                    //cout<<"light: "<<i4<<endl;
                    light_v = all_light[i4]; 
                    shadow_v = v_normalize(v_minus(light_v,i_v));
                    hit_flag2 = 0;
                    for(int i5=0;i5<face_num;i5++)
                    {
                        //cout<<"shadow ray: check face index: "<<i5<<endl;
                        if(i5!=i3)
                        {
                            A2 = m_pic.Vt[m_pic.F3[i5].Vi[0]];
                            B2 = m_pic.Vt[m_pic.F3[i5].Vi[1]];
                            C2 = m_pic.Vt[m_pic.F3[i5].Vi[2]];
                            
                            eb_v2 = v_minus(B2,A2);
                            ec_v2 = v_minus(C2,A2);
                            ao_v2 = v_minus(A2,i_v);
                            t2 = v_hit(eb_v2,ec_v2,shadow_v,ao_v2).X;
                            b2 = v_hit(eb_v2,ec_v2,shadow_v,ao_v2).Y;
                            r2 = v_hit(eb_v2,ec_v2,shadow_v,ao_v2).Z;
                                
                            if(t2>0 && b2>0 && r2>0 && b2+r2<1)//hit
                            {
                                //cout<<"other face hit light: "<<i4<<",face_index: "<<i5<<endl;
                                if(v_isequal(m_pic.Fn[i5],m_pic.Fn[i3])==0)//not duplicate face!
                                {
                                    hit_flag2 = 1;
                                    //cout<<"have shadow!"<<endl;
                                    break;    
                                }
                                else
                                {
                                    //cout<<"duplicate face!"<<endl;
                                }
                            }
                        }
                    }
                    if(hit_flag2 ==1)//hit
                    {
                        all_screen[i1][i2].R+=ck[0][0]*cl[i4][0][0];
                        all_screen[i1][i2].G+=ck[0][1]*cl[i4][0][1];
                        all_screen[i1][i2].B+=ck[0][2]*cl[i4][0][2];
                        //printf("add ambient: r: %f,g: %f,b: %f \n",all_screen[i1][i2].R,all_screen[i1][i2].G,all_screen[i1][i2].B);
                    }
                    else//not hit
                    {
                        //printf("not hit!add light %d:face_index: %d, inter_x: %f,inter_y: %f,inter_z: %f \n",i4,i3,i_v.X,i_v.Y,i_v.Z);
                        //printf("origin color: r: %f,g: %f,b: %f \n",all_screen[i1][i2].R,all_screen[i1][i2].G,all_screen[i1][i2].B);
                        all_screen[i1][i2].R += blinn_phong_illum(shadow_v,n_v,view_v,ck[0][0],cl[i4][0][0],ck[1][0],cl[i4][1][0],ck[2][0],cl[i4][2][0],shininess_a);
                        all_screen[i1][i2].G += blinn_phong_illum(shadow_v,n_v,view_v,ck[0][1],cl[i4][0][1],ck[1][1],cl[i4][1][1],ck[2][1],cl[i4][2][1],shininess_a);
                        all_screen[i1][i2].B += blinn_phong_illum(shadow_v,n_v,view_v,ck[0][2],cl[i4][0][2],ck[1][2],cl[i4][1][2],ck[2][2],cl[i4][2][2],shininess_a);
                        //printf("update color: r: %f,g: %f,b: %f \n",all_screen[i1][i2].R,all_screen[i1][i2].G,all_screen[i1][i2].B);    
                    }
                }
            }
        }
    }
}

void draw_pixel()
{
    double r,g,b;
    for(int i1=0;i1<500;i1++)//init
    {
        for(int i2=0;i2<500;i2++)
        {
            r = all_screen[i1][i2].R;
            g = all_screen[i1][i2].G;
            b = all_screen[i1][i2].B;
            write_pixel_color(i2,i1,r,g,b);

        }
    }    
}

void display_scene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
    
    init_pixel();
    trans_obj();
    update_normal();
    ray_tracing();
    
    draw_pixel();    
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
		case 'x'://smooth mode
			if(smooth_mode == 1)smooth_mode=0;//change to common smoothing
			else if(smooth_mode == 0)smooth_mode=1;//change to phong smoothing
			break;
		case 'b'://bump mapping
			if(bump_mode == 1)bump_mode=0;//change to common smoothing
			else if(bump_mode == 0)bump_mode=1;//change to phong smoothing
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
   using the escape key.  */
    read_obj("model2.obj");
    triangle_face();//use triangle faces
    normalize_obj();//normalize
    
    glutInit( &argc, argv ); 
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
    glutInitWindowSize( 500,500 );
    //glutInitWindowPosition(0, 0);
    //glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow( "Computer Graphics" ); 
    init_scene(); 
    
    //init_pixel();
    //test_obj();
    //update_normal();
    //ray_tracing();
    
    glutDisplayFunc(display_scene);  
    //glutReshapeFunc(reshape);
    //glutMouseFunc(moseMove);
    glutKeyboardFunc( keyboard );
    glutIdleFunc(myIdle);
    glutMainLoop( );                 // Initialize The Main Loop
    
}


