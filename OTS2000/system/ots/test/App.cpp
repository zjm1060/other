/* -*-c++-*-
 * testQt  - Using 'The MIT License'
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "App.h"

#include <dtCore/deltawin.h>
#include <dtCore/object.h>
#include <dtCore/scene.h>
#include <dtCore/orbitmotionmodel.h>
#include <QtOpenGL/QGLWidget>
#include <dtQt/osggraphicswindowqt.h>

#include <osg/Drawable>
#include <osg/NodeCallback>
#include <osgUtil/RenderStage>
#include <osgUtil/CullVisitor>
#include <osg/Geode>

using namespace dtCore;
using namespace dtABC;


/* Rim, body, lid, and bottom data must be reflected in x and
   y; handle and spout data across the y axis only.  */

static int patchdata[][16] =
{
    /* rim */
  {102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15},
    /* body */
  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27},
  {24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40},
    /* lid */
  {96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101,
    101, 0, 1, 2, 3,},
  {0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112,
    113, 114, 115, 116, 117},
    /* bottom */
  {118, 118, 118, 118, 124, 122, 119, 121, 123, 126,
    125, 120, 40, 39, 38, 37},
    /* handle */
  {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56},
  {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
    28, 65, 66, 67},
    /* spout */
  {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83},
  {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
    92, 93, 94, 95}
};
/* *INDENT-OFF* */

static float cpdata[][3] =
{
    {0.2, 0, 2.7}, {0.2, -0.112, 2.7}, {0.112, -0.2, 2.7}, {0,
    -0.2, 2.7}, {1.3375, 0, 2.53125}, {1.3375, -0.749, 2.53125},
    {0.749, -1.3375, 2.53125}, {0, -1.3375, 2.53125}, {1.4375,
    0, 2.53125}, {1.4375, -0.805, 2.53125}, {0.805, -1.4375,
    2.53125}, {0, -1.4375, 2.53125}, {1.5, 0, 2.4}, {1.5, -0.84,
    2.4}, {0.84, -1.5, 2.4}, {0, -1.5, 2.4}, {1.75, 0, 1.875},
    {1.75, -0.98, 1.875}, {0.98, -1.75, 1.875}, {0, -1.75,
    1.875}, {2, 0, 1.35}, {2, -1.12, 1.35}, {1.12, -2, 1.35},
    {0, -2, 1.35}, {2, 0, 0.9}, {2, -1.12, 0.9}, {1.12, -2,
    0.9}, {0, -2, 0.9}, {-2, 0, 0.9}, {2, 0, 0.45}, {2, -1.12,
    0.45}, {1.12, -2, 0.45}, {0, -2, 0.45}, {1.5, 0, 0.225},
    {1.5, -0.84, 0.225}, {0.84, -1.5, 0.225}, {0, -1.5, 0.225},
    {1.5, 0, 0.15}, {1.5, -0.84, 0.15}, {0.84, -1.5, 0.15}, {0,
    -1.5, 0.15}, {-1.6, 0, 2.025}, {-1.6, -0.3, 2.025}, {-1.5,
    -0.3, 2.25}, {-1.5, 0, 2.25}, {-2.3, 0, 2.025}, {-2.3, -0.3,
    2.025}, {-2.5, -0.3, 2.25}, {-2.5, 0, 2.25}, {-2.7, 0,
    2.025}, {-2.7, -0.3, 2.025}, {-3, -0.3, 2.25}, {-3, 0,
    2.25}, {-2.7, 0, 1.8}, {-2.7, -0.3, 1.8}, {-3, -0.3, 1.8},
    {-3, 0, 1.8}, {-2.7, 0, 1.575}, {-2.7, -0.3, 1.575}, {-3,
    -0.3, 1.35}, {-3, 0, 1.35}, {-2.5, 0, 1.125}, {-2.5, -0.3,
    1.125}, {-2.65, -0.3, 0.9375}, {-2.65, 0, 0.9375}, {-2,
    -0.3, 0.9}, {-1.9, -0.3, 0.6}, {-1.9, 0, 0.6}, {1.7, 0,
    1.425}, {1.7, -0.66, 1.425}, {1.7, -0.66, 0.6}, {1.7, 0,
    0.6}, {2.6, 0, 1.425}, {2.6, -0.66, 1.425}, {3.1, -0.66,
    0.825}, {3.1, 0, 0.825}, {2.3, 0, 2.1}, {2.3, -0.25, 2.1},
    {2.4, -0.25, 2.025}, {2.4, 0, 2.025}, {2.7, 0, 2.4}, {2.7,
    -0.25, 2.4}, {3.3, -0.25, 2.4}, {3.3, 0, 2.4}, {2.8, 0,
    2.475}, {2.8, -0.25, 2.475}, {3.525, -0.25, 2.49375},
    {3.525, 0, 2.49375}, {2.9, 0, 2.475}, {2.9, -0.15, 2.475},
    {3.45, -0.15, 2.5125}, {3.45, 0, 2.5125}, {2.8, 0, 2.4},
    {2.8, -0.15, 2.4}, {3.2, -0.15, 2.4}, {3.2, 0, 2.4}, {0, 0,
    3.15}, {0.8, 0, 3.15}, {0.8, -0.45, 3.15}, {0.45, -0.8,
    3.15}, {0, -0.8, 3.15}, {0, 0, 2.85}, {1.4, 0, 2.4}, {1.4,
    -0.784, 2.4}, {0.784, -1.4, 2.4}, {0, -1.4, 2.4}, {0.4, 0,
    2.55}, {0.4, -0.224, 2.55}, {0.224, -0.4, 2.55}, {0, -0.4,
    2.55}, {1.3, 0, 2.55}, {1.3, -0.728, 2.55}, {0.728, -1.3,
    2.55}, {0, -1.3, 2.55}, {1.3, 0, 2.4}, {1.3, -0.728, 2.4},
    {0.728, -1.3, 2.4}, {0, -1.3, 2.4}, {0, 0, 0}, {1.425,
    -0.798, 0}, {1.5, 0, 0.075}, {1.425, 0, 0}, {0.798, -1.425,
    0}, {0, -1.5, 0.075}, {0, -1.425, 0}, {1.5, -0.84, 0.075},
    {0.84, -1.5, 0.075}
};

