#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#define FB_BLACK       0x0
#define FB_BLUE        0x1
#define FB_GREEN       0x2
#define FB_CYAN        0x3
#define FB_RED         0x4
#define FB_MAGENTA     0x5
#define FB_BROWN       0x6
#define FB_LIGHT_GREY  0x7
#define FB_DARK_GREY   0x8
#define FB_LIGHT_BLUE  0x9
#define FB_LIGHT_GREEN 0xA
#define FB_LIGHT_CYAN  0xB
#define FB_LIGHT_RED   0xC
#define FB_PINK        0xD
#define FB_YELLOW      0xE
#define FB_WHITE       0xF

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_move_cursor(unsigned short pos);   // low-level
void fb_move(unsigned short x, unsigned short y); // 2D API
void fb_clear();
void fb_write(const char *str);

#endif
