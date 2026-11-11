
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 50
#define DEPT_LEN 30
#define DATA_FILE "students.dat"

typedef struct {
    int regNo;
    char name[NAME_LEN];
    char department[DEPT_LEN];
    float cgpa;
} Student;

typedef struct Node {
    Student data;
    struct Node *left;
    struct Node *right;
} Node;


static void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard */
    }
}

static void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            clearInputBuffer();
        }
    } else {
        buffer[0] = '\0';
    }
}

static int readInt(const char *prompt) {
    char line[100];
    char *end;
    long value;

    for (;;) {
        readLine(prompt, line, sizeof(line));
        value = strtol(line, &end, 10);
        while (isspace((unsigned char)*end)) end++;

        if (line[0] != '\0' && *end == '\0' && value > 0 && value <= 2147483647L) {
            return (int)value;
        }
        printf("Please enter a valid positive integer.\n");
    }
}

static float readCGPA(void) {
    char line[100];
    char *end;
    float value;

    for (;;) {
        readLine("Enter CGPA (0.0 - 10.0): ", line, sizeof(line));
        value = strtof(line, &end);
        while (isspace((unsigned char)*end)) end++;

        if (line[0] != '\0' && *end == '\0' && value >= 0.0f && value <= 10.0f) {
            return value;
        }
        printf("Please enter a valid CGPA between 0 and 10.\n");
    }
}


static Node *createNode(Student student) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    node->data = student;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static Node *insertNode(Node *root, Student student, int *inserted) {
    if (root == NULL) {
        *inserted = 1;
        return createNode(student);
    }

    if (student.regNo < root->data.regNo) {
        root->left = insertNode(root->left, student, inserted);
    } else if (student.regNo > root->data.regNo) {
        root->right = insertNode(root->right, student, inserted);
    } else {
        *inserted = 0;
    }
    return root;
}

static Node *searchNode(Node *root, int regNo) {
    while (root != NULL) {
        if (regNo == root->data.regNo) return root;
        root = (regNo < root->data.regNo) ? root->left : root->right;
    }
    return NULL;
}

static Node *findMinimum(Node *root) {
    while (root != NULL && root->left != NULL) {
        root = root->left;
    }
    return root;
}

static Node *deleteNode(Node *root, int regNo, int *deleted) {
    if (root == NULL) return NULL;

    if (regNo < root->data.regNo) {
        root->left = deleteNode(root->left, regNo, deleted);
    } else if (regNo > root->data.regNo) {
        root->right = deleteNode(root->right, regNo, deleted);
    } else {
        Node *temp;
        *deleted = 1;

        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }

        temp = findMinimum(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data.regNo, deleted);
    }
    return root;
}

static void displayHeader(void) {
    printf("\n%-10s %-25s %-20s %-8s\n",
           "Reg. No.", "Student Name", "Department", "CGPA");
    printf("---------------------------------------------------------------------\n");
}

static void displayStudent(Student s) {
    printf("%-10d %-25s %-20s %-8.2f\n",
           s.regNo, s.name, s.department, s.cgpa);
}

static void inorderDisplay(Node *root) {
    if (root == NULL) return;
    inorderDisplay(root->left);
    displayStudent(root->data);
    inorderDisplay(root->right);
}

static int countNodes(Node *root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

static void copyToArray(Node *root, Student arr[], int *index) {
    if (root == NULL) return;
    copyToArray(root->left, arr, index);
    arr[(*index)++] = root->data;
    copyToArray(root->right, arr, index);
}



static void insertionSortByCGPA(Student arr[], int n) {
    int i, j;
    Student key;

    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 &&
               (arr[j].cgpa < key.cgpa ||
                (arr[j].cgpa == key.cgpa && arr[j].regNo > key.regNo))) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}


static void saveToFile(Node *root) {
    FILE *fp = fopen(DATA_FILE, "wb");
    int n = countNodes(root);
    Student *arr = NULL;
    int index = 0;

    if (fp == NULL) {
        printf("Could not open %s for saving.\n", DATA_FILE);
        return;
    }

    if (n > 0) {
        arr = (Student *)malloc((size_t)n * sizeof(Student));
        if (arr == NULL) {
            printf("Memory allocation failed while saving.\n");
            fclose(fp);
            return;
        }
        copyToArray(root, arr, &index);
        if (fwrite(arr, sizeof(Student), (size_t)n, fp) != (size_t)n) {
            printf("Warning: not all records could be saved.\n");
        }
        free(arr);
    }

    fclose(fp);
    printf("%d student record(s) saved to %s.\n", n, DATA_FILE);
}

