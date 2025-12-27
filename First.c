#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REMINDERS 100
#define MAX_LENGTH 200
#define FILENAME "reminders.dat"

// Reminder structure
typedef struct {
    int id;
    char title[MAX_LENGTH];
    char description[MAX_LENGTH];
    char category[MAX_LENGTH];
    char due_date[20];      // DD/MM/YYYY HH:MM
    time_t due_time;        // For accurate sorting
    int priority;           // 1–5 (Auto assigned)
    int completed;          // 0 = Pending, 1 = Done
    time_t created_at;
} Reminder;

Reminder reminders[MAX_REMINDERS];
int total_reminders = 0;

// User profile structure
struct UserProfile {
    char username[50];
    char theme[10]; // light or dark
};

/* Function Prototypes for Reminder System */
void loadReminders();
void saveReminders();
void addReminder();
void viewAllReminders();
void markAsCompleted();
void searchReminder();
void sortReminders();
void categorizeReminder();
void displayReminder(Reminder r);
void displayReminderMenu();
int getValidIntInput(const char* prompt, int min, int max);
void getStringInput(const char* prompt, char* buffer, int max_length);
void updatePriority(Reminder *r);

/* Function Prototypes for User Profile System */
void saveProfile(struct UserProfile user);
struct UserProfile loadProfile();
void updateProfile();
void backupData();
void restoreData();
void exportData();
void showHelp();
void showAbout();
void displayProfileMenu();

/* Auto Update Priority Function */
void updatePriority(Reminder *r) {
    double hours_left = difftime(r->due_time, time(NULL)) / 3600.0;

    if (hours_left <= 24) r->priority = 5;
    else if (hours_left <= 72) r->priority = 4;
    else if (hours_left <= 168) r->priority = 3;
    else if (hours_left <= 720) r->priority = 2;
    else r->priority = 1;
}

/* File Handling for Reminders */
void loadReminders() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) return;

    fread(&total_reminders, sizeof(int), 1, file);
    fread(reminders, sizeof(Reminder), total_reminders, file);
    fclose(file);
}

void saveReminders() {
    FILE *file = fopen(FILENAME, "wb");
    if (!file) return;

    fwrite(&total_reminders, sizeof(int), 1, file);
    fwrite(reminders, sizeof(Reminder), total_reminders, file);
    fclose(file);
}

/* Add Reminder */
void addReminder() {
    if (total_reminders >= MAX_REMINDERS) {
        printf("Maximum reminders reached!\n");
        return;
    }

    Reminder r;
    r.id = total_reminders + 1;

    printf("\nADD NEW REMINDER\n");
    getStringInput("Title: ", r.title, MAX_LENGTH);
    getStringInput("Description: ", r.description, MAX_LENGTH);

    // Category selection
    printf("\nSelect category:\n");
    printf("1. Personal\n2. Work\n3. Shopping\n4. Health\n5. Finance\n6. Education\n7. Other\n");
    int c = getValidIntInput("Choose: ", 1, 7);

    const char *cats[] = {
        "Personal","Work","Shopping","Health",
        "Finance","Education","Other"
    };
    strcpy(r.category, cats[c-1]);

    getStringInput("Due date & time (DD/MM/YYYY HH:MM): ", r.due_date, 20);

    struct tm tm_due = {0};
    sscanf(r.due_date, "%d/%d/%d %d:%d",
           &tm_due.tm_mday,
           &tm_due.tm_mon,
           &tm_due.tm_year,
           &tm_due.tm_hour,
           &tm_due.tm_min);

    tm_due.tm_mon -= 1;
    tm_due.tm_year -= 1900;
    r.due_time = mktime(&tm_due);

    // Auto priority assignment
    updatePriority(&r);
    r.completed = 0;
    r.created_at = time(NULL);

    reminders[total_reminders++] = r;
    printf("Auto-assigned Priority: %d (based on due time)\n", r.priority);
    printf("\nReminder added successfully!\n");
}

/* View All Reminders */
void viewAllReminders() {
    if (total_reminders == 0) {
        printf("\nNo reminders!\n");
        return;
    }

    printf("\nID | Status | Pr | Category   | Due Date & Time   | Title\n");
    printf("------------------------------------------------------------\n");

    for(int i = 0; i < total_reminders; i++) {
        printf("%-2d | %-6s | %-2d | %-10s | %-16s | %s\n",
            reminders[i].id,
            reminders[i].completed ? "Done" : "Pend",
            reminders[i].priority,
            reminders[i].category,
            reminders[i].due_date,
            reminders[i].title
        );
    }
}

