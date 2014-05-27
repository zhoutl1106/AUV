// ѧ�������Ϸϵ�д���
// ������ E_mail: cqtmL@163.com
//====================================================================
// ѧ�������Ϸϵ�д���
// ������ E_mail: cqtmL@163.com
//====================================================================
#include "stdafx.h"
#include "baiscobj.h"
#include "../include/BITMAP.H"

GLfloat	r;       //�ɻ������Ƕȣ���������OpenGL��
float  gao=1.8f;
//////////////////////////////////////////////////////////////////////
baiscobj::baiscobj()
{	g_eye[0]= MAP;//
	g_eye[2]=-MAP;//
	g_Angle=0;//
	g_elev=-0;//360;//
////////////////////////////////////////////////////////////////////////
	char	appdir[256];
	GetCurrentDirectory(256,appdir);
	CString dir=appdir;
	if(dir.Right(8)!="���г���")
	SetCurrentDirectory("../���г���");
////////////////////////////////////////////////////////////////////////
	g_imageData = LoadBit("res/images/Terrain1.bmp",&g_bit);	//���ȸߵ���ͼ
	LoadT8("res/images/sand0.bmp",	 g_cactus[0]);	//������ͼ
	LoadT8("res/images/2RBack.bmp",g_cactus[2]);	//�����ͼ��
	LoadT8("res/images/2Front.bmp",g_cactus[3]);	//�����ͼǰ
	LoadT8("res/images/2Top.bmp",	 g_cactus[4]);	//�����ͼ��
	LoadT8("res/images/2Left.bmp", g_cactus[5]);	//�����ͼ��
	LoadT8("res/images/2Right.bmp",g_cactus[6]);	//�����ͼ��
	InitTerrain(5);//��ʼ������
	glEnable(GL_TEXTURE_2D);
}
baiscobj::~baiscobj()
{	for(int i=0;i<16;i++) glDeleteTextures(1, &g_cactus[i]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
//����===========================================================
void baiscobj::light0()
{	GLfloat light_position[] = {1.0,5.0,1.0,1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}
BOOL baiscobj::DisplayScene()//��������
{ float speed=0.5f;						//����
  float x=g_eye[0],y=g_eye[2],z=g_eye[2];
  if (KEY_DOWN(VK_SHIFT))  speed   =speed*2;//��SHIFTʱ�ļ���true
  if (KEY_DOWN(VK_LEFT))   g_Angle-=speed*2;//��ת,��λ��-
  if (KEY_DOWN(VK_RIGHT))  g_Angle+=speed*2;//��ת,��λ��+
  rad_xz = float (3.13149* g_Angle/180.0f);	//����������ת�Ƕ�
  if (KEY_DOWN(33))		   g_elev +=speed;	//Page UP  ��
  if (KEY_DOWN(34))		   g_elev -=speed;	//Page Down��
  if (g_elev<-360)		   g_elev  =-360;	//������
  if (g_elev> 360)		   g_elev  = 360;	//������
  if (KEY_DOWN(VK_UP))						//ǰ��
  { g_eye[2]+=(float)sin(rad_xz)*speed;			//�ӵ��x����
    g_eye[0]+=(float)cos(rad_xz)*speed;			//�ӵ��Z����
  }
  if (KEY_DOWN(VK_DOWN))					//����
  { g_eye[2]-=(float)sin(rad_xz)*speed;			//�ӵ��x����
    g_eye[0]-=(float)cos(rad_xz)*speed;			//�ӵ��Z����
  }
 //���Ƶ���������뿪����
  //if(g_eye[0]<  MAP_SCALE)			g_eye[0]=  MAP_SCALE;
  //if(g_eye[0]> (MAP_W-2)*MAP_SCALE)	g_eye[0]= (MAP_W-2)*MAP_SCALE;
 // if(g_eye[2]<-(MAP_W-2)*MAP_SCALE)	g_eye[2]=-(MAP_W-2)*MAP_SCALE;
 // if(g_eye[2]> -MAP_SCALE)			g_eye[2]= -MAP_SCALE;
  g_eye[1] =GetHeight((float)g_eye[0],(float)g_eye[2])+gao;//����������Ե�λ�ø�
  //������ķ���
  g_look[0] = (float)(g_eye[0] +100*cos(rad_xz));	//Ŀ���X����
  g_look[2] = (float)(g_eye[2] +100*sin(rad_xz));	//Ŀ���Z����
  g_look[1] = g_eye[1] +g_elev;				//Ŀ���Y����
  //����modelview������
 gluLookAt(g_eye[0],g_eye[1],g_eye[2],		//�ӵ�
			g_look[0],g_look[1],g_look[2],	//Ŀ���
			0.0,1.0,0.0						//�ӵ㷽��
		   );
 
  return TRUE;
}
//==========================================================================
void baiscobj::InitTerrain(float h)//������������
{ int index = 0;
  int Vertex;
  for (int z = 0; z < MAP_W; z++)
   for (int x = 0; x < MAP_W; x++)
    { Vertex = z * MAP_W + x;
      g_terrain [Vertex][0] = float(x)*MAP_SCALE;	//��������X����
      g_terrain [Vertex][1] = (float)(g_imageData[(z*MAP_W+x)*3]/3);//Y������ɽ�ĸ߶�
//	  g_terrain [Vertex][1] = h + FRAND * h;		//Y�������������ɽ�ĸ߶�
      g_terrain [Vertex][2] = -float(z)*MAP_SCALE;	//��������Z����
      g_texcoord[Vertex][0] = (float) x;			//��������x
     g_texcoord[Vertex][1] = (float) z;				//��������z
     g_index [index++] = Vertex;					//��������1ά
     g_index [index++] = Vertex+ MAP_W;
   }
  glEnableClientState(GL_VERTEX_ARRAY);				//����ʹ�õ�������
  glVertexPointer    (3,GL_FLOAT,0,g_terrain);		//װ���������
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);		//����ʹ����������
  glTexCoordPointer  (2,GL_FLOAT,0,g_texcoord);		//װ����������
}
void baiscobj::DrawSand()//��ʾ���ɽ��
{ glBindTexture(GL_TEXTURE_2D, g_cactus[0]);//����ͼ
  glTexEnvf    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�Ŵ���������˲�
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  for (int z = 0; z < MAP_W-1; z++)
    glDrawElements(GL_TRIANGLE_STRIP,MAP_W*2,GL_UNSIGNED_INT,&g_index[z*MAP_W*2]);
	//��ʾ�ɶ�������ָ�����档
}
float baiscobj::GetHeight(float x, float z)// ȡ����߶�
{ 	float CameraX = x/MAP_SCALE;              //��������һ��
	float CameraZ =-z/MAP_SCALE;              //��������һ��
	int Col0 = int(CameraX);                  //����к�
	int Row0 = int(CameraZ);                  //����к�
	int Col1 = Col0 + 1;                      //������
	int Row1 = Row0 + 1;                      //������
	if (Col1 > MAP_W)	Col1 = 0;             //�����д��ڵؿ�����ȡ����
	if (Row1 > MAP_W)	Row1 = 0;             //�����д��ڵؿ�����ȡ����
	float h00=g_terrain[Col0 + Row0*MAP_W][1];//��ȡ���Ľǵĸ߶�
	float h01=g_terrain[Col1 + Row0*MAP_W][1];
	float h11=g_terrain[Col1 + Row1*MAP_W][1];
	float h10=g_terrain[Col0 + Row1*MAP_W][1];
	float tx =CameraX - int(CameraX);         //�����Xƫ��λ��
	float ty =CameraZ - int(CameraZ);         //�����Zƫ��λ��
	float txty = tx * ty;                     //����Ϊ˫���Բ�ֵ(�ڲ�)����
	return h00*(1.0f-ty-tx+txty) 
			+ h01*(tx-txty)
			+ h11*txty
			+ h10*(ty-txty);                  //���ز�ֵ����ֵ��Ϊ�����ĸ߶ȡ�
}
void baiscobj::CreateSkyBox(int a,int wi,int he,int le)//��ʾ���
{	float width =MAP*wi;		// ��պп�
	float height=MAP*he;		// ��պи�
	float length=MAP*le;		// ��պг�
	float x = MAP  -width /2;	// ��յ�λ��x
	float y = MAP/a-height/2;	// ��յ�λ��y
	float z = -MAP -length/2;	// ��յ�λ��z
///////////////////////////////////////////////////////////////////////////////
	texture(g_cactus[2]);		// ����BACK��ͼ��
	glBegin(GL_QUADS);			// �����������ı���	
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();
	texture(g_cactus[3]);		// ����FRONT��ͼ��
	glBegin(GL_QUADS);			// �����������ı���
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();

	texture(g_cactus[4]);		// ����TOP��ͼ��
	glBegin(GL_QUADS);			// �����������ı���		
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);
	glEnd();
	texture(g_cactus[5]);		// ����LEFT��ͼǰ
	glBegin(GL_QUADS);			// �����������ı���
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);	
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z);		
	glEnd();
	texture(g_cactus[6]);		// ����RIGHT��ͼ��
	glBegin(GL_QUADS);			// �����������ı���		
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
	glEnd();
}
void baiscobj::texture(UINT textur)//������ͼ�˲�
{	glBindTexture  (GL_TEXTURE_2D, textur);// ������ͼ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); //��С���������˲�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�Ŵ���������˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}
//���ͼ��=================================================================
void baiscobj::picter(float x,float y,float z)//���ͼ��
{y=GetHeight(x,z);
 glPushAttrib(GL_CURRENT_BIT);//����������ɫ��ʵ��
 glDisable(GL_TEXTURE_2D);
 glPushMatrix();//ƽ̨==============================
	glTranslatef(x,y+0.5f,z);		//ƽ̨�Ķ�λ
	glColor3f(0.0f,1.0f,0.2f);		//��ɫ
	auxSolidCube(1);				//��̨(�߳�)
	glTranslatef(0.0f,0.8f,0.0f);	//�ܵ�λ�õ���,����0.8
	glColor3f(0.0f,0.0f,1.0f);		//��ɫ
	auxSolidBox(.2f,1.3f,.2f);		//������(�����ߡ���)
 glPopMatrix();
 glPushMatrix();//�״�==============================
	glTranslatef(x,y+2.5f,z);		//�״�Ķ�λ1
	glRotatef(r-90,0.0,1.0,0.0);	//�״���ת2
	//=======================================
	glColor3f(1.0f,1.0f,1.0f);		//��ɫ
	glRotatef(45, 1.0, 0.0, 0.0);	//�̵ĽǶȵ�������30��
	auxWireCone(1.5,0.6f);			//��԰׶��(�װ뾶����)
	//=======================================
	glRotatef(180, 1.0, 0.0, 0.0);	//�˵ĽǶȵ���,������ת
	glTranslatef(0.0f,0.0f,-0.7f);  //�˵�λ�õ���,����һ��
	auxWireCone(0.2f,2.0f);			//԰׶��(�װ뾶����)
	glColor3f(FRAND,0,0);			//�����ɫ
	glTranslatef(0.0f,0.0f,2.0f);	//�˵�λ�õ���,����һ��
	auxSolidSphere(0.1f);			//԰(�뾶)
 glPopMatrix();
 glPushMatrix();//glPopMatrix();
	glTranslatef(x,y+30.0f,z);//����Ķ�λ
	glRotatef(r, 0.0, 1.0, 0.0);	//�������ת
	glTranslatef(x/5,0,0);//����Ķ�λ
	//=============================================
	glColor3f(1.0f,0.0f,0.0f);		//��ɫ
	glRotatef(180, 0.0, 1.0, 0.0);	//�Ƕȵ���,���״�ƽ�У���ͷ��ǰ
	auxSolidCone(.2,0.6);			//԰׶(�װ뾶����)
	//=============================================
	glColor3f(1.0f,1.0f,1.0f);		//��ɫ
	glRotatef(90, 1.0, 0.0, 0.0);	//�Ƕȵ���,����ͷ�Խ�
	glTranslatef(0.0f,-1.0f,0);		//λ�õ���,����ͷ�Խ�
	auxSolidCylinder(.2f,1);		//԰��(�뾶����)
	glRotatef(-270, 1.0, 0.0, 0.0);
	glColor3f(FRAND+.6f,0.2f,0.0f);	//���ɫ
	glTranslatef(0.0f,-0.0f,-0.2f); //λ�õ���,����һ��
	auxSolidCone(.2,1.5);			//԰׶(�װ뾶����)
 glPopMatrix();
 glEnable(GL_TEXTURE_2D);
 glPopAttrib();//�ָ�ǰһ����
 r+=1.0f;if(r>360) r=0;
}
bool baiscobj::LoadT8(char *filename, GLuint &texture)//��8λ��ͼ
{	AUX_RGBImageRec *pImage = NULL;
	pImage = auxDIBImageLoad(filename);	 // װ��λͼ			
	if(pImage == NULL)	return false;	 // λͼûװ�뷵�ش���
	glGenTextures(1, &texture);			 // ������ͼ(����)
	glBindTexture    (GL_TEXTURE_2D,texture);// ������ͼ(����)
	gluBuild2DMipmaps(GL_TEXTURE_2D,4,   //
					  pImage->sizeX,     // ͼ�ο�
					  pImage->sizeY,     // ͼ�θ�
					  GL_RGB, GL_UNSIGNED_BYTE,
					  pImage->data       // ͼ������
					 );
	free(pImage->data);                  // �ͷ�λͼ����ռ�ݵ��ڴ���Դ
	free(pImage);	
	return true;                         // ���سɹ�
}
void baiscobj::LoadT16(char *filename, GLuint &texture)//����ͼ
{ glGenTextures(1, &texture);            //��ȡ1��δʹ�õ���ͼ����
  glBindTexture(GL_TEXTURE_2D, texture); //ѡ��Ҫ�󶨵���ͼ(����)
  BITMAPINFOHEADER bitHeader;			 //����λͼ�ṹ
  unsigned char *buffer;                 //����λͼָ��
  buffer=LoadBitmapFileWithAlpha(filename,&bitHeader); //����λͼ
  gluBuild2DMipmaps	( GL_TEXTURE_2D,     // ����һ�� 2D��ͼ(����)
					  4,                 // ʹ��3����ɫ(RGB)+ ��ɫ���
					  bitHeader.biWidth, // ͼ���
					  bitHeader.biHeight,// ͼ���
					  GL_RGBA,           // ������˳��
					  GL_UNSIGNED_BYTE,  // ͼ��������������ֽ�
					  buffer             // ָ����ͼ(����)����
					);                   // ������ͼ(����)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); //��С�����������˲�
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�Ŵ���������˲�
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  free(buffer);                                // �ͷ�λͼ����
}
unsigned char * baiscobj::LoadBit(char *filename, BITMAPINFOHEADER *bitmap)
{ FILE *filePtr;                        // �����ļ����
  BITMAPFILEHEADER  Header;   // ����λͼָ��
  unsigned char    *Image;        // λͼ������
  unsigned int      imageIdx = 0;       // ͼ������
  unsigned char     tempRGB;            // ��������
  filePtr = fopen(filename, "rb");// ���ļ�
  if (filePtr == NULL)    return NULL;// ���ļ���������
  fread(&Header, sizeof(BITMAPFILEHEADER), 1, filePtr);
  if (Header.bfType != BITMAP_ID)
  { fclose(filePtr);
    return NULL;
  }
  fread(bitmap, sizeof(BITMAPINFOHEADER), 1, filePtr);
  fseek(filePtr, Header.bfOffBits, SEEK_SET);
  Image = (unsigned char*)malloc(bitmap->biSizeImage);
  if (!Image)
  { free(Image);
    fclose(filePtr);
    return NULL;
  }
  fread(Image, 1, bitmap->biSizeImage, filePtr);//��ͼ�����ݶ���
  if (Image == NULL)
  { fclose(filePtr);
    return NULL;
  }
  for (imageIdx = 0; imageIdx < bitmap->biSizeImage; imageIdx+=3)
  { tempRGB = Image[imageIdx];
    Image[imageIdx] = Image[imageIdx + 2];
    Image[imageIdx + 2] = tempRGB;
  }
  fclose(filePtr);
  return Image;
/*  BITMAPINFOHEADER bitHeader;			 //����λͼ�ṹ
  unsigned char *Image;                 //����λͼָ��
  unsigned int      imageIdx = 0;       // ͼ������
  unsigned char     tempRGB;            // ��������
  Image=LoadBitmapFileWithAlpha(filename,&bitHeader); //����λͼ
  for (imageIdx =bitHeader.biSize; imageIdx < bitHeader.biSizeImage; imageIdx+=3)
  { tempRGB = Image[imageIdx];
    Image[imageIdx] = Image[imageIdx + 2];
    Image[imageIdx + 2] = tempRGB;
  }
  return Image;*/
}