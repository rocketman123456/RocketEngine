#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(RK_WINDOWS)
#include <Windows.h>
#include <conio.h>  // _kbhit
HANDLE hStdin = INVALID_HANDLE_VALUE;
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/termios.h>
#endif

#include "vm.h"

enum {
    VM_EXIT_SUCCESS,
    VM_EXIT_USAGE,
    VM_EXIT_INPUT_INVALID,
    VM_EXIT_OPCODE_INVALID,
};

#if defined(RK_WINDOWS)
DWORD fdwMode, fdwOldMode;

void disable_input_buffering()
{
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &fdwOldMode); /* save old mode */
    fdwMode = fdwOldMode
            ^ ENABLE_ECHO_INPUT  /* no input echo */
            ^ ENABLE_LINE_INPUT; /* return when one or
                                    more characters are available */
    SetConsoleMode(hStdin, fdwMode); /* set new mode */
    FlushConsoleInputBuffer(hStdin); /* clear buffer */
}

void restore_input_buffering()
{
    SetConsoleMode(hStdin, fdwOldMode);
}
#else
static struct termios original_tio;

static void disable_input_buffering() {
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

static void restore_input_buffering() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}
#endif

static void handle_signal(int signal) {
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <program.obj>\n", argv[0]);
        return VM_EXIT_USAGE;
    }

    vm_ctx vm = vm_create();
    vm_load_os(vm);

    vm_load_result load_result = vm_load_file(vm, argv[1]);

    switch (load_result) {
        case VM_LOAD_SUCCESS:
            break;

        case VM_LOAD_INPUT_NOT_FOUND:
            fprintf(stderr, "%s: Failed to load input: %s\n", argv[0], strerror(errno));
            return VM_EXIT_INPUT_INVALID;

        case VM_LOAD_INPUT_TOO_LARGE:
            fprintf(stderr, "%s: Failed to load input: Input exceeded memory space\n", argv[0]);
            return VM_EXIT_INPUT_INVALID;
    }

    disable_input_buffering();
    signal(SIGINT, handle_signal);

    vm_run_result run_result = vm_run(vm);

    restore_input_buffering();

    switch (run_result) {
        case VM_RUN_SUCCESS:
            break;

        case VM_RUN_UNIMPLEMENTED_OPCODE:
            fprintf(stderr, "%s: Failed to execute input: Attempted to execute unimplemented opcode\n", argv[0]);
            return VM_EXIT_OPCODE_INVALID;
    }

    vm_destroy(vm);

    return VM_EXIT_SUCCESS;
}
