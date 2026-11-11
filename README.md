

A menu-driven C program that manages student records using a Binary Search Tree, with Insertion Sort for ranking students by CGPA. Done as an individual DSA mini-project and developed in Visual Studio Code.

## Table of Contents

1. [Project Description](#project-description)
2. [Objectives](#objectives)
3. [Features](#features)
4. [Technologies Used](#technologies-used)
5. [Data Structures Used](#data-structures-used)
6. [Sorting Algorithm](#sorting-algorithm)
7. [File Handling](#file-handling)
8. [Project Folder Structure](#project-folder-structure)
9. [How to Compile and Run](#how-to-compile-and-run)
10. [Sample Input and Output](#sample-input-and-output)
11. [Time and Space Complexity](#time-and-space-complexity)
12. [Advantages](#advantages)
13. [Conclusion](#conclusion)
14. [Author Details](#author-details)

Project Description

This project stores student records in a Binary Search Tree (BST). The register number is the key, so every student is identified by a unique number and the records stay in sorted order of register number. Because of this, adding, finding, updating and deleting a student is fast compared to scanning a plain list.

To see who is performing best, the program can list students by CGPA from highest to lowest. This is done with Insertion Sort. The records are also saved to a file, and the program loads them again the next time it starts, so nothing is lost when it is closed.

Objectives

- Use a Binary Search Tree to store and manage student records.
- Implement insertion, searching, deletion and inorder traversal on a BST.
- Use Insertion Sort to sort students by CGPA in descending order.
- Use file handling so the records are stored permanently.
- Understand the time and space complexity of the methods used.

Features

1. Add student records
2. Search a student using the register number
3. Update student details
4. Delete student records
5. Display all student records
6. Sort students by CGPA in descending order
7. Count the total number of students
8. Save student records to a file
9. Load previously saved records when the program starts

Technologies Used

| Item | Details |
|------|---------|
| Language | C |
| Data structure | Binary Search Tree |
| Sorting algorithm | Insertion Sort |
| Storage | Text file (file handling) |
| IDE | Visual Studio Code |
| Compilers | GCC and Microsoft Visual C (MSVC) |

## Data Structures Used

### Binary Search Tree

A Binary Search Tree is a tree where each node has at most two children, a left child and a right child. The rule is simple: everything in the left subtree has a smaller key than the node, and everything in the right subtree has a bigger key.

Here the key is the register number. A small example:

```
                 2023005
                /       \
          2023002       2023008
          /     \            \
    2023001   2023003       2023010
```

Each node in this project holds one student:

```c
typedef struct Student {
    int regNo;               // register number (BST key)
    char name[50];
    char department[30];
    float cgpa;
    struct Student *left;
    struct Student *right;
} Student;
```

### Insertion

Start at the root and compare the new register number with the current node. If it is smaller, go left. If it is bigger, go right. Keep going until you reach an empty spot and place the new node there. If the register number already exists, the record is rejected because register numbers must be unique.

### Searching

Searching follows the same path as insertion. Compare the register number with the current node. If it matches, the student is found. If it is smaller, search the left subtree, otherwise search the right subtree. If you reach NULL, the student is not in the tree.

### Deletion

There are three cases:

| Case | What to do |
|------|-----------|
| Node has no children | Remove it directly. |
| Node has one child | Replace the node with its child. |
| Node has two children | Find the inorder successor (the smallest node in the right subtree), copy its data into the node, then delete the successor. |

### Inorder Traversal

Inorder traversal visits the left subtree, then the node, then the right subtree. For a BST this gives the records in ascending order of register number. The program uses it to display records, count students, copy records into an array for sorting, and write records to the file.

```c
void inorder(Student *root) {
    if (root != NULL) {
        inorder(root->left);
        // process this student
        inorder(root->right);
    }
}
```

## Sorting Algorithm

### Insertion Sort

Insertion Sort works like sorting playing cards in your hand. Take one element at a time and insert it at the correct place among the elements before it, shifting the others to the right if needed. It is simple, sorts in place, and is stable, meaning students with equal CGPA keep their earlier relative order.

### Sorting Students by CGPA

The BST is ordered by register number, not CGPA, so the tree cannot give CGPA order directly. The steps used are:

1. Do an inorder traversal and copy all the records into an array.
2. Run Insertion Sort on the array using CGPA as the key.
3. The comparison is reversed so a higher CGPA comes first.
4. Print the sorted array.

The tree is not changed by this, so searching and the other operations work as before.

```c
void insertionSortByCGPA(Student arr[], int n) {
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j].cgpa < key.cgpa) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
```

Example with CGPA values 8.5, 9.1, 7.8, 9.4:

| Pass | Array |
|------|-------|
| Start | 8.5, 9.1, 7.8, 9.4 |
| After pass 1 | 9.1, 8.5, 7.8, 9.4 |
| After pass 2 | 9.1, 8.5, 7.8, 9.4 |
| After pass 3 | 9.4, 9.1, 8.5, 7.8 |

## File Handling

Records are kept in a text file called `students.txt` so they survive after the program is closed.

- **Saving:** the tree is traversed inorder and each student is written to the file as one line.
- **Loading:** when the program starts, the file is read line by line and each record is inserted back into the BST.
- **First run:** if the file does not exist yet, the program simply starts with an empty tree.

Each line has this format:

```
RegNo,Name,Department,CGPA
```

For example:

```
2023001,Arun Kumar,CSE,8.75
2023002,Priya Sharma,ECE,9.10
2023003,Rahul Verma,IT,7.85
```

Functions used: `fopen()`, `fprintf()`, `fscanf()`, `fclose()`.

## Project Folder Structure

```
Student-Record-Management-System/
├── main.c          # complete source code
├── students.txt    # data file, created when records are saved
└── README.md
```

## How to Compile and Run

### Using GCC

Open a terminal in the project folder (the VS Code terminal works fine).

```bash
cd Student-Record-Management-System
gcc main.c -o student_record
```

Run it on Windows:

```bash
student_record.exe
```

Run it on Linux or macOS:

```bash
./student_record
```

### Using Microsoft Visual C (MSVC)

Open the Developer Command Prompt for Visual Studio and go to the project folder.

```bat
cd path\to\Student-Record-Management-System
cl main.c /D _CRT_SECURE_NO_WARNINGS /Fe:student_record.exe
student_record.exe
```

The `/D _CRT_SECURE_NO_WARNINGS` flag stops MSVC from showing warnings about functions like `scanf` and `fopen`.

## Sample Input and Output

### Main menu

```
========== STUDENT RECORD MANAGEMENT SYSTEM ==========
1. Add Student Record
2. Search Student by Register Number
3. Update Student Details
4. Delete Student Record
5. Display All Records
6. Sort Students by CGPA (Descending)
7. Count Total Students
8. Save Records to File
9. Exit
======================================================
Enter your choice: 
```

### Adding a student

```
Enter your choice: 1
Enter Register Number : 2023001
Enter Name            : Arun Kumar
Enter Department      : CSE
Enter CGPA            : 8.75

Student record added successfully.
```

### Searching a student

```
Enter your choice: 2
Enter Register Number to search: 2023002

Student Found!
Register Number : 2023002
Name            : Priya Sharma
Department      : ECE
CGPA            : 9.10
```

### Updating a student

```
Enter your choice: 3
Enter Register Number to update: 2023003
Enter New Name       : Rahul Verma
Enter New Department : IT
Enter New CGPA       : 8.20

Student record updated successfully.
```

### Deleting a student

```
Enter your choice: 4
Enter Register Number to delete: 2023004

Student record deleted successfully.
```

### Displaying all records

```
Enter your choice: 5

---------------------------------------------------------
Reg. No      Name                 Department     CGPA
---------------------------------------------------------
2023001      Arun Kumar           CSE            8.75
2023002      Priya Sharma         ECE            9.10
2023003      Rahul Verma          IT             8.20
2023005      Sneha Reddy          EEE            9.40
2023008      Karthik Raj          MECH           7.60
---------------------------------------------------------
```

### Sorting by CGPA

```
Enter your choice: 6

Students sorted by CGPA (High to Low):

---------------------------------------------------------
Reg. No      Name                 Department     CGPA
---------------------------------------------------------
2023005      Sneha Reddy          EEE            9.40
2023002      Priya Sharma         ECE            9.10
2023001      Arun Kumar           CSE            8.75
2023003      Rahul Verma          IT             8.20
2023008      Karthik Raj          MECH           7.60
---------------------------------------------------------
```

### Counting students

```
Enter your choice: 7

Total number of students: 5
```

### Saving records

```
Enter your choice: 8

Records saved successfully to students.txt
```

### Sample student records

| Register Number | Name | Department | CGPA |
|:---------------:|------|:----------:|:----:|
| 2023001 | Arun Kumar | CSE | 8.75 |
| 2023002 | Priya Sharma | ECE | 9.10 |
| 2023003 | Rahul Verma | IT | 8.20 |
| 2023005 | Sneha Reddy | EEE | 9.40 |
| 2023008 | Karthik Raj | MECH | 7.60 |

## Time and Space Complexity

Here n is the number of students and h is the height of the tree.

| Operation | Average case | Worst case |
|-----------|:------------:|:----------:|
| Insertion | O(log n) | O(n) |
| Searching | O(log n) | O(n) |
| Update (search, then modify) | O(log n) | O(n) |
| Deletion | O(log n) | O(n) |
| Display / Inorder traversal | O(n) | O(n) |
| Count students | O(n) | O(n) |
| Insertion Sort by CGPA | O(n²) | O(n²) |
| Saving to file | O(n) | O(n) |
| Loading from file | O(n log n) | O(n²) |

The worst case for the BST happens when the tree becomes skewed, for example when register numbers are entered in already sorted order. The tree then looks like a linked list and h becomes n. Insertion Sort takes O(n) in the best case, when the data is already sorted. Loading inserts every record into the tree one by one, which is why it follows the insertion cost.

Space complexity:

| Part | Space |
|------|:-----:|
| BST with n nodes | O(n) |
| Array used for sorting | O(n) |
| Extra space inside Insertion Sort | O(1) |
| Recursion stack in BST operations | O(h) |
| Overall | O(n) |

## Advantages

- Searching by register number is fast because the BST cuts the search space at each step.
- Inorder traversal gives records sorted by register number without any extra sorting.
- Memory is allocated dynamically, so it is used only for the records that exist.
- Insertion Sort is easy to understand and implement, and it works well for small amounts of data.
- Insertion Sort is stable and in place, so it keeps the order of equal CGPAs and needs no extra space for sorting.
- Records are stored in a file, so data is not lost after the program closes.
- The menu-driven interface is simple to use.
- Only standard C libraries are used.

## Conclusion

This project uses a Binary Search Tree to manage student records and Insertion Sort to list them by CGPA in descending order. It supports adding, searching, updating, deleting, displaying and counting records, and it saves and loads the data using file handling.

Working on it helped me understand tree operations, recursion, dynamic memory allocation, sorting and file handling in C, and how the choice of data structure affects the speed of a program.

## Author Details

| Field | Details |
|-------|---------|
| Name | |
| Register Number | |
| College | |