static float tex[2][2][2] =
{
  { {0, 0},
    {1, 0}},
  { {0, 1},
    {1, 1}}
};

int VertexCount[10],LayerCount,FaceCount[10];
float Vertex[10][5000][3];
int Face[10][5000][3];
double VertexNormal[10][5000][4];
int			ListNo[100];
char *FileName="D:\\OpenSceneGraph-2.8.3\\examples\\osgteapot\\landship.DAT";
char *BKFileName="D:\\OpenSceneGraph-2.8.3\\examples\\osgteapot\\Sea.raw";

void CalcNormal(float *p1,float *p2,float *p3,double *n)
{
	//Form two vectors from the points.
	float a[3],b[3];
	a[0] = p2[0] - p1[0];
	a[1] = p2[1] - p1[1];
	a[2] = p2[2] - p1[2];
	b[0] = p3[0] - p1[0];
	b[1] = p3[1] - p1[1];
	b[2] = p3[2] - p1[2];

	//Calculate the cross product of the two vectors.
	n[0] = a[1] * b[2] - a[2] * b[1];
	n[1] = a[2] * b[0] - a[0] * b[2];
	n[2] = a[0] * b[1] - a[1] * b[0];
}

void CalcVertexNormal(double *p,double *n)
{
	//Form two vectors from the points.
	p[0] += n[0];
	p[1] += n[1];
	p[2] += n[2];
	p[3] +=1;
}

