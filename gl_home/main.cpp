//
//  main.cpp
//  gl_home
//
//  Created by ll on 14/11/10.
//  Copyright (c) 2014年 ll. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage [checkImageHeight][checkImageWidth][4];
double light_x = 0, light_y = 50, light_z = 0;

int v_num=25000; //记录点的数量
int f_num=50000; //记录面的数量

GLfloat **vArr; //存放点的二维数组
GLfloat **nArr; //存放法向的二维数组
int **fvArr; //存放面顶点的二维数组



//----------------场景漫游---------------
void moveMeFlat(int i);
static float angle=0.0,angle2=0.0,ratio;
static float x=0.0f,y=0.0f,z=80.0f;
static float lx=0.0f,ly=0.0f,lz=-10.0f;

GLuint LoadTexture( const char * filename )
{
    
    GLuint texture;
    
    int width, height;
    
    unsigned char * data;
    
    FILE * file;
    
    file = fopen( filename, "rb" );
    
    //cout<<filename<<endl;
    
    if ( file == NULL ) return 0;
    width = 1024;
    height = 1024;
    data = (unsigned char *)malloc( width * height * 3 );
    //int size = fseek(file,);
    fread( data, width * height * 3, 1, file );
    fclose( file );
    
    for(int i = 0; i < width * height ; i++)
    {
        int index = i*3;
        unsigned char R,G,B;
        R = data[index];
        B = data[index+1];
        G = data[index+2];
        
        //printf("R:%d, G:%d, B:%d\n",R,G,B);
        
        data[index] = B;
        data[index+1] = R;
        data[index+2] = G;
    }
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data);
    free( data );
    
    return texture;
}



void makeCheckImage(){
    int i,j,c;
    for ( i =0;i< checkImageWidth;i++){
        for(j =0;j<checkImageHeight;j++){
            c=(((i&0x8)==0)^((j&0x8)==0))*255;
            checkImage[i][j][0]=(GLubyte)c;
            checkImage[i][j][1]=(GLubyte)c;
            checkImage[i][j][2]=(GLubyte)c;
            checkImage[i][j][3]=(GLubyte)255;
            
        }
    }
}

static GLuint texture1,texture2,texture3,texture4,texture5,texture6,
                texture_desk,texture_chair,texture_earth,texture_earth2,texture_vase,texture_light,texture_dragon;

void init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    //视角变化，矩形外观测
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_COLOR_MATERIAL);
    
    texture1= LoadTexture( "/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/wall_1.bmp" );
    texture2=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/wall_2.bmp");
    texture3=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/wall_3'.bmp");
    texture5=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/wall_1.bmp");
    texture6=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/floor.bmp");
    texture_desk=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/desk10.bmp");
    texture_chair=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/desk6.bmp");
    texture_earth=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/earth.bmp");
    texture_earth2=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/earth3.bmp");
    texture_vase=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/earth2.bmp");
    texture_light=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/mickey.bmp");
    texture_dragon=LoadTexture("/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/dragon.bmp");
    
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel( GL_SMOOTH );
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 80, 0, 0, 0, 0, 1, 0);
    
    
    
}

