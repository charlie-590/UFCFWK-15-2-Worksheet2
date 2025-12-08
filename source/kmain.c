#include "../drivers/frame_buffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/hardware_interrupt_enabler.h"
#include "../drivers/power.h"

// structure to represent a terminal command and its handler function
struct command {
    const char *name;
    void (*function)(char *args);
};

// forward declarations of command functions
static void cmd_echo(char *args);
static void cmd_clear(char *args);
static void cmd_help(char *args);

// compare two strings for equality
static int str_eq(const char *a, const char *b)
{
    while (*a && *b && (*a == *b)) {
        a++;
        b++;
    }
    return (*a == 0 && *b == 0);
}

// splits a string into command and arguments
static void parse_command(char *line, char **out_cmd, char **out_args)
{
    while (*line == ' ') line++;

    if (*line == '\0') {
        *out_cmd  = 0;
        *out_args = 0;
        return;
    }

    // command starts here
    *out_cmd = line;

    // find end of command
    while (*line && *line != ' ') {
        line++;
    }

    if (*line == '\0') {
        // only command, no args
        *out_args = 0;
        return;
    }

    // terminate command string
    *line = '\0';
    line++;

    while (*line == ' ') line++;

    if (*line == '\0') {
        *out_args = 0;
    } else {
        *out_args = line;
    }
}

//echoes command back at user
static void cmd_echo(char *args)
{
    if (args && *args) {
        fb_write_string(args);
    }
    fb_newline();
}

//clears the screen and resets cursor
static void cmd_clear(char *args)
{
    (void)args; 
    fb_clear();
}

// displays list of available commands
static void cmd_help(char *args)
{
    (void)args; 
    fb_write_string("Available commands:\n");
    fb_write_string("  echo [text] - print text\n");
    fb_write_string("  clear       - clear the screen\n");
    fb_write_string("  shutdown      - halt the system\n");
    fb_write_string("  help        - show this help\n");
    fb_newline();
}

// shuts down the system
static void cmd_shutdown(char *args)
{
    fb_newline();
    fb_write_string("Shutting down...\n");
    poweroff();
}

// Table of supported commands
static struct command commands[] = {
    { "echo",  cmd_echo },
    { "clear", cmd_clear },
    { "shutdown", cmd_shutdown },
    { "help",  cmd_help },
};

static int commands_count = sizeof(commands) / sizeof(commands[0]);

// parses and executes a single command line
static void run_command(char *line)
{
    char *cmd;
    char *args;

    parse_command(line, &cmd, &args);

    if (cmd == 0 || *cmd == '\0') {
        return;
    }

    // search in command table
    for (int i = 0; i < commands_count; i++) {
        if (str_eq(cmd, commands[i].name)) {
            commands[i].function(args);
            return;
        }
    }

    // error for unknown commands
    fb_write_string("Unknown command: ");
    fb_write_string(cmd);
    fb_newline();
}

void kmain()
{
    fb_clear();  

    interrupts_install_idt();
    enable_hardware_interrupts();

    char line[80];

    while (1) {
        fb_write_string("myos> ");

        int n = readline(line, sizeof(line));
        (void)n; 

        fb_newline();

        run_command(line);
    }
}
