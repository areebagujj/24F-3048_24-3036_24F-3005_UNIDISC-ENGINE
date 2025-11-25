#include "Entities.h"
#include "LogicEngine.h"
#include "SetOperationsModule.h"
#include "RelationsModule.h"
#include "InductionModule.h"
#include "FunctionsModule.h"
#include "CourseSchedulingModule.h"
#include "StudentGroupModule.h" 
#include "ReportingModule.h" 

#include <iostream>
#include <cstring>
#include <limits> 

using namespace std;

// --- Global Fixed-Size Data Arrays (The Universe Sets) ---
Student allStudents[MAX_STUDENTS];
int studentCount = 0;

Faculty allFaculty[MAX_FACULTY];
int facultyCount = 0;

Course allCourses[MAX_COURSES];
int courseCount = 0;

Room allRooms[MAX_ROOMS];
int roomCount = 0;

// =========================================================
//                  GLOBAL HELPER FUNCTIONS (Used by all modules)
// =========================================================

Student* getStudentById(int id)
{
    int i = 0;
    while (i < studentCount)
    {
        if (allStudents[i].base.id == id)
        {
            return &(allStudents[i]);
        }
        i++;
    }
    return NULL;
}

Faculty* getFacultyById(int id)
{
    int i = 0;
    while (i < facultyCount)
    {
        if (allFaculty[i].base.id == id)
        {
            return &(allFaculty[i]);
        }
        i++;
    }
    return NULL;
}

Course* getCourseByCode(const char* code)
{
    int i = 0;
    while (i < courseCount)
    {
        if (strcmp(allCourses[i].courseCode, code) == 0)
        {
            return &(allCourses[i]);
        }
        i++;
    }
    return NULL;
}

void assignCourseToStudent(int studentId, const char* courseCode)
{
    Student* s = getStudentById(studentId);
    Course* c = getCourseByCode(courseCode);

    if (s != NULL && c != NULL && s->courseCount < MAX_COURSES && c->enrolledCount < MAX_ENROLLMENT)
    {
        // Check uniqueness
        int i = 0;
        bool alreadyEnrolled = false;
        while (i < s->courseCount)
        {
            if (strcmp(s->enrolledCourseCodes[i], courseCode) == 0)
            {
                alreadyEnrolled = true;
                break;
            }
            i++;
        }

        if (alreadyEnrolled)
        {
            cout << "Assignment Failed: Student is already enrolled in " << courseCode << ".\n";
            return;
        }

        // Perform assignment
        strcpy_s(s->enrolledCourseCodes[s->courseCount], courseCode);
        s->courseCount++;
        c->studentEnrollmentIds[c->enrolledCount] = studentId;
        c->enrolledCount++;
        cout << "Assignment Successful.\n";
    }
    else
    {
        cout << "Assignment Failed: Invalid ID, Code, or capacity reached.\n";
    }
}

void assignCourseToFaculty(int facultyId, const char* courseCode)
{
    Faculty* f = getFacultyById(facultyId);
    Course* c = getCourseByCode(courseCode);

    if (f != NULL && c != NULL && f->courseCount < MAX_COURSES)
    {
        // Check uniqueness
        int i = 0;
        bool alreadyAssigned = false;
        while (i < f->courseCount)
        {
            if (strcmp(f->assignedCourseCodes[i], courseCode) == 0)
            {
                alreadyAssigned = true;
                break;
            }
            i++;
        }

        if (alreadyAssigned)
        {
            cout << "Assignment Failed: Faculty is already assigned to " << courseCode << ".\n";
            return;
        }

        // Perform assignment
        strcpy_s(f->assignedCourseCodes[f->courseCount], courseCode);
        f->courseCount++;
        c->facultyId = facultyId;
        cout << "Assignment Successful.\n";
    }
    else
    {
        cout << "Assignment Failed: Invalid ID, Code, or capacity reached.\n";
    }
}

// =========================================================
//                   ADMIN MANUAL DATA INPUT
// =========================================================