void SetupLights()
{
    //glLoadIdentity();
    GLfloat ambientLight[]={0.4f,0.4f,0.4f,1.0f};//白色主光源环境光
    GLfloat diffuseLight[]={0.4,0.4,0.4f,1.0f};//白色主光源漫反射
    GLfloat specularLight[]={1.0f,1.0f,1.0f,1.0f};//白色主光源镜面光
    
    GLfloat redAmbiLight[]={0.2f,0.0f,0.0f,1.0f};//红色主光源环境光
    GLfloat redDiffLight[]={0.4f,0.0f,0.0f,1.0f};//红色主光源漫反射
    GLfloat redSpecLight[]={1.0f,0.0f,0.0f,1.0f};//红色主光源镜面光
    
    GLfloat lightPos[]={0.0f,45.0f,0.0f,1.0f};	//光源位置
    GLfloat spotLightPosR[]={0.0f,0.0f,0.0f,1.0f};	//射灯位置
    GLfloat spotLightPosG[]={-20.0f,0.0f,0.0f,1.0f};	//射灯位置
    GLfloat spotLightPosB[]={20.0f,0.0f,0.0f,1.0f};	//射灯位置
    
    GLfloat spotDirR[]={0.0f,-1.0f,-1.0f};			//射灯方向
    GLfloat spotDirG[]={0.0f,1.0f,0.0f};			//射灯方向
    GLfloat spotDirB[]={0.0f,1.0f,0.0f};			//射灯方向
    
    GLfloat redLight[]={1.0f,0.0f,0.0f,1.0f};		//红色光源
    GLfloat greenLight[]={0.0f,1.0f,0.0f,1.0f};		//绿色光源
    GLfloat blueLight[]={0.0f,0.0f,1.0f,1.0f};		//蓝色光源
    GLfloat redLight2[]={1.0f,0.5f,0.0f,1.0f};
    
    glEnable(GL_LIGHTING);							//启用光照
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);	//设置环境光源
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);	//设置漫反射光源
    glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);	//设置镜面光源
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);		//设置灯光位置
    glEnable(GL_LIGHT0);							//打开白色主光源
    
    glLightfv(GL_LIGHT4,GL_AMBIENT,redAmbiLight);
    glLightfv(GL_LIGHT4,GL_DIFFUSE,redDiffLight);
    glLightfv(GL_LIGHT4,GL_SPECULAR,redSpecLight);
    glLightfv(GL_LIGHT4,GL_POSITION,lightPos);
    
    glLightfv(GL_LIGHT1,GL_DIFFUSE,redLight);		//红色射灯
    glLightfv(GL_LIGHT1,GL_POSITION,spotLightPosR);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spotDirR);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,90.0f);
    
    glLightfv(GL_LIGHT2,GL_DIFFUSE,greenLight);	//绿色射灯
    glLightfv(GL_LIGHT2,GL_POSITION,spotLightPosG);
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,spotDirR);
    glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,90.0f);
    
    glLightfv(GL_LIGHT3,GL_DIFFUSE,blueLight);		//蓝色射灯
    glLightfv(GL_LIGHT3,GL_POSITION,spotLightPosB);
    glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,spotDirR);
    glLightf(GL_LIGHT3,GL_SPOT_CUTOFF,90.0f);
    
    
    glLightfv(GL_LIGHT5,GL_DIFFUSE,redLight2);		//红色射灯
    glLightfv(GL_LIGHT5,GL_POSITION,spotLightPosG);
    glLightfv(GL_LIGHT5,GL_SPOT_DIRECTION,spotDirG);
    glLightf(GL_LIGHT5,GL_SPOT_CUTOFF,90.0f);
    
    glLightfv(GL_LIGHT6,GL_DIFFUSE,redLight);		//绿色射灯
    glLightfv(GL_LIGHT6,GL_POSITION,spotLightPosB);
    glLightfv(GL_LIGHT6,GL_SPOT_DIRECTION,spotDirB);
    glLightf(GL_LIGHT6,GL_SPOT_CUTOFF,90.0f);

    
    
    GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
    GLfloat mat_amb_diff[]={0.5,0.5,0.5,1.0};
    glEnable(GL_COLOR_MATERIAL);						//启用材质的颜色跟踪
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);	//指定材料着色的面
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);	//指定材料对镜面光的反应
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
    glMateriali(GL_FRONT,GL_SHININESS,100);				//指定反射系数

}


//---------------------------------wall--------------------------------
void wall(){
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glBegin(GL_QUADS);
    //右
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0, 1.0);glVertex3f(50,50,-50);
    glTexCoord2f(0.0, 0.0);glVertex3f(50,-50,-50);
    glTexCoord2f(1.0, 0.0);glVertex3f(50,-50,50);
    glTexCoord2f(1.0, 1.0);glVertex3f(50,50,50);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBegin(GL_QUADS);
    //左
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.1, 1.0);glVertex3f(-50,50,50);
    glTexCoord2f(0.0, 0.0);glVertex3f(-50,-50,50);
    glTexCoord2f(1.0, 0.0);glVertex3f(-50,-50,-50);
    glTexCoord2f(1.0, 1.0);glVertex3f(-50,50,-50);
    glEnd();
    
    //正
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.02, 1.0);glVertex3f(-50,50,-50);
    glTexCoord2f(0.0, 0.0);glVertex3f(-50,-50,-50);
    glTexCoord2f(1.0, 0.0);glVertex3f(50,-50,-50);
    glTexCoord2f(1.0, 1.0);glVertex3f(50,50,-50);
    glEnd();
    
    //ceil
    glBindTexture(GL_TEXTURE_2D, texture5);
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0, 0.0);glVertex3f(50,50,-50);
    glTexCoord2f(0.0, 1.0);glVertex3f(50,50,50);
    glTexCoord2f(1.0, 1.0);glVertex3f(-50,50,50);
    glTexCoord2f(1.0, 0.0);glVertex3f(-50,50,-50);
    glEnd();
    
    
    //后
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0, 0.0);glVertex3f(-50,-50,50);
    glTexCoord2f(0.0, 1.0);glVertex3f(-50,50,50);
    glTexCoord2f(1.0, 1.0);glVertex3f(50,50,50);
    glTexCoord2f(1.0, 0.0);glVertex3f(50,-50,50);
    glEnd();
    
    //floor
    glBindTexture(GL_TEXTURE_2D, texture6);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 2.5);glVertex3f(-50,-50,-50);
    glTexCoord2f(0.0, 0.0);glVertex3f(-50,-50,50);
    glTexCoord2f(2.5, 0.0);glVertex3f(50,-50,50);
    glTexCoord2f(2.5, 2.5);glVertex3f(50,-50,-50);
    glEnd();

    
}