void InitData() 
{
	// TODO: Add your specialized code here and/or call the base class
	FILE *DataFile;
	int LayerNo,FaceNo,VertexNo;
	float DivideRight;
	float MaxX,MaxY,MaxZ,MinX,MinY,MinZ;
	char tmp_str[10];
	double n[3];


	//读入模型数据文件
	if ((DataFile=fopen(FileName,"rt"))==NULL)
	{
		exit(1);
	}
	fscanf(DataFile,"%d",&LayerCount);
	fscanf(DataFile,"%6s %f %f %f",tmp_str,&MinX,&MinY,&MinZ);
	fscanf(DataFile,"%6s %f %f %f",tmp_str,&MaxX,&MaxY,&MaxZ);

	DivideRight=(float)(std::max(std::max(fabs(MaxX),fabs(MinX)),std::max(std::max(fabs(MaxY),fabs(MinY)),std::max(fabs(MaxZ),fabs(MinZ))))/2.5);
	for(LayerNo=0;LayerNo<LayerCount;LayerNo++)
	{
		fscanf(DataFile,"%d %d",&FaceCount[LayerNo],&VertexCount[LayerNo]);
		for(FaceNo=0;FaceNo<FaceCount[LayerNo];FaceNo++)
			fscanf(DataFile,"%d %d %d",&Face[LayerNo][FaceNo][0],&Face[LayerNo][FaceNo][1],
			&Face[LayerNo][FaceNo][2]);
		for(VertexNo=0;VertexNo<VertexCount[LayerNo];VertexNo++)
		{
			fscanf(DataFile,"%f %f %f",&Vertex[LayerNo][VertexNo][0],&Vertex[LayerNo][VertexNo][2],
				&Vertex[LayerNo][VertexNo][1]);
			Vertex[LayerNo][VertexNo][0]/=DivideRight;
			Vertex[LayerNo][VertexNo][1]/=DivideRight;
			Vertex[LayerNo][VertexNo][2]/=DivideRight;
		}
	}
	fclose(DataFile);

	// 法向量清零
	for(LayerNo=0;LayerNo<LayerCount;LayerNo++)
		for(VertexNo=0;VertexNo<VertexCount[LayerNo];VertexNo++)
		{
			VertexNormal[LayerNo][VertexNo][0]=0.0f;
			VertexNormal[LayerNo][VertexNo][1]=0.0f;
			VertexNormal[LayerNo][VertexNo][2]=0.0f;
			VertexNormal[LayerNo][VertexNo][3]=0.0f;
		}

		// 计算每点法向量
		for(LayerNo=0;LayerNo<LayerCount;LayerNo++)
		{
			for(FaceNo=0;FaceNo<FaceCount[LayerNo];FaceNo++)
			{
				CalcNormal(Vertex[LayerNo][(Face[LayerNo][FaceNo][0])],
					Vertex[LayerNo][(Face[LayerNo][FaceNo][1])],
					Vertex[LayerNo][(Face[LayerNo][FaceNo][2])],n);
				CalcVertexNormal(VertexNormal[LayerNo][(Face[LayerNo][FaceNo][0])],n);
				CalcVertexNormal(VertexNormal[LayerNo][(Face[LayerNo][FaceNo][1])],n);
				CalcVertexNormal(VertexNormal[LayerNo][(Face[LayerNo][FaceNo][2])],n);
			}
		}
		// 法向量平均
		double r;
		for(LayerNo=0;LayerNo<LayerCount;LayerNo++)
			for(VertexNo=0;VertexNo<VertexCount[LayerNo];VertexNo++)
			{
				if (VertexNormal[LayerNo][VertexNo][3]!=0)
				{
					VertexNormal[LayerNo][VertexNo][0]/=VertexNormal[LayerNo][VertexNo][3];
					VertexNormal[LayerNo][VertexNo][1]/=VertexNormal[LayerNo][VertexNo][3];
					VertexNormal[LayerNo][VertexNo][2]/=VertexNormal[LayerNo][VertexNo][3];
					r=sqrt(VertexNormal[LayerNo][VertexNo][0]*VertexNormal[LayerNo][VertexNo][0]+
						VertexNormal[LayerNo][VertexNo][1]*VertexNormal[LayerNo][VertexNo][1]+
						VertexNormal[LayerNo][VertexNo][2]*VertexNormal[LayerNo][VertexNo][2]);
					VertexNormal[LayerNo][VertexNo][0]/=r;
					VertexNormal[LayerNo][VertexNo][1]/=r;
					VertexNormal[LayerNo][VertexNo][2]/=r;
				}
			}
}

#define ImageWidth 512
#define ImageHeight 512
GLubyte Image[ImageWidth][ImageHeight][3];
void makeImage()
{
	int i, j; 
	int chr,chg,chb;

	FILE *fp;
	if((fp=fopen(BKFileName,"rb"))==NULL)
	{
		exit(0);
	}

	for (i = 1; i <=ImageWidth; i++) 
	{
		for (j = 0; j < ImageHeight; j++) 
		{
			chr=fgetc(fp);
			Image[ImageWidth-i][j][0] = (GLubyte) chr;
			chg=fgetc(fp);
			Image[ImageWidth-i][j][1] = (GLubyte) chg;
			chb=fgetc(fp);
			Image[ImageWidth-i][j][2] = (GLubyte) chb;
		}
	}
}

