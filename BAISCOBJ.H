// baiscobj.h: interface for the baiscobj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_)
#define AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class baiscobj  
{
public:
	baiscobj();
	virtual ~baiscobj();
public:
//A����
	float		g_terrain	[MAP_W*MAP_W][3];	    //��������
	GLuint		g_index		[MAP_W*MAP_W* 2];	    //��������
	float		g_texcoord	[MAP_W*MAP_W][2];	    //��������
	void		InitTerrain(float h);	            //�����������
	void		DrawSand();					        //��ʾ���ɽ��
	float		GetHeight(float x, float z);        //ȡ��ǰλ�ø߶�
//B������
	float	    g_eye [3]; 	                        //�ӵ�
	float	    g_look[3];	                        //Ŀ���
	float	    rad_xz;		                        //�Ƕ�
	float	    g_Angle;	                        //��λ��
	float	    g_elev;		                        //������
	BOOL		DisplayScene();	                    //��������
//C���졢����
	UINT		g_cactus[16];	                    //��ͼ
	BITMAPINFOHEADER  g_bit;                        //
	unsigned char    *g_imageData;                  //
	void		CreateSkyBox(int a,int wi,int he,int le);//��ʾ��
	void		texture(UINT textur);				//������ͼ�˲�
	void		light0();                           // ������
//D��
	void		picter(float x,float y,float z);    //���ͼ��
	bool		LoadT8(char *filename, GLuint &texture); //��8λ��ͼ
	void		LoadT16(char *filename, GLuint &texture);//��16λ��ͼ
	unsigned char* LoadBit(char *filename, BITMAPINFOHEADER *bitmap);
};

#endif // !defined(AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_)