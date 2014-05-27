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
//A、地
	float		g_terrain	[MAP_W*MAP_W][3];	    //地域数据
	GLuint		g_index		[MAP_W*MAP_W* 2];	    //顶点数组
	float		g_texcoord	[MAP_W*MAP_W][2];	    //索引数组
	void		InitTerrain(float h);	            //载入地域数组
	void		DrawSand();					        //显示随机山势
	float		GetHeight(float x, float z);        //取当前位置高度
//B、漫游
	float	    g_eye [3]; 	                        //视点
	float	    g_look[3];	                        //目标点
	float	    rad_xz;		                        //角度
	float	    g_Angle;	                        //方位角
	float	    g_elev;		                        //仰俯角
	BOOL		DisplayScene();	                    //摄像漫游
//C、天、其它
	UINT		g_cactus[16];	                    //贴图
	BITMAPINFOHEADER  g_bit;                        //
	unsigned char    *g_imageData;                  //
	void		CreateSkyBox(int a,int wi,int he,int le);//显示天
	void		texture(UINT textur);				//设置贴图滤波
	void		light0();                           // 环境光
//D、
	void		picter(float x,float y,float z);    //组合图形
	bool		LoadT8(char *filename, GLuint &texture); //调8位贴图
	void		LoadT16(char *filename, GLuint &texture);//调16位帖图
	unsigned char* LoadBit(char *filename, BITMAPINFOHEADER *bitmap);
};

#endif // !defined(AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_)
