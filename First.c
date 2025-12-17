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
    int priority;
    int completed;
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
                categorizeReminder(NULL);
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

// Add a new reminder
void addReminder() {
    if (total_reminders >= MAX_REMINDERS) {
        printf("\nMaximum reminders limit reached!\n");
        return;
    }

    Reminder new_reminder;

    printf("\nADD NEW REMINDER\n");

    new_reminder.id = total_reminders + 1;

    getStringInput("Enter title: ", new_reminder.title, MAX_LENGTH);
    getStringInput("Enter description: ", new_reminder.description, MAX_LENGTH);

    // Category selection
    printf("\nSelect category:\n");
    printf("1. Personal\n");
    printf("2. Work\n");
    printf("3. Shopping\n");
    printf("4. Health\n");
    printf("5. Finance\n");
    printf("6. Education\n");
    printf("7. Other\n");

    int cat_choice = getValidIntInput("Choose category (1-7): ", 1, 7);

    switch(cat_choice) {
        case 1: strcpy(new_reminder.category, "Personal"); break;
        case 2: strcpy(new_reminder.category, "Work"); break;
        case 3: strcpy(new_reminder.category, "Shopping"); break;
        case 4: strcpy(new_reminder.category, "Health"); break;
        case 5: strcpy(new_reminder.category, "Finance"); break;
        case 6: strcpy(new_reminder.category, "Education"); break;
        case 7: strcpy(new_reminder.category, "Other"); break;
    }

    getStringInput("Enter due date (DD/MM/YYYY): ", new_reminder.due_date, 20); // time , nF,

    printf("\nSet priority (1-5, where 5 is highest):\n");
    new_reminder.priority = getValidIntInput("Priority: ", 1, 5);

    new_reminder.completed = 0;
    new_reminder.created_at = time(NULL);

    reminders[total_reminders] = new_reminder;
    total_reminders++;

    printf("\nReminder added successfully! (ID: %d)\n", new_reminder.id);
}
void viewAllReminders(){

}

// Mark reminder as completed
void markAsCompleted() {

}

void searchReminder() {

}
//due f
