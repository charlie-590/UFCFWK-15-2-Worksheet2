#include "ports.h"

#define QEMU_ACPI_SHUTDOWN_PORT  0x604
#define QEMU_ACPI_SHUTDOWN_VALUE 0x2000

void poweroff(void)
{
    // Ask QEMU/ACPI to power off
    outw(QEMU_ACPI_SHUTDOWN_PORT, QEMU_ACPI_SHUTDOWN_VALUE);

    // Fallback: if nothing happens, just halt the CPU forever
    __asm__ __volatile__("cli");
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
