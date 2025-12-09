#include <stdio.h>





/* Optional: delete reminder (not requested but handy) */
void delete_reminder(ReminderList *list) {
    int id;
    printf("Enter reminder ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        flush_input();
        return;
    }
    flush_input();
    for (size_t i = 0; i < list->size; ++i) {
        if (list->data[i].id == id) {
            // Move last into position i
            list->data[i] = list->data[list->size - 1];
            list->size--;
            printf("Reminder %d deleted.\n", id);
            return;
        }
    }
    printf("No reminder found with ID %d.\n", id);
}

/* Seed sample reminders (for quick testing) */
void seed_sample(ReminderList *list) {
    Reminder r;
    r.id = list->next_id++;
    strncpy(r.title, "Project meeting", TITLE_LEN);
    strncpy(r.category, "Work", CAT_LEN);
    strncpy(r.description, "Discuss module integration", DESC_LEN);
    r.year = 2025; r.month = 12; r.day = 05; r.hour = 10; r.minute = 30; r.completed = 0;
    ensure_capacity(list); list->data[list->size++] = r;

    r.id = list->next_id++;
    strncpy(r.title, "Buy groceries", TITLE_LEN);
    strncpy(r.category, "Personal", CAT_LEN);
    strncpy(r.description, "Milk, eggs, rice", DESC_LEN);
    r.year = 2025; r.month = 12; r.day = 02; r.hour = 18; r.minute = 00; r.completed = 0;
    ensure_capacity(list); list->data[list->size++] = r;

    r.id = list->next_id++;
    strncpy(r.title, "Dentist appointment", TITLE_LEN);
    strncpy(r.category, "Health", CAT_LEN);
    strncpy(r.description, "Annual check-up", DESC_LEN);
    r.year = 2025; r.month = 12; r.day = 15; r.hour = 14; r.minute = 00; r.completed = 0;
    ensure_capacity(list); list->data[list->size++] = r;
}

/* --- Main menu --- */
void print_menu(void) {
    puts("\n--- Smart Reminder Pro ---");
    puts("1. Add reminder");
    puts("2. View all reminders");
    puts("3. View reminders by category");
    puts("4. Search reminders");
    puts("5. Mark reminder as completed");
    puts("6. Sort reminders by date/time");
    puts("7. Delete reminder (optional)");
    puts("8. Seed sample reminders (for tests)");
    puts("0. Exit");
    printf("Choose an option: ");
}

int main() {
    ReminderList list;
    init_list(&list);

    int choice;
    for (;;) { case used .
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            flush_input();
            continue;
        }
        flush_input();
        switch (choice) {
            case 1: add_reminder(&list); break;
            case 2: view_all(&list); break;
            case 3: view_by_category(&list); break;
            case 4: search_reminders(&list); break;
            case 5: mark_completed(&list); break;
            case 6: sort_reminders(&list); break;
            case 7: delete_reminder(&list); break;
            case 8: seed_sample(&list); printf("Sample reminders added.\n"); break;
            case 0: free_list(&list); printf("Goodbye!\n"); return 0;
            default: printf("Unknown option. Try again.\n"); break;
        }
    }








    return 0;
}