//-------------------------------cube-------------------------------
void cube(){
    glPushMatrix();
    glScaled(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
    // 前面
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// 纹理和四边形的左上
    // 后面
    glNormal3f(0, 0, -1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// 纹理和四边形的左下
    // 顶面
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 1.f); glVertex3f(-1.0f,  1.0f, -1.0f);	// 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// 纹理和四边形的左下
    glTexCoord2f(2.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// 纹理和四边形的右下
    glTexCoord2f(2.0f, 1.f); glVertex3f( 1.0f,  1.0f, -1.0f);	// 纹理和四边形的右上
    // 底面
    glNormal3f(0, -1, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// 纹理和四边形的右下
    // 右面
    glNormal3f(1, 0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// 纹理和四边形的左上
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// 纹理和四边形的左下
    // 左面
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// 纹理和四边形的左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// 纹理和四边形的右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// 纹理和四边形的右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// 纹理和四边形的左上
    
    glEnd();
    glPopMatrix();
}


//a b c 表示平移距离
void legs(double a, double b, double c){

    glPushMatrix();
    glTranslated(a, -b, c);
    glScaled(1, 2*b, 1);
    cube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-a, -b, -c);
    glScaled(1, 2*b, 1);
    cube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-a, -b, c);
    glScaled(1, 2*b, 1);
    cube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(a, -b, -c);
    glScaled(1, 2*b, 1);
    cube();
    glPopMatrix();

    
}

//--------------------------------desk---------------------------------
void desk(){
    glBindTexture(GL_TEXTURE_2D, texture_desk);
    //glDisable(GL_TEXTURE_2D);
    //--------desk--------
    glPushMatrix();
    glTranslatef(0, -30, 0);
    
    //plane
    glPushMatrix();
    glScaled(60, 1, 30);
    cube();
    glPopMatrix();
    
    //legs
    legs(25,10,10);
    glPopMatrix();
    //glEnable(GL_TEXTURE_2D);
    
}


//--------------------------------chair---------------------------------
void chair(){
    
    glBindTexture(GL_TEXTURE_2D, texture_chair);
    glPushMatrix();
    glTranslated(0, -42, 15);
    
    //plane
    glPushMatrix();
    glScaled(20, 1, 10);
    cube();
    glPopMatrix();
    
    //legs
    legs(8,4,4);
    glPopMatrix();

}




//-------------------------------sphere-------------------------------

#define pi 3.1415926

void sphere(){
    int i,j,w=40,h=20;
    float a=0.0,b=0.0;
    float hStep=180.0/h;
    float wStep=360.0/w;
    float x,y,z;
    glBegin(GL_QUADS);
    for(a=0,i=0;i<h;i++,a+=hStep){
        for(b=0,j=0;j<w;j++,b+=wStep){
            
            x=sin(a*pi/180.0)*cos(b*pi/180.0); y=sin(a*pi/180.0)*sin(b*pi/180.0); z=cos(a*pi/180.0);
            glNormal3f(x,y,z);
            glTexCoord2d(b/360.0, 1-a/180.0);
            glVertex3f(x,y,z);
            
            a=a+hStep;
            x=sin(a*pi/180.0)*cos(b*pi/180.0); y=sin(a*pi/180.0)*sin(b*pi/180.0); z=cos(a*pi/180.0);
            glNormal3f(x,y,z);
            glTexCoord2d(b/360.0, 1-a/180.0);
            glVertex3f(x,y,z);
            
            b=b+wStep;
            x=sin(a*pi/180.0)*cos(b*pi/180.0); y=sin(a*pi/180.0)*sin(b*pi/180.0); z=cos(a*pi/180.0);
            glNormal3f(x,y,z);
            glTexCoord2d(b/360.0, 1-a/180.0);
            glVertex3f(x,y,z);
            
            a=a-hStep;
            x=sin(a*pi/180.0)*cos(b*pi/180.0); y=sin(a*pi/180.0)*sin(b*pi/180.0); z=cos(a*pi/180.0);
            glNormal3f(x,y,z);
            glTexCoord2d(b/360.0, 1-a/180.0);
            glVertex3f(x,y,z);
            
            b=b-wStep;
        }
    }
    glEnd();

}

//--------------------------------light---------------------------------
void light(){
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_light);
    
    GLfloat mat_emission [] = {0.3,0.3,0.2,1.0};
    GLfloat no_mat[] = {0.0,0.0,0.0,1.0};
    glMaterialfv(GL_FRONT,GL_EMISSION, mat_emission);
    
    glColor3f(1.0, 1.0, 1.0);
    
    glPushMatrix();
    glTranslated(0, 50, 0);
    glScaled(8, 2, 8);
    //glRotated(-90, 0, 1, 0);
    glRotated(90, 1, 0, 0);
    glRotated(90, 0, 1, 0);
    sphere();
    glPopMatrix();
    
    
    glMaterialfv(GL_FRONT,GL_EMISSION, no_mat);
    
}

//--------------------------------earth---------------------------------
void earth()
{
    glEnable(GL_NORMALIZE);
    glBindTexture(GL_TEXTURE_2D, texture_earth);
    
    glPushMatrix();
    glTranslated(-15, -20, 0);
    
    glPushMatrix();
    glRotated(23.26, 0, 0, 1);
    //-------------sphere-----------
    glPushMatrix();
    glRotated(-90, 1, 0, 0);
    glScaled(7, 7, 7);
    sphere();
    glPopMatrix();
    //---cube1---
    glBindTexture(GL_TEXTURE_2D, texture_earth2);
    glPushMatrix();
    glTranslated(0, -8, 0);
    glScaled(0.9, 4, 0.9);
    cube();
    glPopMatrix();
    glPopMatrix();
    
    //-------------cube2------------
    glPushMatrix();
    glTranslated(0, -9.5, 0);
    glScaled(6, 1.2, 6);
    sphere();
    glPopMatrix();
    
    
    glPopMatrix();
    
}

void shadow_vase(){
    glPushMatrix();
    glTranslated(-2, -22.5, -2);
    
    //glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, texture_vase);
    double x=0,y=-7,z=0,r=0.0,a=0;
    int h=40, w=40,i=0,j=0;
    double hStep=10.0/h, wStep=360.0/w;
    
    glBegin(GL_QUADS);
    for(y=-7.0,i=0;i<h;i++,y=y+hStep){
        for(a=0,j=0;j<w;j++,a=a+wStep){
            
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60.0+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            a=a+wStep;
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60.0+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180.0);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            y=y+hStep;
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60.0+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180.0);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            a=a-wStep;
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180.0);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            y=y-hStep;
            
        }
    }
    glEnd();
    //glEnable(GL_CULL_FACE);
    
    glPopMatrix();
    
}
//--------------------------------vase---------------------------------
void vase(){
    
    glPushMatrix();
    glTranslated(-2, -22.5, -2);
    
    glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, texture_vase);
    double x=0,y=-7,z=0,r=0.0,a=0;
    int h=40, w=40,i=0,j=0;
    double hStep=10.0/h, wStep=360.0/w;
    
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    for(y=-7.0,i=0;i<h;i++,y=y+hStep){
        for(a=0,j=0;j<w;j++,a=a+wStep){
            
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60.0+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            a=a+wStep;
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60.0+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180.0);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            y=y+hStep;
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60.0+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180.0);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            a=a-wStep;
            r=(pow(y, 3)+4*pow(y, 2)-21*y)/60+1;x=r*cos(a*pi/180.0);z=r*sin(a*pi/180.0);
            //glNormal3f(x, 1, z);
            glTexCoord2d(a/360.0, (y+7)/10);
            glVertex3f(x, y, -z);
            
            y=y-hStep;
        
        }
    }
    glEnd();
    glEnable(GL_CULL_FACE);
    
    glPopMatrix();
}




