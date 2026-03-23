// stb_truetype.h - v1.26 - public domain
// Minimal stb_truetype header for ImGui

#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#define __STB_INCLUDE_STB_TRUETYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint8_t stbtt_uint8;
typedef int8_t stbtt_int8;
typedef uint16_t stbtt_uint16;
typedef int16_t stbtt_int16;
typedef uint32_t stbtt_uint32;
typedef int32_t stbtt_int32;

typedef struct
{
  void *userdata;
  unsigned char *data;
  int fontstart;
  int numGlyphs;
  int loca, head, glyf, hhea, hmtx, kern, gpos, svg;
  int index_map;
  int indexToLocFormat;
} stbtt_fontinfo;

typedef struct
{
  stbtt_uint16 x0, y0, x1, y1;
  float xoff, yoff, xadvance;
} stbtt_bakedchar;

// Inline stub implementations
static inline int stbtt_InitFont(
    stbtt_fontinfo *info, const unsigned char *data, int offset)
{
  if (info)
    info->data = (unsigned char *)data;
  return 1;
}

static inline int stbtt_GetFontOffsetForIndex(
    const unsigned char *data, int index)
{
  return 0;
}

static inline int stbtt_FindGlyphIndex(
    const stbtt_fontinfo *info, int unicode_codepoint)
{
  return 0;
}

static inline void stbtt_GetFontVMetrics(
    const stbtt_fontinfo *info, int *ascent, int *descent, int *lineGap)
{
  if (ascent)
    *ascent = 0;
  if (descent)
    *descent = 0;
  if (lineGap)
    *lineGap = 0;
}

static inline int stbtt_GetGlyphHMetrics(const stbtt_fontinfo *info,
    int glyph_index,
    int *advanceWidth,
    int *leftSideBearing)
{
  if (advanceWidth)
    *advanceWidth = 0;
  if (leftSideBearing)
    *leftSideBearing = 0;
  return 0;
}

static inline float stbtt_ScaleForPixelHeight(
    const stbtt_fontinfo *info, float pixels)
{
  return 1.0f;
}

static inline float stbtt_ScaleForMappingEmToPixels(
    const stbtt_fontinfo *info, float pixels)
{
  return 1.0f;
}

static inline void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo *font,
    int glyph,
    float scale_x,
    float scale_y,
    int *ix0,
    int *iy0,
    int *ix1,
    int *iy1)
{
  if (ix0)
    *ix0 = 0;
  if (iy0)
    *iy0 = 0;
  if (ix1)
    *ix1 = 0;
  if (iy1)
    *iy1 = 0;
}

static inline void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *font,
    int glyph,
    float scale_x,
    float scale_y,
    float shift_x,
    float shift_y,
    int *ix0,
    int *iy0,
    int *ix1,
    int *iy1)
{
  if (ix0)
    *ix0 = 0;
  if (iy0)
    *iy0 = 0;
  if (ix1)
    *ix1 = 0;
  if (iy1)
    *iy1 = 0;
}

static inline void stbtt_MakeGlyphBitmap(const stbtt_fontinfo *info,
    unsigned char *output,
    int out_w,
    int out_h,
    int out_stride,
    float scale_x,
    float scale_y,
    int glyph)
{}

static inline void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo *info,
    unsigned char *output,
    int out_w,
    int out_h,
    int out_stride,
    float scale_x,
    float scale_y,
    float shift_x,
    float shift_y,
    int glyph)
{}

static inline void stbtt_MakeGlyphBitmapSubpixelPrefilter(
    const stbtt_fontinfo *info,
    unsigned char *output,
    int out_w,
    int out_h,
    int out_stride,
    float scale_x,
    float scale_y,
    float shift_x,
    float shift_y,
    int oversample_x,
    int oversample_y,
    float *sub_x,
    float *sub_y,
    int glyph)
{}

#ifdef __cplusplus
}
#endif

#endif
