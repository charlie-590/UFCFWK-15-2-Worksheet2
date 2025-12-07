#include "frame_buffer.h"
#include "ports.h"

char *fb = (char *)0x000B8000;
static unsigned short cursor_pos = 0;

#define FB_WIDTH 80
#define FB_HEIGHT 25

// Write the character c ti a frame buffer cell
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i * 2]     = c;
    fb[i * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

// move the hardware cursor to the given position
void fb_move(unsigned short x, unsigned short y)
{
    unsigned short pos = y * FB_WIDTH + x;
    fb_move_cursor(pos);
}

// Move the hardware cursor using VGA IO ports
void fb_move_cursor(unsigned short pos)
{
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);

    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// Clear the framebuffer by writing spaces to all cells
void fb_clear()
{
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fb_write_cell(i, ' ', 15, 0);
    }

    cursor_pos = 0;
    fb_move_cursor(cursor_pos);
}

// Write a null-terminated string to the framebuffer at the current cursor position
void fb_write(const char *str)
{
    int i = 0;

    while (str[i] != '\0') {
        fb_write_cell(cursor_pos, str[i], 15, 0);
        cursor_pos++;
        i++;
    }

    fb_move_cursor(cursor_pos);
}