void addCourseManual()
{
    if (courseCount >= MAX_COURSES)
    {
        cout << "Error: Max course limit reached.\n";
        return;
    }

    char code[MAX_CODE_LENGTH];
    char name[MAX_NAME_LENGTH];
    char prereqCode[MAX_CODE_LENGTH];
    int prereqCount = 0;

    cout << "\n--- Add New Course ---\n";
    cout << "Enter Course Code (e.g., CS101): ";
    cin >> code;

    if (getCourseByCode(code) != NULL)
    {
        cout << "Error: Course Code already exists.\n";
        return;
    }

    cout << "Enter Course Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(name, MAX_NAME_LENGTH);

    // Set basic data
    strcpy_s(allCourses[courseCount].courseCode, code);
    strcpy_s(allCourses[courseCount].courseName, name);
    allCourses[courseCount].creditHours = 3;
    allCourses[courseCount].enrolledCount = 0;
    allCourses[courseCount].facultyId = 0;

    cout << "Enter Prerequisites (Type 'done' to finish, max " << MAX_PREREQS << "):\n";
    while (prereqCount < MAX_PREREQS)
    {
        cout << "Prerequisite " << prereqCount + 1 << " (Code): ";
        cin >> prereqCode;

        if (strcmp(prereqCode, "done") == 0)
        {
            break;
        }

        if (getCourseByCode(prereqCode) == NULL)
        {
            cout << "Warning: Prerequisite course " << prereqCode << " does not exist yet.\n";
        }

        strcpy_s(allCourses[courseCount].prereqCodes[prereqCount], prereqCode);
        prereqCount++;
    }

    allCourses[courseCount].prereqCount = prereqCount;
    courseCount++;
    cout << "Course " << code << " added successfully. Current count: " << courseCount << "\n";
}

void addStudentManual()
{
    if (studentCount >= MAX_STUDENTS)
    {
        cout << "Error: Max student limit reached.\n";
        return;
    }

    int id;
    char name[MAX_NAME_LENGTH];

    cout << "\n--- Add New Student ---\n";
    cout << "Enter Student ID (e.g., 1001): ";
    cin >> id;

    if (getStudentById(id) != NULL)
    {
        cout << "Error: Student ID already exists.\n";
        return;
    }

    cout << "Enter Student Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(name, MAX_NAME_LENGTH);

    allStudents[studentCount].base.id = id;
    strcpy_s(allStudents[studentCount].base.name, name);
    allStudents[studentCount].courseCount = 0;
    studentCount++;

    cout << "Student " << name << " added successfully with ID " << id << ". Current count: " << studentCount << "\n";
}

void addFacultyManual()
{
    if (facultyCount >= MAX_FACULTY)
    {
        cout << "Error: Max faculty limit reached.\n";
        return;
    }

    int id;
    char name[MAX_NAME_LENGTH];

    cout << "\n--- Add New Faculty ---\n";
    cout << "Enter Faculty ID (e.g., 201): ";
    cin >> id;

    if (getFacultyById(id) != NULL)
    {
        cout << "Error: Faculty ID already exists.\n";
        return;
    }

    cout << "Enter Faculty Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(name, MAX_NAME_LENGTH);

    allFaculty[facultyCount].base.id = id;
    strcpy_s(allFaculty[facultyCount].base.name, name);
    allFaculty[facultyCount].courseCount = 0;
    facultyCount++;

    cout << "Faculty " << name << " added successfully with ID " << id << ". Current count: " << facultyCount << "\n";
}

void assignCourseToStudentManual()
{
    int studentId;
    char courseCode[MAX_CODE_LENGTH];

    cout << "\n--- Enroll Student in Course ---\n";
    cout << "Enter Student ID: ";
    cin >> studentId;
    cout << "Enter Course Code: ";
    cin >> courseCode;

    assignCourseToStudent(studentId, courseCode);
}

