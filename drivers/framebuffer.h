#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

void fb_move_cursor(unsigned short pos);   // low-level
void fb_move(unsigned short x, unsigned short y); // 2D API

void fb_clear();
void fb_write(const char *str);

#endif
