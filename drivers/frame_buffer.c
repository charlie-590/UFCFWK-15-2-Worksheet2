#include "frame_buffer.h"
#include "ports.h"

char *fb = (char *)0x000B8000;
static unsigned short cursor_pos = 0;

#define FB_WIDTH 80
#define FB_HEIGHT 25

// Writes a single character at the given cell index
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i * 2]     = c;
    fb[i * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

// moves the cursor to (x, y)
void fb_move(unsigned short x, unsigned short y)
{
    unsigned short pos = y * FB_WIDTH + x;
    fb_move_cursor(pos);
}

// moves the VGA hardware cursor to the given linear position
void fb_move_cursor(unsigned short pos)
{
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);

    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

// Clear the framebuffer and moves cursor back
void fb_clear()
{
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fb_write_cell(i, ' ', 15, 0);
    }

    cursor_pos = 0;
    fb_move_cursor(cursor_pos);
}

// writes a single character at the current cursor position
void fb_write_char(char c)
{
    fb_write_cell(cursor_pos, c, FB_LIGHT_GREY, FB_BLACK);
    cursor_pos++;
    fb_move_cursor(cursor_pos);
}

// moves the cursor to the beginning of the next line
void fb_newline(void)
{
    cursor_pos += 80 - (cursor_pos % 80);
    fb_move_cursor(cursor_pos);
}

// removes the character before the current cursor position
void fb_backspace(void)
{
    if (cursor_pos == 0) return;

    cursor_pos--;
    fb_write_cell(cursor_pos, ' ', FB_LIGHT_GREY, FB_BLACK);
    fb_move_cursor(cursor_pos);
}

// writes a null-terminated string at the current cursor position
void fb_write_string(const char *str)
{
    int i = 0;
    while (str[i]) {
        if (str[i] == '\n') {
            fb_newline();
        } else {
            fb_write_char(str[i]);
        }
        i++;
    }
}

