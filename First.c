#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_REMINDERS 100
#define MAX_LENGTH 200
#define FILENAME "reminders.dat"
// Structure for Reminder
typedef struct {
    int id;
    char title[MAX_LENGTH];
    char description[MAX_LENGTH];
    char category[MAX_LENGTH];
    char due_date[20];
    int priority; // 1-5 scale
    int completed; // 0 = pending, 1 = completed
    time_t created_at;
} Reminder;

Reminder reminders[MAX_REMINDERS];
int total_reminders = 0;

// Function prototypes
void loadReminders();
void saveReminders();

int main() {
    loadReminders();

    int choice;

    do {
        displayMenu();
        choice = getValidIntInput("Enter your choice: ", 1, 8);

        switch(choice) {
            case 1:
                addReminder();
                break;
            case 2:
                viewAllReminders();
                break;
            case 3:
                markAsCompleted();
                break;
            case 4:
                searchReminder();
                break;
            case 5:
                sortReminders();
                break;
            case 6:
                categorizeReminder(NULL); // View categories
                break;
            case 7:
                printf("\nSaving reminders...\n");
                saveReminders();
                break;
            case 8:
                printf("\nExiting Smart Reminder Pro. Goodbye!\n");
                saveReminders();
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
        getchar();

    } while(choice != 8);

    return 0;
}

// Load reminders from file
void loadReminders() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        total_reminders = 0;
        return;
    }

    fread(&total_reminders, sizeof(int), 1, file);
    fread(reminders, sizeof(Reminder), total_reminders, file);

    fclose(file);
    printf("Loaded %d reminders from storage.\n", total_reminders);
}
// Save reminders to file
void saveReminders() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error: Cannot save reminders!\n");
        return;
    }

    fwrite(&total_reminders, sizeof(int), 1, file);
    fwrite(reminders, sizeof(Reminder), total_reminders, file);

    fclose(file);
    printf("Reminders saved successfully!\n");
}