void pointSourceShadow( double lx, double ly, double lz, double a, double b, double c, double d, GLfloat M[16]) {
    double new_d = d + (a * lx + b * ly + c * lz);
    M[0] = 1;            M[4] = 0;            M[8] = 0;             M[12] = 0;
    M[1] = 0;            M[5] = 1;            M[9] = 0;             M[13] = 0;
    M[2] = 0;            M[6] = 0;            M[10] = 1;           M[14] = 0;
    M[3]= -a/new_d;  M[7]= -b/new_d;  M[11]= -c/new_d;  M[15] = 0;
    
}

//--------------------------------shadow---------------------------------
void shadow(double y, void draw()){
    
    GLfloat M[16];
    glDisable( GL_LIGHTING);
    glDisable( GL_TEXTURE_2D);
    
    glColor4f( 0.1, 0.1, 0.1, 0.5);
    
    glPushMatrix();
    
    glTranslatef( light_x, light_y, light_z);
    
    pointSourceShadow( light_x, light_y, light_z, 0, 1, 0, -y-0.1, M);
    glMultMatrixf( M);
    glTranslatef( -light_x, -light_y, -light_z);
    draw();
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    
    
    
}

//----------------------------------dragon----------------------------------

int readfile(const char * path){
    vArr=new GLfloat*[v_num];
    int i;
    for (i=0;i<v_num;i++)
    {
        vArr[i]=new GLfloat[3];
    }
    
    fvArr=new int*[f_num];
    nArr=new GLfloat*[f_num];
    
    for (i=0;i<f_num;i++)
    {
        fvArr[i]=new int[3];
    }
    
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }
    
    int v=0,f=0;
    while( 1 ){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        
        // else : parse lineHeader
        
        if ( strcmp( lineHeader, "v" ) == 0 ){
            fscanf(file, "%f %f %f\n", &vArr[v][0], &vArr[v][1], &vArr[v][2] );
            v++;
            
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            fscanf(file, "%d %d %d\n", &fvArr[f][0], &fvArr[f][1], &fvArr[f][2]);
            f++;
        }
    }
    return 0;
}


