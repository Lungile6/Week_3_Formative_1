#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h> // For dynamic loading

#define MAX_ENTRIES 100
#define MAX_LEN 1024
// #define PASSWORD "pass123" // Remove hardcoded password

// "In-memory" storage as requested by the prompt
char diary_storage[MAX_ENTRIES][MAX_LEN];
int entry_count = 0;
char current_password[100] = {0}; // To store the password dynamically

// Function pointers for our dynamically loaded functions
void (*encrypt_func)(char *);
void (*decrypt_func)(char *);

// Helper to load library functions
int load_library() {
    // 1. Open the shared library
    void *handle = dlopen("./libdiary.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: Could not open libdiary.so\n%s\n", dlerror());
        return 0;
    }

    // 2. Load the 'encrypt' function
    encrypt_func = dlsym(handle, "encrypt");
    if (dlerror() != NULL) {
        fprintf(stderr, "Error: Could not find 'encrypt' function.\n");
        return 0;
    }

    // 3. Load the 'decrypt' function
    decrypt_func = dlsym(handle, "decrypt");
    if (dlerror() != NULL) {
        fprintf(stderr, "Error: Could not find 'decrypt' function.\n");
        return 0;
    }
    
    // We could dlclose(handle) here if we wanted, but we keep it open
    // as the functions are needed for the life of the program.
    return 1;
}

// Helper for verification
int verify_user() {
    char input[100];
    printf("Enter password to view entries: ");
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = 0; // Remove trailing newline

    if (strcmp(input, current_password) == 0) {
        return 1; // Success
    } else {
        printf("Access Denied.\n");
        return 0; // Failure
    }
}

void set_password() {
    printf("Set your new diary password: ");
    fgets(current_password, sizeof(current_password), stdin);
    current_password[strcspn(current_password, "\n")] = 0; // Remove trailing newline
    printf("Password set successfully.\n");
}

void update_password() {
    char old_input[100];
    printf("Enter old password to update: ");
    fgets(old_input, 100, stdin);
    old_input[strcspn(old_input, "\n")] = 0;

    if (strcmp(old_input, current_password) == 0) {
        printf("Enter new password: ");
        fgets(current_password, sizeof(current_password), stdin);
        current_password[strcspn(current_password, "\n")] = 0;
        printf("Password updated successfully.\n");
    } else {
        printf("Old password incorrect. Password not updated.\n");
    }
}

void add_entry() {
    if (entry_count >= MAX_ENTRIES) {
        printf("Diary is full.\n");
        return;
    }

    printf("Enter new diary entry:\n");
    char buffer[MAX_LEN];
    fgets(buffer, MAX_LEN, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    // Encrypt and store
    encrypt_func(buffer); // Encrypt using the function from .so
    strcpy(diary_storage[entry_count], buffer);
    entry_count++;
    
    printf("Entry added and encrypted in memory.\n");
}

void view_entries() {
    if (!verify_user()) {
        return; // Verification failed
    }

    if (entry_count == 0) {
        printf("No entries to display.\n");
        return;
    }
    
    printf("\n--- Your Decrypted Diary Entries ---\n");
    for (int i = 0; i < entry_count; i++) {
        // Create a temporary copy to decrypt for display
        char temp_entry[MAX_LEN];
        strcpy(temp_entry, diary_storage[i]);
        
        decrypt_func(temp_entry); // Decrypt using the function from .so
        
        printf("Entry %d: %s\n", i + 1, temp_entry);
    }
    printf("------------------------------------\n");
}

int main() {
    if (!load_library()) {
        return 1; // Exit if library fails to load
    }

    int choice = 0;
    while (choice != 5) {
        printf("\nDiary Manager\n");
        printf("1. Add new entry (encrypts and stores)\n");
        printf("2. View all entries (decrypts for console)\n");
        printf("3. Set Password\n");
        printf("4. Update Password\n");
        printf("5. Exit\n");
        printf("Choice: ");

        char input[10];
        fgets(input, 10, stdin);
        choice = atoi(input);

        switch (choice) {
            case 1:
                add_entry();
                break;
            case 2:
                view_entries();
                break;
            case 3:
                set_password();
                break;
            case 4:
                update_password();
                break;
            case 5:
                printf("Exiting.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