static Node *loadFromFile(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Student student;
    Node *root = NULL;
    int inserted;

    if (fp == NULL) {
        return NULL; /* First run: no data file yet. */
    }

    while (fread(&student, sizeof(Student), 1, fp) == 1) {
        root = insertNode(root, student, &inserted);
    }

    fclose(fp);
    return root;
}


static void addStudent(Node **root) {
    Student s;
    int inserted = 0;

    s.regNo = readInt("Enter register number: ");
    if (searchNode(*root, s.regNo) != NULL) {
        printf("A student with this register number already exists.\n");
        return;
    }

    readLine("Enter student name: ", s.name, sizeof(s.name));
    readLine("Enter department: ", s.department, sizeof(s.department));
    s.cgpa = readCGPA();

    *root = insertNode(*root, s, &inserted);
    if (inserted) {
        printf("Student record added successfully.\n");
        saveToFile(*root);
    }
}

static void searchStudent(Node *root) {
    int regNo = readInt("Enter register number to search: ");
    Node *found = searchNode(root, regNo);

    if (found == NULL) {
        printf("Student record not found.\n");
    } else {
        displayHeader();
        displayStudent(found->data);
    }
}

static void updateStudent(Node *root) {
    int regNo = readInt("Enter register number to update: ");
    Node *found = searchNode(root, regNo);

    if (found == NULL) {
        printf("Student record not found.\n");
        return;
    }

    printf("Enter updated details:\n");
    readLine("Enter student name: ", found->data.name, sizeof(found->data.name));
    readLine("Enter department: ", found->data.department, sizeof(found->data.department));
    found->data.cgpa = readCGPA();

    printf("Student record updated successfully.\n");
    /* The BST key (register number) is unchanged. */
    saveToFile(root);
}

static void removeStudent(Node **root) {
    int regNo = readInt("Enter register number to delete: ");
    int deleted = 0;

    *root = deleteNode(*root, regNo, &deleted);
    if (deleted) {
        printf("Student record deleted successfully.\n");
        saveToFile(*root);
    } else {
        printf("Student record not found.\n");
    }
}

static void displayAllStudents(Node *root) {
    if (root == NULL) {
        printf("No student records available.\n");
        return;
    }
    displayHeader();
    inorderDisplay(root);
    printf("\nTotal students: %d\n", countNodes(root));
}

static void displaySortedByCGPA(Node *root) {
    int n = countNodes(root);
    int index = 0;
    Student *arr;

    if (n == 0) {
        printf("No student records available.\n");
        return;
    }

    arr = (Student *)malloc((size_t)n * sizeof(Student));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    copyToArray(root, arr, &index);
    insertionSortByCGPA(arr, n);

    printf("\nStudents sorted by CGPA (highest to lowest):\n");
    displayHeader();
    for (int i = 0; i < n; i++) {
        displayStudent(arr[i]);
    }
    free(arr);
}

static void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

static void showMenu(void) {
    printf("\n========== STUDENT RECORD MANAGEMENT SYSTEM ==========\n");
    printf("1. Add Student Record\n");
    printf("2. Search Student Record\n");
    printf("3. Update Student Record\n");
    printf("4. Delete Student Record\n");
    printf("5. Display All Students (Register Number Order)\n");
    printf("6. Sort Students by CGPA (Insertion Sort)\n");
    printf("7. Exit\n");
    printf("======================================================\n");
}

int main(void) {
    Node *root = loadFromFile();
    int choice;

    printf("Welcome to Student Record Management System!\n");
    printf("%d existing record(s) loaded.\n", countNodes(root));

    for (;;) {
        showMenu();
        choice = readInt("Enter your choice (1-7): ");

        switch (choice) {
            case 1: addStudent(&root); break;
            case 2: searchStudent(root); break;
            case 3: updateStudent(root); break;
            case 4: removeStudent(&root); break;
            case 5: displayAllStudents(root); break;
            case 6: displaySortedByCGPA(root); break;
            case 7:
                saveToFile(root);
                freeTree(root);
                printf("Thank you for using the system. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please select 1 to 7.\n");
        }
    }
}