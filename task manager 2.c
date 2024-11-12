#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100      // Maximum number of tasks
#define FILE_NAME "tasks.txt"

typedef struct {
    char taskText[100];   // Task description
    char date[11];        // Task due date: YYYY-MM-DD
    int isCompleted;      // 0 = not completed, 1 = completed
    int isActive;         // 0 = deleted, 1 = active
    char priority[10];    // Priority: High, Medium, Low
} Task;

Task tasks[MAX_TASKS];      // Array to store tasks
int taskCount = 0;          // Total number of tasks

// Function to load tasks from the file
void loadTasksFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;  // If file doesn't exist, return

    while (taskCount < MAX_TASKS) {
        char c;
        int i = 0;

        // Read task description (taskText)
        while ((c = fgetc(file)) != EOF && c != '\n' && i < 99) {
            tasks[taskCount].taskText[i++] = c;
        }
        tasks[taskCount].taskText[i] = '\0';

        // Read task date
        i = 0;
        while ((c = fgetc(file)) != EOF && c != '\n' && i < 10) {
            tasks[taskCount].date[i++] = c;
        }
        tasks[taskCount].date[i] = '\0';

        // Read priority
        i = 0;
        while ((c = fgetc(file)) != EOF && c != '\n' && i < 9) {
            tasks[taskCount].priority[i++] = c;
        }
        tasks[taskCount].priority[i] = '\0';

        // Read completion status and active status
        if (fscanf(file, "%d %d", &tasks[taskCount].isCompleted, &tasks[taskCount].isActive) != 2) {
            break;
        }

        taskCount++;
    }

    fclose(file);
}

// Function to save tasks to the file
void saveTasksToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].isActive) {
            fputs(tasks[i].taskText, file);
            fputc(' ', file);
            fputs(tasks[i].date, file);
            fputc(' ', file);
            fputs(tasks[i].priority, file);
            fputc(' ', file);
            fprintf(file, "%d %d\n", tasks[i].isCompleted, tasks[i].isActive);
        }
    }

    fclose(file);
}

// Function to add a task
void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Cannot add more tasks. Task list is full.\n");
        return;
    }

    printf("Enter task description (up to 99 characters): ");
    int i = 0;
    char c;
    while ((c = getchar()) != '\n' && i < 99) {
        tasks[taskCount].taskText[i++] = c;
    }
    tasks[taskCount].taskText[i] = '\0';

    printf("Enter task due date (YYYY-MM-DD): ");
    i = 0;
    while ((c = getchar()) != '\n' && i < 10) {
        tasks[taskCount].date[i++] = c;
    }
    tasks[taskCount].date[i] = '\0';

    printf("Enter task priority (High, Medium, Low): ");
    i = 0;
    while ((c = getchar()) != '\n' && i < 9) {
        tasks[taskCount].priority[i++] = c;
    }
    tasks[taskCount].priority[i] = '\0';

    tasks[taskCount].isCompleted = 0;
    tasks[taskCount].isActive = 1;  // Mark this task as active

    taskCount++;
    printf("Task added!\n");

    saveTasksToFile();  // Save the task list to file
}

// Function to view all tasks
void viewTasks() {
    char currentDate[11];
    printf("Enter current date (YYYY-MM-DD): ");
    int i = 0;
    char c;
    while ((c = getchar()) != '\n' && i < 10) {
        currentDate[i++] = c;
    }
    currentDate[i] = '\0';

    printf("\n=== To-Do List ===\n");
    for (i = 0; i < taskCount; i++) {
        if (tasks[i].isActive) {
            printf("%d. %s (Due: %s, Priority: %s) [%s]",
                   i + 1, tasks[i].taskText, tasks[i].date, tasks[i].priority,
                   tasks[i].isCompleted ? "Completed" : "Not Completed");

            if (!tasks[i].isCompleted && strcmp(currentDate, tasks[i].date) > 0) {
                printf(" - Due Date Passed");
            }
            printf("\n");
        }
    }
}

// Function to mark a task as complete
void markComplete() {
    int taskNum;
    viewTasks();
    printf("Enter task number to mark as complete: ");
    scanf("%d", &taskNum);

    if (taskNum > 0 && taskNum <= taskCount && tasks[taskNum - 1].isActive) {
        tasks[taskNum - 1].isCompleted = 1;
        printf("Task marked as complete!\n");

        saveTasksToFile();  // Save the changes to file
    } else {
        printf("Invalid task number!\n");
    }
}

// Function to delete a task
void deleteTask() {
    int taskNum;
    viewTasks();
    printf("Enter task number to delete: ");
    scanf("%d", &taskNum);

    if (taskNum > 0 && taskNum <= taskCount && tasks[taskNum - 1].isActive) {
        tasks[taskNum - 1].isActive = 0;
        printf("Task deleted!\n");

        saveTasksToFile();  // Save the changes to file
    } else {
        printf("Invalid task number!\n");
    }
}

// Main program
int main() {
    loadTasksFromFile();  // Load tasks from the file at startup

    int choice;
    do {
        printf("\n1. Add Task\n2. View Tasks\n3. Mark Task as Complete\n4. Delete Task\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume the newline character left by scanf

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: markComplete(); break;
            case 4: deleteTask(); break;
            case 5: printf("Exiting... Goodbye!\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
