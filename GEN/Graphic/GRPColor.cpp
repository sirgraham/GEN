
/*------------------------------------------------------------------------------------------
//	GRPCOLOR.CPP
//	
//	Color object for RGBA
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 06/10/2015 12:25:35
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPColor.h"
#include "XFString.h"
#include "XDebug.h"
#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


GRPCOLOR GRPCOLOR::WHITE(1.0f,1.0f,1.0f,1.0f);
GRPCOLOR GRPCOLOR::BLACK(0.0f,0.0f,0.0f,1.0f);
GRPCOLOR GRPCOLOR::GRAY	(0.5f,0.5f,0.5f,1.0f);
GRPCOLOR GRPCOLOR::RED	(1.0f,0.0f,0.0f,1.0f);
GRPCOLOR GRPCOLOR::GREEN(0.0f,1.0f,0.0f,1.0f);
GRPCOLOR GRPCOLOR::BLUE	(0.0f,0.0f,1.0f,1.0f);

GRPCOLOR GRPCOLOR::YELLOW	(0.905f, 0.874f, 0.431f,1.0f);
GRPCOLOR GRPCOLOR::PURPLE	(0.572f, 0.247f, 0.635f,1.0f);
GRPCOLOR GRPCOLOR::PINK		(0.980f, 0.560f, 0.933f,1.0f);
GRPCOLOR GRPCOLOR::ORANGE (0.980f, 0.560f, 0.0f  , 1.0f);

/*-------------------------------------------------------------------
//	GRPCOLOR::GRPCOLOR
*/	
/**	
//	
//	Class Constructor GRPCOLOR
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:49:53
//	
//  @param				c : 
*/
/*-----------------------------------------------------------------*/
GRPCOLOR::GRPCOLOR(float* c)
{
	SetRed	(c[0]);
	SetGreen(c[1]);
	SetBlue	(c[2]);
	SetAlpha(c[3]);
}

/*-------------------------------------------------------------------
//	GRPCOLOR::GRPCOLOR
*/	
/**	
//	
//	Class Constructor GRPCOLOR
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:50:02
//	
//  @param				r : 
//  @param				g : 
//  @param				b : 
//  @param				a : 
*/
/*-----------------------------------------------------------------*/
GRPCOLOR::GRPCOLOR(float r, float g, float b,float a)
{
	this->string.Format(__L("%f,%f,%f,%f"),r,g,b,a);
	SetRed	(r);
	SetGreen(g);
	SetBlue	(b);
	SetAlpha(a);
}

GRPCOLOR::GRPCOLOR(double	r,	double g, double	b,	double a)
{
	this->string.Format(__L("%f,%f,%f,%f"),r,g,b,a);
	SetRed	((float)r);
	SetGreen((float)g);
	SetBlue	((float)b);
	SetAlpha((float)a);
}
												

/*-------------------------------------------------------------------
//	GRPCOLOR::GRPCOLOR
*/	
/**	
//	
//	Class Constructor GRPCOLOR
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:50:08
//	
//  @param				r : 
//  @param				g : 
//  @param				b : 
//  @param				a : 
*/
/*-----------------------------------------------------------------*/
GRPCOLOR::GRPCOLOR(int r, int g, int b, int a)
{
	SetRed	(r/255.0f);
	SetGreen(g/255.0f);
	SetBlue	(b/255.0f);
	SetAlpha(a/255.0f);
}


/*-------------------------------------------------------------------
//	GRPFILE3DXML::COLOR::COLOR
*/	
/**	
//	
//	constructor
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/02/2017 15:58:48
//
//  @param				format : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
GRPCOLOR::GRPCOLOR(XCHAR* format)
{
	Clean();	
	Set(format);
}

/*-------------------------------------------------------------------
//	GRPCOLOR::GRPCOLOR
*/	
/**	
//	
//	Class Constructor GRPCOLOR
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:03
//	
*/
/*-----------------------------------------------------------------*/
GRPCOLOR::GRPCOLOR()									
{ 
	Clean();									
}

/*-------------------------------------------------------------------
//	GRPCOLOR::~GRPCOLOR
*/	
/**	
//	
//	 Class Destructor GRPCOLOR
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:07
//	
*/
/*-----------------------------------------------------------------*/
GRPCOLOR::~GRPCOLOR()								
{ 
	Clean();								
}



/*-------------------------------------------------------------------
//	GRPCOLOR::GetRed
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:12
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float GRPCOLOR::GetRed()									
{ 
	return red;								
}

/*-------------------------------------------------------------------
//	GRPCOLOR::SetRed
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:17
//	
//  @param				red : 
*/
/*-----------------------------------------------------------------*/
void GRPCOLOR::SetRed(float red)				
{ 
	this->red = red;					
}

