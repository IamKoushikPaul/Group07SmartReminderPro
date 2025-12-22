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
if (total_reminders == 0) {
        printf("\nNo reminders found!\n");
        return;
    }

    printf("\nALL REMINDERS (%d total)\n", total_reminders);
    printf("\n");
    printf("ID  | Status  | Priority | Category   | Due Date   | Title\n");
    printf("\n");

    for(int i = 0; i < total_reminders; i++) {
        printf("%-3d | %-7s | %-8d | %-10s | %-10s | %s\n",
               reminders[i].id,
               reminders[i].completed ? "Done" : "Pending",
               reminders[i].priority,
               reminders[i].category,
               reminders[i].due_date,
               reminders[i].title);
    }
}

// Mark reminder as completed
void markAsCompleted() {
if (total_reminders == 0) {
        printf("\nNo reminders to mark as completed!\n");
        return;
    }

    viewAllReminders();

    int id = getValidIntInput("\nEnter reminder ID to mark as completed (0 to cancel): ", 0, total_reminders);

    if (id == 0) return;

    for(int i = 0; i < total_reminders; i++) {
        if (reminders[i].id == id) {
            if (reminders[i].completed) {
                printf("Reminder is already marked as completed!\n");
            } else {
                reminders[i].completed = 1;
                printf("Reminder '%s' marked as completed!\n", reminders[i].title);
                saveReminders();
            }
            return;
        }
    }

    printf("Reminder with ID %d not found!\n", id);
}
// Search reminder
void searchReminder() {
if (total_reminders == 0) {
        printf("\nNo reminders to search!\n");
        return;
    }

    printf("\nSEARCH REMINDER\n");
    printf("1. Search by Title\n");
    printf("2. Search by Category\n");
    printf("3. Search by Priority\n");
    printf("4. Search by Status (Completed/Pending)\n");

    int choice = getValidIntInput("Choose search option: ", 1, 4);
    char search_term[MAX_LENGTH];
    int search_int;
    int found = 0;

    switch(choice) {
        case 1:
            getStringInput("Enter title to search: ", search_term, MAX_LENGTH);
            printf("\nSearch Results for '%s':\n", search_term);
            for(int i = 0; i < total_reminders; i++) {
                if (strstr(reminders[i].title, search_term) != NULL) {
                    displayReminder(reminders[i]);
                    found = 1;
                }
            }
            break;

        case 2:
            printf("\nAvailable categories:\n");
            categorizeReminder(NULL);
            getStringInput("Enter category to search: ", search_term, MAX_LENGTH);
            printf("\nReminders in category '%s':\n", search_term);
            for(int i = 0; i < total_reminders; i++) {
                if (strcmp(reminders[i].category, search_term) == 0) {
                    displayReminder(reminders[i]);
                    found = 1;
                }
            }
            break;

        case 3:
            search_int = getValidIntInput("Enter priority (1-5): ", 1, 5);
            printf("\nReminders with priority %d:\n", search_int);
            for(int i = 0; i < total_reminders; i++) {
                if (reminders[i].priority == search_int) {
                    displayReminder(reminders[i]);
                    found = 1;
                }
            }
            break;

        case 4:
            printf("\n1. Completed\n2. Pending\n");
            search_int = getValidIntInput("Select status: ", 1, 2);
            printf("\n%s reminders:\n", search_int == 1 ? "Completed" : "Pending");
            for(int i = 0; i < total_reminders; i++) {
                if ((search_int == 1 && reminders[i].completed) ||
                    (search_int == 2 && !reminders[i].completed)) {
                    displayReminder(reminders[i]);
                    found = 1;
                }
            }
            break;
    }

    if (!found) {
        printf("No reminders found matching your search criteria.\n");
    }
}
// Sort reminders
void sortReminders() {
    if (total_reminders == 0) {
        printf("\nNo reminders to sort!\n");
        return;
    }
    printf("\nSORT REMINDERS\n");
    printf("1. Sort by Priority (High to Low)\n");
    printf("2. Sort by Due Date\n");
    printf("3. Sort by Category\n");
    printf("4. Sort by Status\n");
    printf("5. Sort by Creation Date\n");

    int choice = getValidIntInput("Choose sorting option: ", 1, 5);
    
    Reminder sorted[MAX_REMINDERS]; // Create a copy array for sorting
    memcpy(sorted, reminders, sizeof(Reminder) * total_reminders);

    for(int i = 0; i < total_reminders - 1; i++) {   // Bubble sort implementation
        for(int j = 0; j < total_reminders - i - 1; j++) {
            int swap = 0;

            switch(choice) {
                case 1: // Sort by priority
                    if (sorted[j].priority < sorted[j+1].priority) swap = 1;
                    break;
                case 2: // Sort by due date
                    if (strcmp(sorted[j].due_date, sorted[j+1].due_date) > 0) swap = 1;
                    break;
                case 3: // Sort by category
                    if (strcmp(sorted[j].category, sorted[j+1].category) > 0) swap = 1;
                    break;
                case 4: // Sort by status
                    if (sorted[j].completed > sorted[j+1].completed) swap = 1;
                    break;
                case 5: // Sort by creation date
                    if (sorted[j].created_at < sorted[j+1].created_at) swap = 1;
                    break;
            }

            if (swap) {
                Reminder temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }

    // Display sorted reminders
    printf("\nSORTED REMINDERS\n");
    printf("\n");
    printf("ID  | Status  | Priority | Category   | Due Date   | Title\n");
    printf("\n");

    for(int i = 0; i < total_reminders; i++) {
        printf("%-3d | %-7s | %-8d | %-10s | %-10s | %s\n",
               sorted[i].id,
               sorted[i].completed ? "Done" : "Pending",
               sorted[i].priority,
               sorted[i].category,
               sorted[i].due_date,
               sorted[i].title);
    }
}
// Categorize reminders
void categorizeReminder(char* category) {
    if (total_reminders == 0) {
        printf("\nNo reminders to categorize!\n");
        return;
    }

    char categories[MAX_REMINDERS][MAX_LENGTH]; // Count reminders per category
    int category_count[MAX_REMINDERS] = {0};
    int unique_categories = 0;

    for(int i = 0; i < total_reminders; i++) {
        int found = 0;
        for(int j = 0; j < unique_categories; j++) {
            if (strcmp(categories[j], reminders[i].category) == 0) {
                category_count[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(categories[unique_categories], reminders[i].category);
            category_count[unique_categories] = 1;
            unique_categories++;
        }
    }

    printf("\nCATEGORY OVERVIEW\n");
    printf("Category | Count |\n");
    printf("-------------------------\n");

    for(int i = 0; i < unique_categories; i++) {
        printf("%-13s | %-5d |\n", categories[i], category_count[i]);
    }

    // If a specific category is requested, show its reminders
    if (category != NULL) {
        printf("\nREMINDERS IN CATEGORY: %s\n", category);
        int found = 0;
        for(int i = 0; i < total_reminders; i++) {
            if (strcmp(reminders[i].category, category) == 0) {
                displayReminder(reminders[i]);
                found = 1;
            }
        }
        if (!found) {
            printf("No reminders found in this category.\n");
        }
    }
}

// Display a single reminder in detail
void displayReminder(Reminder r) {
    printf("\n\n");
    printf("ID: %d | Priority: %d/5 | Status: %s\n",
           r.id, r.priority, r.completed ? "Completed" : "Pending");
    printf("Category: %s | Due: %s\n", r.category, r.due_date);
    printf("Title: %s\n", r.title);
    printf("Description: %s\n", r.description);

    char time_str[26]; // Convert creation time to readable format
    struct tm* time_info = localtime(&r.created_at);
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", time_info);
    printf("Created: %s\n", time_str);
    printf("\n");
}

// Display main menu
void displayMenu() {
    system("clear || cls"); // Clear screen for Linux/Windows

    
    printf("SMART REMINDER PRO v1.0\n");
    printf("Total Reminders: %-3d\n", total_reminders);
    printf("1. Add New Reminder\n");
    printf("2. View All Reminders\n");
    printf("3. Mark as Completed\n");
    printf("4. Search Reminder\n");
    printf("5. Sort Reminders\n");
    printf("6. Categorize Reminders\n");
    printf("7. Save Reminders\n");
    printf("8. Exit\n");
}
// Utility function for integer input validation
int getValidIntInput(const char* prompt, int min, int max) {
    int value;
    char buffer[100];

    while(1) {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
    }
}
// Utility function for string input
void getStringInput(const char* prompt, char* buffer, int max_length) {
    printf("%s", prompt);
    fgets(buffer, max_length, stdin);
    
    buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

    if (strlen(buffer) == 0) { // If input is empty, ask again
        printf("Input cannot be empty! ");
        getStringInput(prompt, buffer, max_length);
    }
}