void dragon(){
    GLfloat mat_emission [] = {0.3,0.3,0.2,0.1};
    GLfloat no_mat[] = {0.0,0.0,0.0,1.0};
    //glMaterialfv(GL_FRONT,GL_EMISSION, mat_emission);
    

    char *path="/Users/ll/Documents/Xcode/OpenGL/gl_home/texture/dragon.obj";
    readfile(path);
    
    glPushMatrix();
    glTranslated(12, -25, 6);
    glRotated(180, 0, 1, 0);
    glScaled(9, 9, 9);
    glDisable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, texture_dragon);
    GLfloat x1,y1,z1,x2,y2,z2,x3,y3,z3,a1,a2,a3,b1,b2,b3,c1,c2,c3;
    
    glBegin(GL_TRIANGLES);

    for (int i=0;i<f_num;i++)
    {
        //glNormal3f(0, 1, 0);
        x1=vArr[fvArr[i][0]-1][0];y1=vArr[fvArr[i][0]-1][1];z1=vArr[fvArr[i][0]-1][2];
        x2=vArr[fvArr[i][1]-1][0];y2=vArr[fvArr[i][1]-1][1];z2=vArr[fvArr[i][1]-1][2];
        x3=vArr[fvArr[i][2]-1][0];y3=vArr[fvArr[i][2]-1][1];z3=vArr[fvArr[i][2]-1][2];
        a1=x2-x1;a2=y2-y1;a3=z2-z1;
        b1=x3-x1;b2=y3-y1;b3=z3-z1;
        c1=a2*b3-b2*a3;
        c2=b1*a3-a1*b3;
        c3=a1*b2-b1*a2;
        glNormal3f(c1, c2, c3);
        glTexCoord2d(0, 0);
        glVertex3f(vArr[fvArr[i][0]-1][0], vArr[fvArr[i][0]-1][1], vArr[fvArr[i][0]-1][2]);
        glTexCoord2d(1, 0);
        glVertex3f(vArr[fvArr[i][1]-1][0], vArr[fvArr[i][1]-1][1], vArr[fvArr[i][1]-1][2]);
        glTexCoord2d(0, 1);
        glVertex3f(vArr[fvArr[i][2]-1][0], vArr[fvArr[i][2]-1][1], vArr[fvArr[i][2]-1][2]);
        
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    glMaterialfv(GL_FRONT,GL_EMISSION, no_mat);
}