/*-------------------------------------------------------------------
//	GRPCOLOR::GetGreen
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:22
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float GRPCOLOR::GetGreen()									
{ 
	return green;							
}

/*-------------------------------------------------------------------
//	GRPCOLOR::SetGreen
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:28
//	
//  @param				green : 
*/
/*-----------------------------------------------------------------*/
void GRPCOLOR::SetGreen(float green)			
{ 
	this->green = green;			
}

/*-------------------------------------------------------------------
//	GRPCOLOR::GetBlue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:34
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float GRPCOLOR::GetBlue()									
{ 
	return blue;							
}

/*-------------------------------------------------------------------
//	GRPCOLOR::SetBlue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:39
//	
//  @param				blue : 
*/
/*-----------------------------------------------------------------*/
void GRPCOLOR::SetBlue(float blue)			
{ 
	this->blue = blue;			
}

/*-------------------------------------------------------------------
//	GRPCOLOR::GetAlpha
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:44
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float GRPCOLOR::GetAlpha()									
{ 
	return alpha;							
}

/*-------------------------------------------------------------------
//	GRPCOLOR::SetAlpha
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:49
//	
//  @param				alpha : 
*/
/*-----------------------------------------------------------------*/
void GRPCOLOR::SetAlpha(float alpha)			
{ 
	this->alpha = alpha;			
}

/*-------------------------------------------------------------------
//	GRPCOLOR::Set
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:53:57
//	
//  @param				red : 
//  @param				green : 
//  @param				blue : 
//  @param				alpha : 
*/
/*-----------------------------------------------------------------*/
void GRPCOLOR::Set(float red,float green,float blue,float alpha) 
{ 
	this->red		= red;
	this->green = green;
	this->blue	= blue;
	this->alpha	=	alpha;
}


