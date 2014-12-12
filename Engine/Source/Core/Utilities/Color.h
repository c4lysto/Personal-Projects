#ifndef COLOR_H
#define COLOR_H

#include "GlobalDefines.h"

class Color
{
private:
	union
	{
		u32 m_Color;

		struct  
		{
			u8 a, b, g, r;
		};
	};

public:
	Color() {}
	explicit Color(u32 color) : m_Color(color) {}
	inline Color(s32 R, s32 G, s32 B, s32 A = 255)
	{
		Set(R, G, B, A);
	}
	inline Color(u32 R, u32 G, u32 B, u32 A = 255)
	{
		Set((s32)R, G, B, A);
	}
	inline Color(float R, float G, float B, float A = 1.0f)
	{
		Set(R, G, B, A);
	}

	u32 GetR() const {return r;}
	float GetRf() const {return r*ONE_OVER_255;}

	u32 GetG() const {return g;}
	float GetGf() const {return g*ONE_OVER_255;}

	u32 GetB() const {return b;}
	float GetBf() const {return b*ONE_OVER_255;}

	u32 GetA() const {return a;}
	float GetAf() const {return a*ONE_OVER_255;}


	inline void SetR(s32 R) {r = Clamp(R,0,255);}
	inline void SetR(float R) {SetR(s32(Saturate(R)*255.0f));}

	inline void SetG(s32 G) {g = Clamp(G,0,255);}
	inline void SetG(float G) {SetR(s32(Saturate(G)*255.0f));}

	inline void SetB(s32 B) {b = Clamp(B,0,255);}
	inline void SetB(float B) {SetR(s32(Saturate(B)*255.0f));}

	inline void SetA(s32 A) {a = Clamp(A,0,255);}
	inline void SetA(float A) {SetR(s32(Saturate(A)*255.0f));}

	__forceinline void Set(u32 color) {m_Color = color;}

	__forceinline void Set(s32 R, s32 G, s32 B, s32 A)
	{
		SetR(R); SetG(G); SetB(B); SetA(A);
	}

	__forceinline void Set(float R, float G, float B, float A)
	{
		SetR(R); SetG(G); SetB(B); SetA(A);
	}
};

// Color Values Taken From: http://www.w3schools.com/html/html_colorvalues.asp