void assignCourseToFacultyManual()
{
    int facultyId;
    char courseCode[MAX_CODE_LENGTH];

    cout << "\n--- Assign Faculty to Course ---\n";
    cout << "Enter Faculty ID: ";
    cin >> facultyId;
    cout << "Enter Course Code: ";
    cin >> courseCode;

    assignCourseToFaculty(facultyId, courseCode);
}
void identifyPrereqViolations()
{
    std::cout << "\n\n=== PREREQUISITE VIOLATION REPORT ===\n";
    bool violationsFound = false;

    // A. Iterate through all students in the system
    for (int sIdx = 0; sIdx < studentCount; sIdx++)
    {
        Student* currentStudent = &allStudents[sIdx];
        bool studentHasViolations = false;

        // B. Iterate through all courses the student is currently enrolled in
        for (int cIdx = 0; cIdx < currentStudent->courseCount; cIdx++)
        {
            const char* enrolledCode = currentStudent->enrolledCourseCodes[cIdx];
            Course* enrolledCourse = getCourseByCode(enrolledCode);

            // Safety check
            if (enrolledCourse == NULL) continue;

            // C. Check all prerequisites for the enrolled course
            for (int pIdx = 0; pIdx < enrolledCourse->prereqCount; pIdx++)
            {
                const char* prereqCode = enrolledCourse->prereqCodes[pIdx];
                bool prereqMet = false;

                // D. Check if the prerequisite course is in the student's enrollment list
                for (int studentCourseIdx = 0; studentCourseIdx < currentStudent->courseCount; studentCourseIdx++)
                {
                    if (strcmp(currentStudent->enrolledCourseCodes[studentCourseIdx], prereqCode) == 0)
                    {
                        prereqMet = true;
                        break;
                    }
                }

                // E. If the prerequisite was NOT found, report the violation
                if (!prereqMet)
                {
                    if (!studentHasViolations)
                    {
                        std::cout << "\n* Student ID: " << currentStudent->base.id
                            << ", Name: " << currentStudent->base.name << "\n";
                        studentHasViolations = true;
                        violationsFound = true;
                    }

                    std::cout << "  - VIOLATION: Enrolled in **" << enrolledCode
                        << "** but missing prerequisite **" << prereqCode << "**\n";
                }
            }
        }
    }

    if (!violationsFound)
    {
        std::cout << "SUCCESS: No prerequisite violations found in the system.\n";
    }
    std::cout << "=======================================\n";
}
void runInitialAdminSetup()
{
    int choice;
    do
    {
        cout << "\n\n=== INITIAL DATA SETUP (ADMIN) ===\n";
        cout << "Populate the system before running discrete structure checks.\n";
        cout << "1. Add Course\n";
        cout << "2. Add Student\n";
        cout << "3. Add Faculty\n";
        cout << "4. Enroll Student in Course\n";
        cout << "5. Assign Faculty to Course\n";
        cout << "0. Finish Setup & Start CLI\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Returning to Main Menu.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        switch (choice)
        {
        case 1: addCourseManual(); break;
        case 2: addStudentManual(); break;
        case 3: addFacultyManual(); break;
        case 4: assignCourseToStudentManual(); break;
        case 5: assignCourseToFacultyManual(); break;
        case 0: cout << "Setup complete. Starting main menu...\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// =========================================================
//                       MENU LOGIC
// =========================================================

int runMainMenu()
{
    int choice;
    cout << "\n\n=== U-N-I-D-I-S-C  E-N-G-I-N-E  ===\n";
    cout << "1. Admin Console (Structural Management)\n";
    cout << "2. Faculty Login\n";
    cout << "3. Student Login\n";
    cout << "0. Exit Program\n";
    cout << "Enter your role (0-3): ";

    if (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    return choice;
}

void runAdminMenu
(
    SetOperationsModule& setOps,
    LogicEngine& logicEng,
    RelationsModule& relMod,
    InductionModule& indMod,
    FunctionsModule& funcMod,
    CourseSchedulingModule& schedMod,
    StudentGroupModule& groupMod,
    ReportingModule& reportMod // ADDED REPORTING MODULE
)
{
    int choice;
    do
    {
        cout << "\n--- Admin Console (Discrete Structures Tools) ---\n";
        cout << "1. Logic & Inference (Uniqueness Check)\n";
        cout << "2. Set Operations (Enrollment Analysis)\n";
        cout << "3. Relations (Prerequisite Partial Order Check)\n";
        cout << "4. Induction/Strong Induction (Chain Verification)\n";
        cout << "5. Functions (Faculty Assignment Properties)\n";
        cout << "6. Course Scheduling (Sequence & Groups)\n";
        cout << "7. **Generate Comprehensive Data Report**\n"; 
        cout << "8. See Prerequisite Violations.\n";
        cout << "9. Add Course\n";
        cout << "10. Add Student\n";
        cout << "11. Add Faculty\n";
        cout << "12. Enroll Student in Course\n";
        cout << "13. Assign Faculty to Course\n";
        cout << "0. Switch Role (Back to Main Menu)\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        switch (choice)
        {
        case 1:
        {
            int sId; char cCode[MAX_CODE_LENGTH];
            cout << "Enter Student ID for uniqueness check: "; cin >> sId;
            cout << "Enter Course Code: "; cin >> cCode;
            logicEng.verifyStudentEnrollmentUniqueness(sId, cCode);
            break;
        }
        case 2:
        {
            char cCode1[MAX_CODE_LENGTH], cCode2[MAX_CODE_LENGTH];
            cout << "Enter Course Code 1 (Intersection): "; cin >> cCode1;
            cout << "Enter Course Code 2 (Intersection): "; cin >> cCode2;
            setOps.findStudentsInIntersection(cCode1, cCode2);
            break;
        }
        case 3:
            relMod.buildPrerequisiteRelation();
            relMod.checkPartialOrderProperties();
            relMod.checkCyclicDependency();
            break;
        case 4:
        {
            char pCode[MAX_CODE_LENGTH], dCode[MAX_CODE_LENGTH];
            cout << "Enter Prerequisite Course Code: "; cin >> pCode;
            cout << "Enter Dependent Course Code: "; cin >> dCode;
            indMod.verifyPrereqChainByInduction(pCode, dCode);
            break;
        }
        case 5:
            funcMod.checkCourseFacultyFunctionProperties();
            break;
        case 6:
        {
            int sId;
            cout << "Enter Student ID for Sequencing: "; cin >> sId;
            schedMod.generateValidCourseSequence(sId);

            char gCode[MAX_CODE_LENGTH]; int gSize;
            cout << "Enter Course Code for Grouping: "; cin >> gCode;
            cout << "Enter Group Size: "; cin >> gSize;
            groupMod.generateProjectGroups(gCode, gSize);
            break;
        }
        case 7: // NEW REPORTING OPTION
            reportMod.generateComprehensiveReport();
            break;
        case 8: // NEW CASE
            identifyPrereqViolations();
            break;
        case 9: addCourseManual(); break;
        case 10: addStudentManual(); break;
        case 11: addFacultyManual(); break;
        case 12: assignCourseToStudentManual(); break;
        case 13: assignCourseToFacultyManual(); break;
        case 0:
            return;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void runFacultyMenu
(
    Faculty* currentFaculty,
    StudentGroupModule& groupMod
)
{
    int choice;
    do
    {
        cout << "\n--- Faculty Portal: " << currentFaculty->base.name << " (ID: " << currentFaculty->base.id << ") ---\n";
        cout << "1. View Assigned Courses and Enrolled Students\n";
        cout << "2. **Create Student Groups (Combinations)**\n";
        cout << "0. Switch Role (Back to Main Menu)\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- Assigned Courses ---\n";
            int i = 0;
            while (i < currentFaculty->courseCount)
            {
                const char* code = currentFaculty->assignedCourseCodes[i];
                Course* c = getCourseByCode(code);

                if (c != NULL)
                {
                    cout << "- " << c->courseName << " (" << code << "): Students (" << c->enrolledCount << ")\n";
                    int j = 0;
                    while (j < c->enrolledCount)
                    {
                        Student* s = getStudentById(c->studentEnrollmentIds[j]);
                        if (s != NULL)
                        {
                            cout << "    |-- " << s->base.id << " " << s->base.name << "\n";
                        }
                        j++;
                    }
                }
                i++;
            }
            break;
        }
        case 2:
        {
            char gCode[MAX_CODE_LENGTH]; int gSize;
            cout << "Enter Course Code for Grouping: "; cin >> gCode;
            cout << "Enter Group Size: "; cin >> gSize;

            groupMod.generateProjectGroups(gCode, gSize);
            break;
        }
        case 0:
            return; // Switch Role
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void runStudentMenu
(
    Student* currentStudent
)
{
    // Need to re-instantiate modules locally as they use fixed global data
    CourseSchedulingModule schedMod(allStudents, &studentCount, allCourses, &courseCount);
    StudentGroupModule groupMod(allStudents, &studentCount, allCourses, &courseCount);

    int choice;
    do
    {
        cout << "\n--- Student Portal: " << currentStudent->base.name << " (ID: " << currentStudent->base.id << ") ---\n";
        cout << "1. View Current Courses\n";
        cout << "2. Register for a Course\n";
        cout << "3. View Valid Course Sequences\n";
        cout << "4. **View Possible Groups (Combinations)**\n";
        cout << "0. Switch Role (Back to Main Menu)\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- Current Enrolled Courses ---\n";
            int i = 0;
            while (i < currentStudent->courseCount)
            {
                const char* code = currentStudent->enrolledCourseCodes[i];
                Course* c = getCourseByCode(code);
                if (c != NULL)
                {
                    cout << "- " << c->courseName << " (" << code << ")\n";
                }
                i++;
            }
            break;
        }
        case 2:
        {
            char regCode[MAX_CODE_LENGTH];
            cout << "Enter Course Code to Register: "; cin >> regCode;

            Course* c = getCourseByCode(regCode);
            if (c == NULL)
            {
                cout << "Registration Failed: Course " << regCode << " not found.\n";
                break;
            }

            // Prerequisite check
            bool prereqsMet = true;
            int i = 0;
            while (i < c->prereqCount)
            {
                const char* prereqCode = c->prereqCodes[i];

                bool found = false;
                int j = 0;
                while (j < currentStudent->courseCount)
                {
                    if (strcmp(currentStudent->enrolledCourseCodes[j], prereqCode) == 0)
                    {
                        found = true;
                        break;
                    }
                    j++;
                }

                if (!found)
                {
                    cout << "Registration Failed: Missing prerequisite " << prereqCode << ".\n";
                    prereqsMet = false;
                    break;
                }
                i++;
            }

            if (prereqsMet)
            {
                assignCourseToStudent(currentStudent->base.id, regCode);
            }
            break;
        }
        case 3:
        {
            schedMod.generateValidCourseSequence(currentStudent->base.id);
            break;
        }
        case 4:
        {
            char gCode[MAX_CODE_LENGTH]; int gSize;
            cout << "Enter Course Code to view Group options: "; cin >> gCode;
            cout << "Enter Desired Group Size: "; cin >> gSize;

            cout << "\n--- Possible Groups for " << currentStudent->base.name << " in " << gCode << " ---\n";
            groupMod.generateProjectGroups(gCode, gSize);
            break;
        }
        case 0:
            return; // Switch Role
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// =========================================================
//                       MAIN FUNCTION
// =========================================================

int main()
{
    // Initialize Modules (They rely on global arrays/pointers)
    RelationsModule relMod(allCourses, &courseCount);
    SetOperationsModule setOps(allStudents, &studentCount, allCourses, &courseCount);
    LogicEngine logicEng(allStudents, &studentCount, allFaculty, &facultyCount);
    InductionModule indMod(allCourses, &courseCount, &relMod);
    FunctionsModule funcMod(allCourses, &courseCount, allFaculty, &facultyCount);
    CourseSchedulingModule schedMod(allStudents, &studentCount, allCourses, &courseCount);
    StudentGroupModule groupMod(allStudents, &studentCount, allCourses, &courseCount);
    // NEW: Reporting Module Initialization
    ReportingModule reportMod(allStudents, &studentCount, allFaculty, &facultyCount, allCourses, &courseCount);

    // Dynamic Data Setup: Admin populates the system arrays
    runInitialAdminSetup();

    int choice;
    int loginId;

    do
    {
        // --- Role Selection Menu ---
        choice = runMainMenu();

        if (choice == 1) // Admin
        {
            // Pass the new reportMod instance to the Admin Menu
            runAdminMenu(setOps, logicEng, relMod, indMod, funcMod, schedMod, groupMod, reportMod);
        }
        else if (choice == 2) // Faculty
        {
            cout << "Enter Faculty ID: ";
            if (cin >> loginId)
            {
                Faculty* faculty = getFacultyById(loginId);
                if (faculty != NULL)
                {
                    runFacultyMenu(faculty, groupMod);
                }
                else
                {
                    cout << "Error: Faculty ID " << loginId << " not found.\n";
                }
            }
            else
            {
                cout << "Invalid ID format. Returning to main menu.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        else if (choice == 3) // Student
        {
            cout << "Enter Student ID: ";
            if (cin >> loginId)
            {
                Student* student = getStudentById(loginId);
                if (student != NULL)
                {
                    runStudentMenu(student);
                }
                else
                {
                    cout << "Error: Student ID " << loginId << " not found.\n";
                }
            }
            else
            {
                cout << "Invalid ID format. Returning to main menu.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        else if (choice == 0)
        {
            break; // Exit program
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    cout << "Exiting Framework. Goodbye!\n";
    return 0;
}