//---------------------------display----------------------

void display(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glMatrixMode(GL_MODELVIEW);
    
    //-------------------------------------- look at ------------------------------------------
    //gluLookAt(0, 0, 80, 0, 0, 0, 0, 1, 0);
    
    
    
    glEnable(GL_TEXTURE);
    
    //glColor3f(0.0, 0.0, 0.0);
    
    
//--------------------------- Wall ------------------------------
    wall();
   
//------------------------- Furniture ----------------------------
    desk();
    shadow(-50, desk);
    
    chair();
    shadow(-49.9, chair);
    
    light();
    
//--------------------------- object ------------------------------
    earth();
    shadow(-29.5, earth);
    
    vase();
    shadow(-29.4, shadow_vase);
    
    
    //glColor3f(255/256.0, 176/256.0, 130/256.0);
    //glColor3f(252/256.0,254/256.0,207/256.0);
    glColor3f(0.96, 0.88, 0.66);
    dragon();
    shadow(-29.4, dragon);
    

//----------------shadow------------------
   // shadow(-29.4, cube);

    
    glutSwapBuffers();
    



}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, (GLfloat)w/(GLfloat)h, 0.1,500.0);
    glutSwapBuffers();

}

void orientMe(float ang) {
    lx = 10*sin(ang);
    lz = -10*cos(ang);
}


void moveMeFlat(int i) {
    x = x + i*(lx)*0.1;
    z = z + i*(lz)*0.1;
}



void inputKey(int key, int xx, int yy) {
    
    switch (key) {
        case GLUT_KEY_LEFT :
            angle -= 0.02f;
            orientMe(angle);
            break;
        case GLUT_KEY_RIGHT :
            angle +=0.02f;
            orientMe(angle);
            break;
        case GLUT_KEY_UP :
            moveMeFlat(1);
            break;
        case GLUT_KEY_DOWN :
            moveMeFlat(-1);
            break;
            
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z,      x + lx,y + ly,z + lz,              0.0f,1.0f,0.0f);
    display();

    
    
}

void OnKey(GLubyte key,GLint xx,GLint yy)
{
    switch(key)
    {
            
        case 'w':
            y=y+1;
            break;
        case 's':
            y=y-1;
            break;
        case 'a':
            angle2 +=0.02f;
            ly=16*sin(angle2);
            break;
        case 'd':
            angle2 -=0.02f;
            ly=16*sin(angle2);
            break;
            
            
//--------------------------light change--------------------------
        case 0x31:                    //打开红色射灯
            glDisable(GL_LIGHT2);
            glDisable(GL_LIGHT3);
            glEnable(GL_LIGHT1);
            glDisable(GL_LIGHT5);
            glDisable(GL_LIGHT6);
            break;
            
        case 0x32:                    //打开绿色射灯
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT3);
            glEnable(GL_LIGHT2);
            glDisable(GL_LIGHT5);
            glDisable(GL_LIGHT6);
            break;
            
        case 0x33:                    //打开蓝色射灯
            glDisable(GL_LIGHT2);
            glDisable(GL_LIGHT1);
            glEnable(GL_LIGHT3);
            glDisable(GL_LIGHT5);
            glDisable(GL_LIGHT6);
            break;
            
        case 0x35:                    //打开红色主光源
            glDisable(GL_LIGHT0);
            glEnable(GL_LIGHT4);
            break;
            
        case 0x34:
            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHT2);
            glEnable(GL_LIGHT1);
            glEnable(GL_LIGHT5);
            glEnable(GL_LIGHT6);
            glDisable(GL_LIGHT3);
            glDisable(GL_LIGHT4);
            break;
            
            
        default:                    //关闭所有射灯并将主光源换成白色
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            glDisable(GL_LIGHT3);
            glDisable(GL_LIGHT4);
            glEnable(GL_LIGHT0);
            break;
            
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z,      x + lx,y + ly,z + lz,              0.0f,1.0f,0.0f);
    display();
    //glutPostRedisplay();
}


int main(int argc, char ** argv) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 1000, 800 );
    glutCreateWindow("Wreck-It Ralph");
    init();
    SetupLights();
    
    glutSpecialFunc(inputKey);
    glutKeyboardFunc(OnKey);
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutMainLoop();
    return 0;

    
}





















