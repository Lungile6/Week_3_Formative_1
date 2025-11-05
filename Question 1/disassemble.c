#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // 1. Check for correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_elf_executable>\n", argv[0]);
        return 1; // Exit with an error
    }

    char *filepath = argv[1];
    char command[1024]; // Buffer to hold the system command

    // 2. Safely construct the command string
    // We use snprintf to prevent buffer overflows
    snprintf(command, sizeof(command), 
             "objdump -d --section=.text %s > disassembly.txt", 
             filepath);

    printf("Executing: %s\n", command);

    // 3. Execute the command using the system shell
    int status = system(command);

    // 4. Check if the command executed successfully
    if (status != 0) {
        fprintf(stderr, "Error: objdump command failed.\n");
        fprintf(stderr, "Please ensure 'objdump' is installed and you provided a valid ELF file.\n");
        return 1;
    }

    printf("Success: Disassembled .text section saved to disassembly.txt\n");
    return 0;
}
