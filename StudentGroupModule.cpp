#include "StudentGroupModule.h"
#include <iostream>
#include <cstring>

using namespace std;

// --- Helper Definitions (Must match definition in Main.cpp) ---
Course* getCourseByCode(Course* courses, int count, const char* code);
Student* getStudentById(Student* students, int count, int id);
// -------------------------------------------------------------

// --- Constructor ---
StudentGroupModule::StudentGroupModule
(
    Student* allStudents,
    int* studentCount,
    Course* allCourses,
    int* courseCount
)
    :
    students(allStudents),
    numStudents(studentCount),
    courses(allCourses),
    numCourses(courseCount)
{
    // Initialize
}

// --- Combination Generation (Project Groups) ---
void StudentGroupModule::combinationRecursive
(
    char studentsInCourse[][MAX_NAME_LENGTH],
    int totalStudents,
    int groupSize,
    int startStudentIndex,
    char currentGroup[][MAX_NAME_LENGTH],
    int currentGroupIndex
)
{
    if (currentGroupIndex == groupSize)
    {
        // Base Case: Group size reached. Print the combination.
        cout << "Group: ";
        int i = 0;
        while (i < groupSize)
        {
            cout << currentGroup[i];
            if (i < groupSize - 1)
            {
                cout << ", ";
            }
            i++;
        }
        cout << "\n";
        return;
    }

    // Stop searching if remaining students are not enough to complete the group
    if (startStudentIndex >= totalStudents)
    {
        return;
    }

    int i = startStudentIndex;
    while (i < totalStudents)
    {
        // 1. Choose: Include the current student
        strcpy_s(currentGroup[currentGroupIndex], studentsInCourse[i]);

        // 2. Explore: Recurse for the next slot, starting from the next student (i+1)
        combinationRecursive(studentsInCourse, totalStudents, groupSize, i + 1, currentGroup, currentGroupIndex + 1);

        // 3. Unchoose: Backtrack
        i++;
    }
}

void StudentGroupModule::generateProjectGroups
(
    const char* courseCode,
    int groupSize
)
{
    Course* c = getCourseByCode(courses, *numCourses, courseCode);
    if (c == NULL)
    {
        cout << "Error: Course " << courseCode << " not found.\n";
        return;
    }

    if (c->enrolledCount < groupSize)
    {
        cout << "Not enough students enrolled (" << c->enrolledCount << ") for group size " << groupSize << ".\n";
        return;
    }

    // Get the names of the enrolled students
    char enrolledNames[MAX_ENROLLMENT][MAX_NAME_LENGTH];
    int enrolledCount = 0;

    int i = 0;
    while (i < c->enrolledCount)
    {
        int studentId = c->studentEnrollmentIds[i];
        Student* s = getStudentById(students, *numStudents, studentId);

        if (s != NULL)
        {
            strcpy_s(enrolledNames[enrolledCount], s->base.name);
            enrolledCount++;
        }
        i++;
    }

    cout << "\n--- Project Group Combinations for " << courseCode << " (Size " << groupSize << ") ---\n";
    // 
    char currentGroup[MAX_ENROLLMENT][MAX_NAME_LENGTH];
    combinationRecursive(enrolledNames, enrolledCount, groupSize, 0, currentGroup, 0);
}