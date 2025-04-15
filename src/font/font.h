#include <stdint.h>

#if 0 // don't need to load all available fonts

#define FONT_05x08           0
#define FONT_05x12           1
#define FONT_06x08           2
#define FONT_06x10           3
#define FONT_08x08           4
#define FONT_08x12           5
#define FONT_08x14           6
#define FONT_10x16           7
#define FONT_12x16           8
#define FONT_12x20           9
#define FONT_16x26          10
#define FONT_22x36          11
#define FONT_24x40          12
#define FONT_32x53          13
#define N_FONTS             14

#else // only use one font

#if WCLOCK24H
#define FONT_16x26           0
#else
#define FONT_24x40           0
#endif
#define N_FONTS              1

#endif // 0

#ifdef unix
extern void     tft_show_screen (void);
#endif

extern int      number_of_fonts (void);
extern void     set_font (int);
extern int      font_width (void);
extern int      font_height (void);
extern void     draw_letter (unsigned char, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t);
extern void     draw_string (unsigned char *, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t);