/* Mark Completed */
void markAsCompleted() {
    viewAllReminders();
    if (total_reminders == 0) return;

    int id = getValidIntInput("Enter ID (0 cancel): ", 0, total_reminders);
    if (id == 0) return;

    for(int i = 0; i < total_reminders; i++) {
        if (reminders[i].id == id) {
            reminders[i].completed = 1;
            printf("Marked as completed!\n");
            return;
        }
    }
    printf("Reminder not found!\n");
}

/* Search */
void searchReminder() {
    if (total_reminders == 0) {
        printf("\nNo reminders to search!\n");
        return;
    }

    char key[MAX_LENGTH];
    getStringInput("Search title: ", key, MAX_LENGTH);

    int found = 0;
    for(int i = 0; i < total_reminders; i++) {
        if (strstr(reminders[i].title, key)) {
            displayReminder(reminders[i]);
            found = 1;
        }
    }
    if (!found) printf("\nNo match found!\n");
}

/* Sort */
void sortReminders() {
    if (total_reminders == 0) {
        printf("\nNo reminders to sort!\n");
        return;
    }

    int c = getValidIntInput("Sort by: 1.Priority 2.Due Time 3.Category 4.Status 5.Created Time\nChoose: ", 1, 5);

    for(int i=0;i<total_reminders-1;i++) {
        for(int j=0;j<total_reminders-i-1;j++) {
            int swap = 0;

            if (c==1 && reminders[j].priority < reminders[j+1].priority) swap=1;
            else if (c==2 && reminders[j].due_time > reminders[j+1].due_time) swap=1;
            else if (c==3 && strcmp(reminders[j].category, reminders[j+1].category) > 0) swap=1;
            else if (c==4 && reminders[j].completed < reminders[j+1].completed) swap=1;
            else if (c==5 && reminders[j].created_at < reminders[j+1].created_at) swap=1;

            if (swap) {
                Reminder t = reminders[j];
                reminders[j] = reminders[j+1];
                reminders[j+1] = t;
            }
        }
    }

    viewAllReminders();
}

/* Categorize */
void categorizeReminder() {
    if (total_reminders == 0) {
        printf("\nNo reminders!\n");
        return;
    }

    char cats[20][MAX_LENGTH];
    int count[20]={0}, n=0;

    for(int i=0;i<total_reminders;i++) {
        int found=0;
        for(int j=0;j<n;j++)
            if(strcmp(cats[j], reminders[i].category)==0){
                count[j]++; found=1;
            }
        if(!found){
            strcpy(cats[n], reminders[i].category);
            count[n++] = 1;
        }
    }

    printf("\nCategory | Count\n");
    printf("-------------------\n");
    for(int i=0;i<n;i++)
        printf("%-10s | %d\n", cats[i], count[i]);
}

/* Display Single Reminder */
void displayReminder(Reminder r) {
    printf("\n[%d] %s (%s)\nPriority:%d  Status:%s\nDue:%s\nNote:%s\n",
        r.id, r.title, r.category,
        r.priority,
        r.completed ? "Done":"Pending",
        r.due_date,
        r.description
    );
}

/* Display Reminder Menu */
void displayReminderMenu() {
    system("cls||clear");
    printf("SMART REMINDER PRO\n");
    printf("Total Reminders: %d\n\n", total_reminders);
    printf("1. Add New Reminder\n2. View All Reminders\n3. Mark as Completed\n4. Search Reminder\n5. Sort Reminders\n6. Categorize Reminders\n7. Save Reminders\n8. User Profile Settings\n9. Exit\n");
}

/* User Profile Functions */
void saveProfile(struct UserProfile user) {
    FILE *fp = fopen("user_profile.dat", "wb");
    if (fp == NULL) {
        printf("Error saving profile!\n");
        return;
    }
    fwrite(&user, sizeof(user), 1, fp);
    fclose(fp);
}

struct UserProfile loadProfile() {
    struct UserProfile user;
    FILE *fp = fopen("user_profile.dat", "rb");

    if (fp == NULL) {
        // Default profile
        strcpy(user.username, "Guest");
        strcpy(user.theme, "Light");
        return user;
    }
    fread(&user, sizeof(user), 1, fp);
    fclose(fp);
    return user;
}

