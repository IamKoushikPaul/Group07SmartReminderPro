#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REMINDERS 100

/* ================= USER ================= */
typedef struct {
    char username[30];
    char password[30];
} User;

User currentUser;

/* ================= REMINDER ================= */
typedef struct {
    int id;
    char title[50];
    char category[20];
    int priority;
    int completed;
    int recurrence;
    time_t due_time;
    time_t created_at;
} Reminder;

Reminder reminders[MAX_REMINDERS];
int total_reminders = 0;

/* ================= FILES ================= */
#define USER_FILE "users.dat"
#define REM_FILE "reminders.dat"
#define BACKUP_FILE "backup.dat"
#define EXPORT_FILE "export.txt"
#define MISSED_FILE "missed_log.txt"

/* ================= USER SYSTEM ================= */
void signup() {
    FILE *fp = fopen(USER_FILE, "ab");
    printf("Create Username: ");
    scanf("%s", currentUser.username);
    printf("Create Password: ");
    scanf("%s", currentUser.password);
    fwrite(&currentUser, sizeof(User), 1, fp);
    fclose(fp);
    printf("Signup successful!\n");
}

int login() {
    FILE *fp = fopen(USER_FILE, "rb");
    if (!fp) return 0;

    char u[30], p[30];
    printf("Username: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);

    User temp;
    while (fread(&temp, sizeof(User), 1, fp)) {
        if (!strcmp(temp.username, u) && !strcmp(temp.password, p)) {
            currentUser = temp;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ================= FILE HANDLING ================= */
void saveReminders() {
    FILE *fp = fopen(REM_FILE, "wb");
    fwrite(reminders, sizeof(Reminder), total_reminders, fp);
    fclose(fp);
}

void loadReminders() {
    FILE *fp = fopen(REM_FILE, "rb");
    if (!fp) return;
    total_reminders = fread(reminders, sizeof(Reminder), MAX_REMINDERS, fp);
    fclose(fp);
}

void sortReminders() {
    for (int i = 0; i < total_reminders - 1; i++) {
        for (int j = 0; j < total_reminders - i - 1; j++) {

            int swap = 0;

            // Category compare
            int catCmp = strcmp(reminders[j].category, reminders[j+1].category);

            if (catCmp > 0)
                swap = 1;
            else if (catCmp == 0) {
                // Time compare
                if (reminders[j].due_time > reminders[j+1].due_time)
                    swap = 1;
                else if (reminders[j].due_time == reminders[j+1].due_time &&
                         reminders[j].priority < reminders[j+1].priority)
                    swap = 1;
            }

            if (swap) {
                Reminder temp = reminders[j];
                reminders[j] = reminders[j+1];
                reminders[j+1] = temp;
            }
        }
    }
}




/* ================= BACKUP / RESTORE / EXPORT ================= */
void backupData() {
    FILE *b = fopen(BACKUP_FILE, "wb");
    fwrite(&currentUser, sizeof(User), 1, b);
    fwrite(&total_reminders, sizeof(int), 1, b);
    fwrite(reminders, sizeof(Reminder), total_reminders, b);
    fclose(b);
    printf("Backup completed successfully!\n");
}

void restoreData() {
    FILE *b = fopen(BACKUP_FILE, "rb");
    if (!b) {
        printf("No backup found!\n");
        return;
    }
    fread(&currentUser, sizeof(User), 1, b);
    fread(&total_reminders, sizeof(int), 1, b);
    fread(reminders, sizeof(Reminder), total_reminders, b);
    fclose(b);
    printf("Data restored successfully!\n");
}

void formatTime(time_t t, char *buffer) {
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 40, "%d-%m-%Y %I:%M %p", tm_info);
}

void exportData() {
    FILE *fp = fopen("exported_reminders.txt", "w");
    if (!fp) {
        printf("Export failed!\n");
        return;
    }

    sortReminders();   // sorted by date & priority

    fprintf(fp, "SMART REMINDER PRO - EXPORTED DATA\n");
    fprintf(fp, "================================\n\n");

    char timeBuffer[40];

    for (int i = 0; i < total_reminders; i++) {

        if (i == 0 || strcmp(reminders[i].category, reminders[i - 1].category) != 0) {
            fprintf(fp, "\nCategory: %s\n", reminders[i].category);
            fprintf(fp, "----------------------------\n");
        }

        formatTime(reminders[i].due_time, timeBuffer);

        fprintf(fp,
            "Title: %s\n"
            "Priority: %d\n"
            "Status: %s\n"
            "Due: %s\n\n",
            reminders[i].title,
            reminders[i].priority,
            reminders[i].completed ? "Completed" : "Pending",
            timeBuffer
        );
    }

    fclose(fp);
    printf("Export completed successfully.\n");
}



/* ================= CORE FEATURES ================= */
void addReminder() {
    if (total_reminders >= MAX_REMINDERS) return;

    Reminder *r = &reminders[total_reminders];
    r->id = total_reminders + 1;
    r->completed = 0;
    r->created_at = time(NULL);

    int d, m, y, hh, mm;
    struct tm t = {0};

    printf("Title: ");
    scanf(" %[^\n]", r->title);

    printf("Category: ");
    scanf("%s", r->category);

    printf("Priority (1-5): ");
    scanf("%d", &r->priority);

    printf("Enter Date (DD MM YYYY): ");
    scanf("%d %d %d", &d, &m, &y);

    printf("Enter Time (HH MM, 24-hour): ");
    scanf("%d %d", &hh, &mm);

    t.tm_mday = d;
    t.tm_mon  = m - 1;     // months start from 0
    t.tm_year = y - 1900;  // years since 1900
    t.tm_hour = hh;
    t.tm_min  = mm;
    t.tm_sec  = 0;

    r->due_time = mktime(&t);

    if (r->due_time == -1) {
        printf("Invalid date/time!\n");
        return;
    }

    total_reminders++;
    printf("Reminder added successfully!\n");
}

void viewAllReminders() {
    printf("\n--- ALL REMINDERS ---\n");
    for (int i = 0; i < total_reminders; i++) {
        printf("%d | %s | %s | P%d | %s\n",
               reminders[i].id,
               reminders[i].title,
               reminders[i].category,
               reminders[i].priority,
               reminders[i].completed ? "Done" : "Pending");
    }
}