#define Color_AliceBlue				Color(240,248,255)
#define Color_AntiqueWhite			Color(250,235,215)
#define Color_Aqua					Color(0,255,255)
#define Color_Aquamarine			Color(127,255,212)
#define Color_Azure					Color(240,255,255)
#define Color_Beige					Color(245,245,220)
#define Color_Bisque				Color(255,228,196)
#define Color_Black					Color(0,0,0)
#define Color_BlanchedAlmond		Color(255,235,205)
#define Color_Blue					Color(0,0,255)
#define Color_BlueViolet			Color(138,43,226)
#define Color_Brown					Color(165,42,42)
#define Color_BurlyWood				Color(222,184,135)
#define Color_CadetBlue				Color(95,158,160)
#define Color_Chartreuse			Color(127,255,0)
#define Color_Chocolate				Color(210,105,30)
#define Color_Coral					Color(255,127,80)
#define Color_CornflowerBlue		Color(100,149,237)
#define Color_Cornsilk				Color(255,248,220)
#define Color_Crimson				Color(220,20,60)
#define Color_Cyan					Color(0,255,255)
#define Color_DarkBlue				Color(0,0,139)
#define Color_DarkCyan				Color(0,139,139)
#define Color_DarkGoldenRod			Color(184,134,11)
#define Color_DarkGray				Color(169,169,169)
#define Color_DarkGreen				Color(0,100,0)
#define Color_DarkKhaki				Color(189,183,107)
#define Color_DarkMagenta			Color(139,0,139)
#define Color_DarkOliveGreen		Color(85,107,47)
#define Color_DarkOrange			Color(255,140,0)
#define Color_DarkOrchid			Color(153,50,204)
#define Color_DarkRed				Color(139,0,0)
#define Color_DarkSalmon			Color(233,150,122)
#define Color_DarkSeaGreen			Color(143,188,143)
#define Color_DarkSlateBlue			Color(72,61,139)
#define Color_DarkSlateGray			Color(47,79,79)
#define Color_DarkTurquoise			Color(0,206,209)
#define Color_DarkViolet			Color(148,0,211)
#define Color_DeepPink				Color(255,20,147)
#define Color_DeepSkyBlue			Color(0,191,255)
#define Color_DimGray				Color(105,105,105)
#define Color_DodgerBlue			Color(30,144,255)
#define Color_FireBrick				Color(178,34,34)
#define Color_FloralWhite			Color(255,250,240)
#define Color_ForestGreen			Color(34,139,34)
#define Color_Fuchsia				Color(255,0,255)
#define Color_Gainsboro				Color(220,220,220)
#define Color_GhostWhite			Color(248,248,255)
#define Color_Gold					Color(255,215,0)
#define Color_GoldenRod				Color(218,165,32)
#define Color_Gray					Color(128,128,128)
#define Color_Green					Color(0,128,0)
#define Color_GreenYellow			Color(173,255,47)
#define Color_HoneyDew				Color(240,255,240)
#define Color_HotPink				Color(255,105,180)
#define Color_IndianRed				Color(205,92,92)
#define Color_Indigo				Color(75,0,130)
#define Color_Ivory					Color(255,255,240)
#define Color_Khaki					Color(240,230,140)
#define Color_Lavender				Color(230,230,250)
#define Color_LavenderBlush			Color(255,240,245)
#define Color_LawnGreen				Color(124,252,0)
#define Color_LemonChiffon			Color(255,250,205)
#define Color_LightBlue				Color(173,216,230)
#define Color_LightCoral			Color(240,128,128)
#define Color_LightCyan				Color(224,255,255)
#define Color_LightGoldenRodYellow	Color(250,250,210)
#define Color_LightGray				Color(211,211,211)
#define Color_LightGreen			Color(144,238,144)
#define Color_LightPink				Color(255,182,193)
#define Color_LightSalmon			Color(255,160,122)
#define Color_LightSeaGreen			Color(32,178,170)
#define Color_LightSkyBlue			Color(135,206,250)
#define Color_LightSlateGray		Color(119,136,153)
#define Color_LightSteelBlue		Color(176,196,222)
#define Color_LightYellow			Color(255,255,224)
#define Color_Lime					Color(0,255,0)
#define Color_LimeGreen				Color(50,205,50)
#define Color_Linen					Color(250,240,230)
#define Color_Magenta				Color(255,0,255)
#define Color_Maroon				Color(128,0,0)
#define Color_MediumAquaMarine		Color(102,205,170)
#define Color_MediumBlue			Color(0,0,205)
#define Color_MediumOrchid			Color(186,85,211)
#define Color_MediumPurple			Color(147,112,219)
#define Color_MediumSeaGreen		Color(60,179,113)
#define Color_MediumSlateBlue		Color(123,104,238)
#define Color_MediumSpringGreen		Color(0,250,154)
#define Color_MediumTurquoise		Color(72,209,204)
#define Color_MediumVioletRed		Color(199,21,133)
#define Color_MidnightBlue			Color(25,25,112)
#define Color_MintCream				Color(245,255,250)
#define Color_MistyRose				Color(255,228,225)
#define Color_Moccasin				Color(255,228,181)
#define Color_NavajoWhite			Color(255,222,173)
#define Color_Navy					Color(0,0,128)
#define Color_OldLace				Color(253,245,230)
#define Color_Olive					Color(128,128,0)
#define Color_OliveDrab				Color(107,142,35)
#define Color_Orange				Color(255,165,0)
#define Color_OrangeRed				Color(255,69,0)
#define Color_Orchid				Color(218,112,214)
#define Color_PaleGoldenRod			Color(238,232,170)
#define Color_PaleGreen				Color(152,251,152)
#define Color_PaleTurquoise			Color(175,238,238)
#define Color_PaleVioletRed			Color(219,112,147)
#define Color_PapayaWhip			Color(255,239,213)
#define Color_PeachPuff				Color(255,218,185)
#define Color_Peru					Color(205,133,63)
#define Color_Pink					Color(255,192,203)
#define Color_Plum					Color(221,160,221)
#define Color_PowderBlue			Color(176,224,230)
#define Color_Purple				Color(128,0,128)
#define Color_Red					Color(255,0,0)
#define Color_RosyBrown				Color(188,143,143)
#define Color_RoyalBlue				Color(65,105,225)
#define Color_SaddleBrown			Color(139,69,19)
#define Color_Salmon				Color(250,128,114)
#define Color_SandyBrown			Color(244,164,96)
#define Color_SeaGreen				Color(46,139,87)
#define Color_SeaShell				Color(255,245,238)
#define Color_Sienna				Color(160,82,45)
#define Color_Silver				Color(192,192,192)
#define Color_SkyBlue				Color(135,206,235)
#define Color_SlateBlue				Color(106,90,205)
#define Color_SlateGray				Color(112,128,144)
#define Color_Snow					Color(255,250,250)
#define Color_SpringGreen			Color(0,255,127)
#define Color_SteelBlue				Color(70,130,180)
#define Color_Tan					Color(210,180,140)
#define Color_Teal					Color(0,128,128)
#define Color_Thistle				Color(216,191,216)
#define Color_Tomato				Color(255,99,71)
#define Color_Turquoise				Color(64,224,208)
#define Color_Violet				Color(238,130,238)
#define Color_Wheat					Color(245,222,179)
#define Color_White					Color(255,255,255)
#define Color_WhiteSmoke			Color(245,245,245)
#define Color_Yellow				Color(255,255,0)
#define Color_YellowGreen			Color(154,205,50)

#endif // COLOR_H