void MakeObject()
{
	int LayerNo,FaceNo;

	for(LayerNo=0;LayerNo<LayerCount;LayerNo++)
	{
		ListNo[LayerNo]=LayerNo+1 ;
		glNewList(ListNo[LayerNo],GL_COMPILE);
		glBegin(GL_TRIANGLES);
		for(FaceNo=0;FaceNo<FaceCount[LayerNo];FaceNo++)
		{
			glNormal3f((GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][0])][0],
				(GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][0])][1],
				(GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][0])][2]);
			glVertex3fv((GLfloat *)Vertex[LayerNo][(Face[LayerNo][FaceNo][0])]);

			glNormal3f((GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][1])][0],
				(GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][1])][1],
				(GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][1])][2]);
			glVertex3fv((GLfloat *)Vertex[LayerNo][(Face[LayerNo][FaceNo][1])]);

			glNormal3f((GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][2])][0],
				(GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][2])][1],
				(GLfloat)VertexNormal[LayerNo][(Face[LayerNo][FaceNo][2])][2]);
			glVertex3fv((GLfloat *)Vertex[LayerNo][(Face[LayerNo][FaceNo][2])]);
		}
		glEnd();
		glEndList();
	}

	makeImage();   //海
	
	glNewList(100,GL_COMPILE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, 
		ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 
		Image);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex3f(-1.50f, -1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.50f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.50f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.50f, -1.0f, 0.0f);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glEndList();

}

/* *INDENT-ON* */

static void
teapot(GLint grid, GLenum type)
{
  float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
  long i, j, k, l;

  glPushAttrib(GL_ENABLE_BIT | GL_EVAL_BIT);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_MAP2_VERTEX_3);
  glEnable(GL_MAP2_TEXTURE_COORD_2);
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 4; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 3; l++) {
          p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
          q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
          if (l == 1)
            q[j][k][l] *= -1.0;
          if (i < 6) {
            r[j][k][l] =
              cpdata[patchdata[i][j * 4 + (3 - k)]][l];
            if (l == 0)
              r[j][k][l] *= -1.0;
            s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
            if (l == 0)
              s[j][k][l] *= -1.0;
            if (l == 1)
              s[j][k][l] *= -1.0;
          }
        }
      }
    }
    /*glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2,
      &tex[0][0][0]);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &p[0][0][0]);
    glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);
    glEvalMesh2(type, 0, grid, 0, grid);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &q[0][0][0]);
    glEvalMesh2(type, 0, grid, 0, grid);
    if (i < 6) {
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &r[0][0][0]);
      glEvalMesh2(type, 0, grid, 0, grid);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &s[0][0][0]);
      glEvalMesh2(type, 0, grid, 0, grid);
    }*/
  }

  MakeObject();

  glPopAttrib();
}


// Now the OSG wrapper for the above OpenGL code, the most complicated bit is computing
// the bounding box for the above example, normally you'll find this the easy bit.
class Teapot : public osg::Drawable
{
    public:
        Teapot() {}

        /** Copy constructor using CopyOp to manage deep vs shallow copy.*/
        Teapot(const Teapot& teapot,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY):
            osg::Drawable(teapot,copyop) {}

        META_Object(myTeapotApp,Teapot)


