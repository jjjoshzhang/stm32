/**
  ******************************************************************************
  * 简介：适用于pal库oled驱动的字体文件
  *       该文件由bdf2c软件自动生成，工具获取方式 Bilibili关注"铁头山羊"
  * 字体名称：-FreeType-HYJinKaiJ-Medium-R-Normal--22-160-100-100-P-182-ISO10646-1
  * 字体字号：16磅
  * 字符数量：5/7911
  * 备注：123
  ******************************************************************************
*/

#ifndef hyjk16_H_
#define hyjk16_H_

#include "oled_font.h"

/* 字形数据 */
static const uint8_t hyjk16_GlyphBitmap_0036[] = {0x07,0x00,0x18,0x00,0x30,0x00,0x60,0x00,0x40,0x00,0xCE,0x00,0xF1,0x80,0xE0,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x40,0xC0,0x61,0x80,0x1E,0x00,}; // 6
static const uint8_t hyjk16_GlyphBitmap_6714[] = {0x01,0x80,0x00,0x61,0x82,0x00,0x33,0x3F,0x80,0x32,0x33,0x00,0x14,0x33,0x00,0x0F,0xF3,0x00,0xFC,0x37,0x00,0x4D,0xBB,0x00,0x6D,0xB3,0x00,0x6D,0xB3,0x00,0x6D,0xBF,0x00,0x7F,0xA3,0x00,0xD9,0xA3,0x00,0x18,0x63,0x00,0x18,0x63,0x00,0x30,0x43,0x00,0x60,0x8F,0x00,0xC0,0x87,0x00,}; // 朔

/* 映射表 */
static const uint32_t hyjk16_Map[] = {54,26388,};

/* 字形 */
static const Glyph_TypeDef hyjk16_Glyphs[] = {
	// 6
	{
		.Name = "0036", 
		.Encoding = 54, 
		.Swx0 = 540, 
		.Swy0 = 0, 
		.Dwx0 = 12, 
		.Dwy0 = 0, 
		.BBw = 10, 
		.BBh = 14, 
		.BBxoff0x = 1, 
		.BByoff0y = -1, 
		.nBytes = 28, 
		.Bitmap = hyjk16_GlyphBitmap_0036, 
	},
	// 朔
	{
		.Name = "6714", 
		.Encoding = 26388, 
		.Swx0 = 990, 
		.Swy0 = 0, 
		.Dwx0 = 22, 
		.Dwy0 = 0, 
		.BBw = 17, 
		.BBh = 18, 
		.BBxoff0x = 3, 
		.BByoff0y = -3, 
		.nBytes = 54, 
		.Bitmap = hyjk16_GlyphBitmap_6714, 
	},
};

/* 字体文件 */
const Font_TypeDef hyjk16 =
{
	.SpecVersion = "2.1",
	.FontName = "-FreeType-HYJinKaiJ-Medium-R-Normal--22-160-100-100-P-182-ISO10646-1",
	.MetricsSet = 0,
	.FontSize = 16,
	.Xres = 100,
	.Yres = 100,
	.FBBx = 24,
	.FBBy = 29,
	.FBBXoff = -1,
	.FBBYoff = -8,
	.nChars = 5,
	.Map = hyjk16_Map,
	.Glyphs = hyjk16_Glyphs,
};

#endif
