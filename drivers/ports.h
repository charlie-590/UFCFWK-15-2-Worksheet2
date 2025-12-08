#ifndef PORTS_H
#define PORTS_H

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short value);

#endif