        // the draw immediate mode method is where the OSG wraps up the drawing of
        // of OpenGL primitives.
        virtual void drawImplementation(osg::RenderInfo&) const
        {
            // teapot(..) doens't use vertex arrays at all so we don't need to toggle their state
            // if we did we'd need to something like following call
            // state.disableAllVertexArrays(), see src/osg/Geometry.cpp for the low down.
        
            // just call the OpenGL code.
            teapot(14,GL_FILL);
        }
        
        
        // we need to set up the bounding box of the data too, so that the scene graph knows where this
        // objects is, for both positioning the camera at start up, and most importantly for culling.
        virtual osg::BoundingBox computeBound() const
        {
            osg::BoundingBox bbox;

            // follow is some truely horrible code required to calculate the 
            // bounding box of the teapot.  Have used the original code above to do
            // help compute it.
            float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
            long i, j, k, l;

            for (i = 0; i < 10; i++) {
              for (j = 0; j < 4; j++) {
                for (k = 0; k < 4; k++) {
                
                  for (l = 0; l < 3; l++) {
                    p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                    q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                    if (l == 1)
                      q[j][k][l] *= -1.0;
                      
                    if (i < 6) {
                      r[j][k][l] =
                        cpdata[patchdata[i][j * 4 + (3 - k)]][l];
                      if (l == 0)
                        r[j][k][l] *= -1.0;
                      s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
                      if (l == 0)
                        s[j][k][l] *= -1.0;
                      if (l == 1)
                        s[j][k][l] *= -1.0;
                    }
                  }
                  
                  bbox.expandBy(osg::Vec3(p[j][k][0],p[j][k][1],p[j][k][2]));
                  bbox.expandBy(osg::Vec3(q[j][k][0],q[j][k][1],q[j][k][2]));

                  if (i < 6)
                  {
                    bbox.expandBy(osg::Vec3(r[j][k][0],r[j][k][1],r[j][k][2]));
                    bbox.expandBy(osg::Vec3(s[j][k][0],s[j][k][1],s[j][k][2]));
                  }
                  
                  
                }
              }
            }

            return bbox;
        }

    protected:
    
        virtual ~Teapot() {}
        
};

osg::Geode* createTeapot()
{
    osg::Geode* geode = new osg::Geode();

	InitData();

    // add the teapot to the geode.
    geode->addDrawable( new Teapot );

    /*/ add a reflection map to the teapot.     
    osg::Image* image = osgDB::readImageFile("Images/reflect.rgb");
    if (image)
    {
        osg::Texture2D* texture = new osg::Texture2D;
        texture->setImage(image);

        osg::TexGen* texgen = new osg::TexGen;
        texgen->setMode(osg::TexGen::SPHERE_MAP);

        osg::StateSet* stateset = new osg::StateSet;
        stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
        stateset->setTextureAttributeAndModes(0,texgen,osg::StateAttribute::ON);
        
        geode->setStateSet(stateset);
    }*/
   
    return geode;
}

App::App(const std::string& configFilename)
: Application(configFilename)
{
}

//////////////////////////////////////////////////////////////////////////
App::~App()
{
   //The widget's about to be destroyed so we need to tell the GraphicsWindow it's gone (to avoid crash on exit)
   dtQt::OSGGraphicsWindowQt* osgGraphWindow = dynamic_cast<dtQt::OSGGraphicsWindowQt*>(GetWindow()->GetOsgViewerGraphicsWindow());
   if (osgGraphWindow)
   {
      osgGraphWindow->SetQGLWidget(NULL);
   }
}
   
//////////////////////////////////////////////////////////////////////////
void App::Config()
{      
   Application::Config();

   connect(&mMainWindow, SIGNAL(LoadFile(const QString&)), this, SLOT(OnLoadFile(const QString&)));

   connect(this, SIGNAL(FileLoaded(bool)), &mMainWindow, SLOT(OnFileLoaded(bool)));

   mCameraMotion = new dtCore::OrbitMotionModel(GetKeyboard(), GetMouse());
   mCameraMotion->SetTarget(GetCamera());
   mCameraMotion->SetFocalPoint(osg::Vec3(0.f, 0.f, 0.f));

   mObject1 = new dtCore::Object("Object1");
   AddDrawable(mObject1.get());

   GetScene()->GetSceneNode()->addChild( createTeapot() );

   //hook up the Qt widgeting stuff
   dtQt::OSGGraphicsWindowQt* osgGraphWindow = dynamic_cast<dtQt::OSGGraphicsWindowQt*>(GetWindow()->GetOsgViewerGraphicsWindow());
   mMainWindow.SetGraphicsWidget(osgGraphWindow->GetQGLWidget());
   mMainWindow.show();
}

////////////////////////////////////////////////////////////////////////////////
void App::OnLoadFile(const QString& filename)
{
   bool valid = mObject1->LoadFile(filename.toStdString());

   emit FileLoaded(valid);  
}