/*-------------------------------------------------------------------
//	GRPCOLOR::Set
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:54:02
//	
//  @param				s : 
*/
/*-----------------------------------------------------------------*/
bool GRPCOLOR::Set(XCHAR* format)
{
		
	//-------------- def	
	if (format==NULL)
		return false;

	XFSTRING encoding;
	encoding=format;


	if (format[0]==__C('#'))
	{				
			int r=0,g=0,b=0;
			int success=0;

				
				success= encoding.scan(__L("#%02x%02x%02x"), &r,&g,&b);			
				if (success==3)
				{
					this->red		=float(r)/255.0f;
					this->green	=float(g)/255.0f;
					this->blue	=float(b)/255.0f;
					this->alpha	=1.0f;
					return true;
				}

		return false;
	}

	int r=0;
	XFSTRING stringencoding(100);

		  r= encoding.scan(__L("%[^'(']"), stringencoding.Get());
	if (r==1)
	{		
		//------------------- rgba
		if (stringencoding.Compare(__L("rgba"))==0)
		{
			int r= encoding.scan(__L("rgba(%f,%f,%f,%f)"), &this->red,&this->green,&this->blue,&this->alpha);			
			if (r!=4)
			{
				XDEBUG_PRINTCOLOR(4,__L("Error encoding rgba color"));						
				return false;				
			}
			return true;
		}
		else
		//------------------- b8Mask
		if (stringencoding.Compare(__L("8bit"))==0)
		{
			int _r,_g,_b,_a;
			int r= encoding.scan(__L("8bit(%d,%d,%d,%d)"), &_r,&_g,&_b,&_a);			
			if (r!=4)
			{
				XDEBUG_PRINTCOLOR(4,__L("Error encoding 8bit color"));
				return false;				
			}

			this->red		=(float(_r)/255.0f);
			this->green	=(float(_g)/255.0f);
			this->blue	=(float(_b)/255.0f);			
			this->alpha	=(float(_a)/255.0f);			
			return true;
		}
		else
	//------------------- b8Mask
		if (stringencoding.Compare(__L("css"))==0)
		{
			int _r,_g,_b,_a=255;
			int r = encoding.scan(__L("css(#%02x%02x%02x)"), &_r,&_g,&_b);
			if (r!=4)
			{
				XDEBUG_PRINTCOLOR(4,__L("Error encoding css color")); 
				return false;
			}

			this->red		=(float(_r)/255.0f);
			this->green	=(float(_g)/255.0f);
			this->blue	=(float(_b)/255.0f);			
			this->alpha	=(float(_a)/255.0f);					
			return true;
		}	
		else
			//------------------- percent
		if (stringencoding.Compare(__L("percent"))==0)
		{
			int _r,_g,_b,_a=255;
			int r = encoding.scan(__L("percent(%d%%,%d%%,%d%%,%d%%)"), &_r,&_g,&_b,&_a);			
			if (r!=4)
			{
				XDEBUG_PRINTCOLOR(4,__L("Error encoding percent color"));										
				return false;				
			}

			this->red		=float(_r)/100.0f;
			this->green	=float(_g)/100.0f;
			this->blue	=float(_b)/100.0f;	
			this->alpha	=float(_a)/100.0f;			
			return true;
		}
		else
			//------------------- hsl
		if (stringencoding.Compare(__L("hsla"))==0)
		{
				XDEBUG_PRINTCOLOR(3,__L("HSLA color encoding unavailable"));								
				return false;				
		}	
		else
		{

			//-------------- palette
					XFSTRING	xs;
					xs.Set			(stringencoding);

					xs.ToLowerCase();
					if (xs.Compare("white")	==0)					{						*this=GRPCOLOR::WHITE;										}
					else
					if (xs.Compare("black")	==0)					{						*this=GRPCOLOR::BLACK;										}
					else
					if (xs.Compare("gray")	==0)					{						*this=GRPCOLOR::GRAY;											}
					else
					if (xs.Compare("grey") == 0)					{						*this=GRPCOLOR::GRAY;											}
					else
					if (xs.Compare("red")		==0)					{						*this=GRPCOLOR::RED;											}
					else
					if (xs.Compare("green")	==0)					{						*this=GRPCOLOR::GREEN;										}
					else
					if (xs.Compare("blue")	==0)					{						*this=GRPCOLOR::BLUE;											}
					else
					if (xs.Compare("purple")== 0)					{						*this=GRPCOLOR::PURPLE;										}
					else
					if (xs.Compare("pink")	== 0)					{						*this=GRPCOLOR::PINK;											}
					else
					if (xs.Compare("orange")== 0)					{						*this=GRPCOLOR::ORANGE;										}
					else
					if (xs.Compare("yellow") == 0)				{						*this = GRPCOLOR::YELLOW;									}
					else
					{
					float dr=0,dg=0,db=0,da=0;
									int b=xs.scan(__L("%f %f %f %f"),&dr,&dg,&db,&da);
								//	int r = SSCANF(origin, "%f %f %f %f", &dr, &dg, &db, &da);									
									if (r != 4)																												
											return false;
									

									red			=(float)dr;
									green		=(float)dg;
									blue		=(float)db;
									alpha		=(float)da;
					}
		}
	}

	this->string=format;	
	return true;
}


/*-------------------------------------------------------------------
//	GRPCOLOR::Get
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2016 10:54:07
//	
//  @param				r : 
//  @param				g : 
//  @param				b : 
//  @param				a : 
*/
/*-----------------------------------------------------------------*/
void GRPCOLOR::Get(float& r,float& g,float& b,float& a) 
{ 
	r	=	red;
	g	=	green;
	b	=	blue;
	a	= alpha;
}


