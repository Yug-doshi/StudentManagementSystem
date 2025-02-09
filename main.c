#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

void createTable(sqlite3 *db) {
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS Student ("
        "StudentID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "Name TEXT NOT NULL,"
        "MarksMaths REAL NOT NULL,"
        "MarksScience REAL NOT NULL,"
        "MarksEnglish REAL NOT NULL,"
        "Department TEXT NOT NULL);";

    char *errMsg = 0;
    int result = sqlite3_exec(db, sql, 0, 0, &errMsg);
}

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

void addStudentRecord(sqlite3 *db) {
    const char *sql = "INSERT INTO Student (Name, MarksMaths, MarksScience, MarksEnglish, Department) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    char name[100], department[50];
    double marksMaths, marksScience, marksEnglish;

    // Input validation loop for Name
    do {
        printf("Enter Student Name: ");
        scanf(" %[^\n]", name);
        if (strlen(name) == 0) {
            printf("Error: Name cannot be empty. Please enter again.\n");
        }
    } while (strlen(name) == 0);

    // Input validation loop for Marks
    do {
        printf("Enter Marks in Maths (0-100): ");
        if (scanf("%lf", &marksMaths) != 1 || marksMaths < 0 || marksMaths > 100) {
            printf("Invalid input! Marks must be between 0 and 100.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            break;
        }
    } while (1);

    do {
        printf("Enter Marks in Science (0-100): ");
        if (scanf("%lf", &marksScience) != 1 || marksScience < 0 || marksScience > 100) {
            printf("Invalid input! Marks must be between 0 and 100.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            break;
        }
    } while (1);

    do {
        printf("Enter Marks in English (0-100): ");
        if (scanf("%lf", &marksEnglish) != 1 || marksEnglish < 0 || marksEnglish > 100) {
            printf("Invalid input! Marks must be between 0 and 100.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            break;
        }
    } while (1);

    // Input validation loop for Department
    do {
        printf("Enter Department: ");
        scanf(" %[^\n]", department);
        if (strlen(department) == 0) {
            printf("Error: Department cannot be empty. Please enter again.\n");
        }
    } while (strlen(department) == 0);

    // Prepare SQL statement
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (result != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, marksMaths);
    sqlite3_bind_double(stmt, 3, marksScience);
    sqlite3_bind_double(stmt, 4, marksEnglish);
    sqlite3_bind_text(stmt, 5, department, -1, SQLITE_TRANSIENT);

    // Execute statement
    result = sqlite3_step(stmt);
    if (result == SQLITE_DONE) {
        printf("Student record added successfully!\n");
    } else {
        printf("Error adding record: %s\n", sqlite3_errmsg(db));
    }

    // Finalize statement
    sqlite3_finalize(stmt);
}


void displayStudentRecords(sqlite3 *db) {
    const char *sql = "SELECT StudentID, Name, MarksMaths, MarksScience, MarksEnglish, Department FROM Student;";
    sqlite3_stmt *stmt;

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (result != SQLITE_OK) {
        printf("Failed to fetch records: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n================================================================================\n");
    printf("| %-5s | %-20s | %-10s | %-10s | %-10s | %-15s |\n", 
           "ID", "Name", "Maths", "Science", "English", "Department");
    printf("================================================================================\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        double marksMaths = sqlite3_column_double(stmt, 2);
        double marksScience = sqlite3_column_double(stmt, 3);
        double marksEnglish = sqlite3_column_double(stmt, 4);
        const char *department = (const char*)sqlite3_column_text(stmt, 5);

        printf("| %-5d | %-20s | %-10.2f | %-10.2f | %-10.2f | %-15s |\n",
               id, name, marksMaths, marksScience, marksEnglish, department);
    }

    printf("================================================================================\n");
    sqlite3_finalize(stmt);
}

void deleteStudentByID(sqlite3 *db) {
    const char *sql = "DELETE FROM Student WHERE StudentID = ?;";
    sqlite3_stmt *stmt;
    int id;

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    result = sqlite3_step(stmt);

    if (result == SQLITE_DONE) {
        printf("Record deleted successfully.\n");
    } else {
        printf("Failed to delete record: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

void updateStudentDetails(sqlite3 *db) {
    const char *sql = "UPDATE Student SET MarksMaths = ?, MarksScience = ?, MarksEnglish = ?, Department = ? WHERE StudentID = ?;";
    sqlite3_stmt *stmt;
    int id;
    double marksMaths, marksScience, marksEnglish;
    char department[50];

    printf("Enter Student ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input! Please enter a valid Student ID.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    // Input marks with validation
    printf("Enter new Marks in Maths (0-100): ");
    if (scanf("%lf", &marksMaths) != 1 || marksMaths < 0 || marksMaths > 100) {
        printf("Invalid Marks! Must be between 0 and 100.\n");
        return;
    }

    printf("Enter new Marks in Science (0-100): ");
    if (scanf("%lf", &marksScience) != 1 || marksScience < 0 || marksScience > 100) {
        printf("Invalid Marks! Must be between 0 and 100.\n");
        return;
    }

    printf("Enter new Marks in English (0-100): ");
    if (scanf("%lf", &marksEnglish) != 1 || marksEnglish < 0 || marksEnglish > 100) {
        printf("Invalid Marks! Must be between 0 and 100.\n");
        return;
    }

    printf("Enter new Department: ");
    scanf(" %[^\n]", department);

    // Check if department is empty
    if (strlen(department) == 0) {
        printf("Department cannot be empty!\n");
        return;
    }

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_double(stmt, 1, marksMaths);
    sqlite3_bind_double(stmt, 2, marksScience);
    sqlite3_bind_double(stmt, 3, marksEnglish);
    sqlite3_bind_text(stmt, 4, department, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, id);

    result = sqlite3_step(stmt);

    if (result == SQLITE_DONE && sqlite3_changes(db) > 0) {
        printf("Record updated successfully.\n");
    } else {
        printf("Failed to update record. Either no student found or an error occurred: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}




void searchStudentByID(sqlite3 *db) {
    const char *sql = "SELECT StudentID, Name, MarksMaths, MarksScience, MarksEnglish, Department FROM Student WHERE StudentID = ?;";
    sqlite3_stmt *stmt;
    int id;

    printf("Enter Student ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input! Please enter a valid Student ID.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW) {
        printf("\n--------------------------------------------------------------\n");
        printf("| %-10s | %-20s | %-8s | %-8s | %-8s | %-15s |\n", 
               "ID", "Name", "Maths", "Science", "English", "Department");
        printf("------------------------------------------------------------------------------\n");
        printf("| %-10d | %-20s | %-8.2f | %-8.2f | %-8.2f | %-15s |\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_double(stmt, 2),
               sqlite3_column_double(stmt, 3),
               sqlite3_column_double(stmt, 4),
               sqlite3_column_text(stmt, 5));
        printf("------------------------------------------------------------------------------\n");
    } else {
        printf("\nNo student found with ID %d.\n", id);
    }

    sqlite3_finalize(stmt);
}

void findTopStudent(sqlite3 *db) {
    const char *sql = 
        "SELECT StudentID, Name, Department, (MarksMaths + MarksScience + MarksEnglish) AS TotalMarks "
        "FROM Student ORDER BY TotalMarks DESC LIMIT 1;";
    
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        printf("Error finding top student: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        const char *department = (const char*)sqlite3_column_text(stmt, 2);
        double totalMarks = sqlite3_column_double(stmt, 3);

        printf("\nTop Student:\n");
        printf("ID          : %d\n", id);
        printf("Name        : %s\n", name);
        printf("Department  : %s\n", department);
        printf("Total Marks : %.2f\n", totalMarks);
    } else {
        printf("\nNo student records found.\n");
    }

    sqlite3_finalize(stmt);
}

// Function to count total students
void countStudents(sqlite3 *db) {
    const char *sql = "SELECT COUNT(*) FROM Student;";
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    int totalStudents = 0;

    if (result == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        totalStudents = sqlite3_column_int(stmt, 0);
    }

    printf("\nTotal Number of Students: %d\n", totalStudents);
    sqlite3_finalize(stmt);
}

// Function to find the average marks of all students
void findAverageMarks(sqlite3 *db) {
    const char *sql = "SELECT AVG(MarksMaths + MarksScience + MarksEnglish) FROM Student;";
    sqlite3_stmt *stmt;
    double avgMarks = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        avgMarks = sqlite3_column_double(stmt, 0);
    }

    printf("\nAverage Marks of All Students: %.2f\n", avgMarks);
    sqlite3_finalize(stmt);
}

void reverseStudentList(sqlite3 *db) {
    const char *sql = "SELECT StudentID, Name, MarksMaths, MarksScience, MarksEnglish, Department "
                      "FROM Student ORDER BY StudentID DESC;";
    
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        printf("Failed to fetch reversed list: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nReversed Student List:\n");
    printf("===========================================================\n");
    printf("| ID   | Name                | Maths | Science | English | Department  |\n");
    printf("===========================================================\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("| %-4d | %-20s | %-5.2f | %-7.2f | %-7.2f | %-12s |\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_double(stmt, 2),
               sqlite3_column_double(stmt, 3),
               sqlite3_column_double(stmt, 4),
               sqlite3_column_text(stmt, 5));
    }

    printf("===========================================================\n");
    sqlite3_finalize(stmt);
}


#include <stdio.h>
#include <sqlite3.h>

void sortStudentsByMarks(sqlite3 *db, int order) {
    const char *sql;
    
    if (order == 1) {
        sql = "SELECT StudentID, Name, MarksMaths, MarksScience, MarksEnglish, Department, "
              "(MarksMaths + MarksScience + MarksEnglish) AS TotalMarks "
              "FROM Student ORDER BY TotalMarks ASC;";
        printf("\nStudents Sorted by Marks (Ascending Order):\n");
    } else {
        sql = "SELECT StudentID, Name, MarksMaths, MarksScience, MarksEnglish, Department, "
              "(MarksMaths + MarksScience + MarksEnglish) AS TotalMarks "
              "FROM Student ORDER BY TotalMarks DESC;";
        printf("\nStudents Sorted by Marks (Descending Order):\n");
    }
    
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        printf("Failed to fetch sorted list: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("===================================================================================\n");
    printf("| ID   | Name                | Maths | Science | English | Total Marks | Department  |\n");
    printf("===================================================================================\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("| %-4d | %-20s | %-5.2f | %-7.2f | %-7.2f | %-11.2f | %-12s |\n",
               sqlite3_column_int(stmt, 0),              // StudentID
               sqlite3_column_text(stmt, 1),            // Name
               sqlite3_column_double(stmt, 2),          // MarksMaths
               sqlite3_column_double(stmt, 3),          // MarksScience
               sqlite3_column_double(stmt, 4),          // MarksEnglish
               sqlite3_column_double(stmt, 6),          // **Corrected TotalMarks index**
               sqlite3_column_text(stmt, 5));           // Department
    }

    printf("===================================================================================\n");
    sqlite3_finalize(stmt);
}







int main() {
    sqlite3 *db;
    int result = sqlite3_open("student.db", &db);

    if (result) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    createTable(db);

    int choice;
    do {
        printf("\n\n\n\n");
        printf("============================================\n");
        printf("      Student Record Management System      \n");
        printf("============================================\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Student Records\n");
        printf("3. Search for a Student by ID\n");
        printf("4. Delete a Student Record by ID\n");
        printf("5. Update Student Details\n");
        printf("6. Find the Student with the Highest Marks\n");
        printf("7. Count the Total Number of Students\n");
        printf("8. Reverse the Student List\n");
        printf("9. Sort Students by Marks (Ascending/Descending)\n");
        printf("10. Find Average Marks of All Students\n");
        printf("0. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudentRecord(db); break;
            case 2: displayStudentRecords(db); break;
            case 3:searchStudentByID(db); break;
            case 4: deleteStudentByID(db); break;
            case 5: updateStudentDetails(db); break;
            case 6: findTopStudent(db); break;
            case 7: countStudents(db); break;
            case 8: reverseStudentList(db); break;
            case 9:
            {
                printf("\nPress 1 for Ascending");
                printf("\nPress 2 for descending");
                printf("\n\nEnter Your choice");
                int ch;
                printf("Enter your choice:-");
                scanf("%d",&ch);
                if(ch==1)
                    sortStudentsByMarks(db, 1);  // For Ascending Order
                if(ch==2)
                    sortStudentsByMarks(db, 0);  // For Descending Order
                else    
                    printf("Invalid choice");
                break;
            }
            case 10: findAverageMarks(db); break;
            case 0:exit;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    sqlite3_close(db);
    return 0;
}