void updateProfile() {
    struct UserProfile user;
    printf("\nEnter your name: ");
    scanf(" %[^\n]", user.username);

    printf("Choose Theme (Light/Dark): ");
    scanf("%s", user.theme);

    saveProfile(user);
    printf("Profile updated successfully!\n");

    // Clear input buffer
    while(getchar() != '\n');
}

void backupData() {
    printf("Backup functionality coming soon...\n");
}

void restoreData() {
    printf("Restore functionality coming soon...\n");
}

void exportData() {
    if (total_reminders == 0) {
        printf("No data to export!\n");
        return;
    }

    FILE *file = fopen("exported_reminders.txt", "w");
    if (!file) {
        printf("Error creating export file!\n");
        return;
    }

    fprintf(file, "Exported Reminders\n");
    fprintf(file, "===================\n\n");

    for(int i = 0; i < total_reminders; i++) {
        fprintf(file, "[%d] %s\n", reminders[i].id, reminders[i].title);
        fprintf(file, "Category: %s\n", reminders[i].category);
        fprintf(file, "Due: %s\n", reminders[i].due_date);
        fprintf(file, "Priority: %d\n", reminders[i].priority);
        fprintf(file, "Status: %s\n", reminders[i].completed ? "Completed" : "Pending");
        fprintf(file, "Description: %s\n", reminders[i].description);
        fprintf(file, "----------------------------------------\n");
    }

    fclose(file);
    printf("Data exported to 'exported_reminders.txt'\n");
}

void showHelp() {
    printf("\n===== HELP SECTION =====\n");
    printf("1. Add reminders with date and time\n");
    printf("2. Use backup to save data safely\n");
    printf("3. Extract data for external use\n");
    printf("4. Mark reminders as completed when done\n");
    printf("5. Use search to find specific reminders\n");
}

void showAbout() {
    printf("\n===== ABOUT =====\n");
    printf("Smart Reminder Pro\n");
    printf("Developed using C Language\n");
    printf("Version 1.0\n");
    printf("Module by: Nusrat Jahan\n");
}

void displayProfileMenu() {
    struct UserProfile user = loadProfile();
    int choice;

    do {
        printf("\n--- User Profile Menu ---\n");
        printf("Current User: %s (Theme: %s)\n", user.username, user.theme);
        printf("1. Update Profile\n");
        printf("2. Backup Data\n");
        printf("3. Restore Data\n");
        printf("4. Export Data\n");
        printf("5. Help\n");
        printf("6. About\n");
        printf("7. Return to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // Clear input buffer
        while(getchar() != '\n');

        switch (choice) {
            case 1: updateProfile(); break;
            case 2: backupData(); break;
            case 3: restoreData(); break;
            case 4: exportData(); break;
            case 5: showHelp(); break;
            case 6: showAbout(); break;
            case 7: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 7);
}

/* Input Helpers */
int getValidIntInput(const char* p, int min, int max) {
    int v;
    char b[50];
    while(1){
        printf("%s", p);
        fgets(b, sizeof(b), stdin);
        if(sscanf(b,"%d",&v)==1 && v>=min && v<=max) return v;
        printf("Invalid! Enter a number between %d and %d: ", min, max);
    }
}

void getStringInput(const char* p, char* b, int m) {
    printf("%s", p);
    fgets(b, m, stdin);
    b[strcspn(b,"\n")] = 0;
}

/* Main Function */
int main() {
    loadReminders();
    struct UserProfile currentUser = loadProfile();
    int choice;

    printf("Welcome to Smart Reminder Pro, %s!\n", currentUser.username);

    do {
        displayReminderMenu();
        choice = getValidIntInput("Enter your choice: ", 1, 9);

        switch(choice) {
            case 1: addReminder(); break;
            case 2: viewAllReminders(); break;
            case 3: markAsCompleted(); break;
            case 4: searchReminder(); break;
            case 5:
                for (int i = 0; i < total_reminders; i++)
                    updatePriority(&reminders[i]);
                sortReminders();
                break;
            case 6: categorizeReminder(); break;
            case 7:
                saveReminders();
                printf("Reminders saved successfully!\n");
                break;
            case 8: displayProfileMenu(); break;
            case 9:
                saveReminders();
                printf("\nExiting Smart Reminder Pro. Goodbye, %s!\n", currentUser.username);
                break;
        }

        if (choice != 9) {
            printf("\nPress Enter to continue...");
            getchar();
        }
    } while(choice != 9);

    return 0;
}
