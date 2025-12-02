#include <stdio.h>








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
    for (;;) {
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