GRPCOLORPALETTE::GRPCOLORPALETTE()
{
		const char colors[] = {
"AliceBlue	F0F8FF	240, 248, 255\n"
"AntiqueWhite	FAEBD7	250, 235, 215\n"
"Aqua	00FFFF	0, 255, 255\n"
"Aquamarine	7FFFD4	127, 255, 212\n"
"Azure	F0FFFF	240, 255, 255\n"
"Beige	F5F5DC	245, 245, 220\n"
"Bisque	FFE4C4	255, 228, 196\n"
"Black	000000	0, 0, 0\n"
"BlanchedAlmond	FFEBCD	255, 235, 205\n"
"Blue	0000FF	0, 0, 255\n"
"BlueViolet	8A2BE2	138, 43, 226\n"
"Brown	A52A2A	165, 42, 42\n"
"BurlyWood	DEB887	222, 184, 135\n"
"CadetBlue	5F9EA0	95, 158, 160\n"
"Chartreuse	7FFF00	127, 255, 0\n"
"Chocolate	D2691E	210, 105, 30\n"
"Coral	FF7F50	255, 127, 80\n"
"CornflowerBlue	6495ED	100, 149, 237\n"
"Cornsilk	FFF8DC	255, 248, 220\n"
"Crimson	DC143C	220, 20, 60\n"
"Cyan	00FFFF	0, 255, 255\n"
"DarkBlue	00008B	0, 0, 139\n"
"DarkCyan	008B8B	0, 139, 139\n"
"DarkGoldenRod	B8860B	184, 134, 11\n"
"DarkGray	A9A9A9	169, 169, 169\n"
"DarkGrey	A9A9A9	169, 169, 169\n"
"DarkGreen	006400	0, 100, 0\n"
"DarkKhaki	BDB76B	189, 183, 107\n"
"DarkMagenta	8B008B	139, 0, 139\n"
"DarkOliveGreen	556B2F	85, 107, 47\n"
"DarkOrange	FF8C00	255, 140, 0\n"
"DarkOrchid	9932CC	153, 50, 204\n"
"DarkRed	8B0000	139, 0, 0\n"
"DarkSalmon	E9967A	233, 150, 122\n"
"DarkSeaGreen	8FBC8F	143, 188, 143\n"
"DarkSlateBlue	483D8B	72, 61, 139\n"
"DarkSlateGray	2F4F4F	47, 79, 79\n"
"DarkSlateGrey	2F4F4F	47, 79, 79\n"
"DarkTurquoise	00CED1	0, 206, 209\n"
"DarkViolet	9400D3	148, 0, 211\n"
"DeepPink	FF1493	255, 20, 147\n"
"DeepSkyBlue	00BFFF	0, 191, 255\n"
"DimGray	696969	105, 105, 105\n"
"DimGrey	696969	105, 105, 105\n"
"DodgerBlue	1E90FF	30, 144, 255\n"
"FireBrick	B22222	178, 34, 34\n"
"FloralWhite	FFFAF0	255, 250, 240\n"
"ForestGreen	228B22	34, 139, 34\n"
"Fuchsia	FF00FF	255, 0, 255\n"
"Gainsboro	DCDCDC	220, 220, 220\n"
"GhostWhite	F8F8FF	248, 248, 255\n"
"Gold	FFD700	255, 215, 0\n"
"GoldenRod	DAA520	218, 165, 32\n"
"Gray	808080	128, 128, 128\n"
"Grey	808080	128, 128, 128\n"
"Green	008000	0, 128, 0\n"
"GreenYellow	ADFF2F	173, 255, 47\n"
"HoneyDew	F0FFF0	240, 255, 240\n"
"HotPink	FF69B4	255, 105, 180\n"
"IndianRed	CD5C5C	205, 92, 92\n"
"Indigo	4B0082	75, 0, 130\n"
"Ivory	FFFFF0	255, 255, 240\n"
"Khaki	F0E68C	240, 230, 140\n"
"Lavender	E6E6FA	230, 230, 250\n"
"LavenderBlush	FFF0F5	255, 240, 245\n"
"LawnGreen	7CFC00	124, 252, 0\n"
"LemonChiffon	FFFACD	255, 250, 205\n"
"LightBlue	ADD8E6	173, 216, 230\n"
"LightCoral	F08080	240, 128, 128\n"
"LightCyan	E0FFFF	224, 255, 255\n"
"LightGoldenRodYellow	FAFAD2	250, 250, 210\n"
"LightGray	D3D3D3	211, 211, 211\n"
"LightGrey	D3D3D3	211, 211, 211\n"
"LightGreen	90EE90	144, 238, 144\n"
"LightPink	FFB6C1	255, 182, 193\n"
"LightSalmon	FFA07A	255, 160, 122\n"
"LightSeaGreen	20B2AA	32, 178, 170\n"
"LightSkyBlue	87CEFA	135, 206, 250\n"
"LightSlateGray	778899	119, 136, 153\n"
"LightSlateGrey	778899	119, 136, 153\n"
"LightSteelBlue	B0C4DE	176, 196, 222\n"
"LightYellow	FFFFE0	255, 255, 224\n"
"Lime	00FF00	0, 255, 0\n"
"LimeGreen	32CD32	50, 205, 50\n"
"Linen	FAF0E6	250, 240, 230\n"
"Magenta	FF00FF	255, 0, 255\n"
"Maroon	800000	128, 0, 0\n"
"MediumAquaMarine	66CDAA	102, 205, 170\n"
"MediumBlue	0000CD	0, 0, 205\n"
"MediumOrchid	BA55D3	186, 85, 211\n"
"MediumPurple	9370DB	147, 112, 219\n"
"MediumSeaGreen	3CB371	60, 179, 113\n"
"MediumSlateBlue	7B68EE	123, 104, 238\n"
"MediumSpringGreen	00FA9A	0, 250, 154\n"
"MediumTurquoise	48D1CC	72, 209, 204\n"
"MediumVioletRed	C71585	199, 21, 133\n"
"MidnightBlue	191970	25, 25, 112\n"
"MintCream	F5FFFA	245, 255, 250\n"
"MistyRose	FFE4E1	255, 228, 225\n"
"Moccasin	FFE4B5	255, 228, 181\n"
"NavajoWhite	FFDEAD	255, 222, 173\n"
"Navy	000080	0, 0, 128\n"
"OldLace	FDF5E6	253, 245, 230\n"
"Olive	808000	128, 128, 0\n"
"OliveDrab	6B8E23	107, 142, 35\n"
"Orange	FFA500	255, 165, 0\n"
"OrangeRed	FF4500	255, 69, 0\n"
"Orchid	DA70D6	218, 112, 214\n"
"PaleGoldenRod	EEE8AA	238, 232, 170\n"
"PaleGreen	98FB98	152, 251, 152\n"
"PaleTurquoise	AFEEEE	175, 238, 238\n"
"PaleVioletRed	DB7093	219, 112, 147\n"
"PapayaWhip	FFEFD5	255, 239, 213\n"
"PeachPuff	FFDAB9	255, 218, 185\n"
"Peru	CD853F	205, 133, 63\n"
"Pink	FFC0CB	255, 192, 203\n"
"Plum	DDA0DD	221, 160, 221\n"
"PowderBlue	B0E0E6	176, 224, 230\n"
"Purple	800080	128, 0, 128\n"
"RebeccaPurple	663399	102, 51, 153\n"
"Red	FF0000	255, 0, 0\n"
"RosyBrown	BC8F8F	188, 143, 143\n"
"RoyalBlue	4169E1	65, 105, 225\n"
"SaddleBrown	8B4513	139, 69, 19\n"
"Salmon	FA8072	250, 128, 114\n"
"SandyBrown	F4A460	244, 164, 96\n"
"SeaGreen	2E8B57	46, 139, 87\n"
"SeaShell	FFF5EE	255, 245, 238\n"
"Sienna	A0522D	160, 82, 45\n"
"Silver	C0C0C0	192, 192, 192\n"
"SkyBlue	87CEEB	135, 206, 235\n"
"SlateBlue	6A5ACD	106, 90, 205\n"
"SlateGray	708090	112, 128, 144\n"
"SlateGrey	708090	112, 128, 144\n"
"Snow	FFFAFA	255, 250, 250\n"
"SpringGreen	00FF7F	0, 255, 127\n"
"SteelBlue	4682B4	70, 130, 180\n"
"Tan	D2B48C	210, 180, 140\n"
"Teal	008080	0, 128, 128\n"
"Thistle	D8BFD8	216, 191, 216\n"
"Tomato	FF6347	255, 99, 71\n"
"Turquoise	40E0D0	64, 224, 208\n"
"Violet	EE82EE	238, 130, 238\n"
"Wheat	F5DEB3	245, 222, 179\n"
"White	FFFFFF	255, 255, 255\n"
"WhiteSmoke	F5F5F5	245, 245, 245\n"
"Yellow	FFFF00	255, 255, 0\n"
"YellowGreen	9ACD32	154, 205, 50\n" };

XFSTRING scolors = colors;
XVECTOR<XFSTRING*> splitcolors;
scolors.Explode(__C('\n'), &splitcolors);

GRPCOLOR color;
palette[__L("none")] = GRPCOLOR(0.0, 0.0, 0.0, 0.0);

for (XDWORD e = 0; e < splitcolors.GetSize(); e++)
{
		XFSTRING colorsample = *splitcolors.Get(e);
		
		int r, g, b;
		XFSTRING name;
		XFSTRING css;
		name.AdjustSize(50);
		css.AdjustSize(50);

		colorsample.scan(__L("%ls %*s %d, %d, %d\n"), name.Get(),&r,&g,&b);

		color.Set(float(r)/255.0f, float(g)/255.0f, float(b)/255.0f, 1.0f);
		color.string = name;
		XFSTRING sname;
		sname=name;
		sname.ToLowerCase();
		palette[sname.Get()] = color;				
}

splitcolors.DeleteContents();
splitcolors.DeleteAll();

}

/*-------------------------------------------------------------------
//	GRPCOLORPALETTE::~GRPCOLORPALETTE
*/
/**
//
//	destructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	26/02/2018 9:50:31
//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPCOLORPALETTE::~GRPCOLORPALETTE()
{
		palette.entries.DeleteContents();
		palette.entries.DeleteAll();
}