#include "SetOperationsModule.h"
#include <iostream>
#include <cstring>

using namespace std;

SetOperationsModule::SetOperationsModule
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

// --- Helper Implementation ---
bool SetOperationsModule::isStudentEnrolled
(
    Student s,
    const char* targetCode
)
{
    int i = 0;
    while (i < s.courseCount)
    {
        if (strcmp(s.enrolledCourseCodes[i], targetCode) == 0)
        {
            return true;
        }
        i++;
    }
    return false;
}

// --- Intersection (A ∩ B) ---
void SetOperationsModule::findStudentsInIntersection
(
    const char* courseCode1,
    const char* courseCode2
)
{
    cout << "\n ∩ Set Operation: Students in " << courseCode1 << " AND " << courseCode2 << " \n";

    bool found = false;
    int i = 0;
    while (i < *numStudents)
    {
        Student s = students[i];

        if (isStudentEnrolled(s, courseCode1) &&
            isStudentEnrolled(s, courseCode2))
        {
            cout << s.base.id << " - " << s.base.name << "\n";
            found = true;
        }
        i++;
    }

    if (!found)
    {
        cout << "No student found in the intersection.\n";
    }
}

// --- Union (A ∪ B) ---
void SetOperationsModule::findStudentsInUnion
(
    const char* courseCode1,
    const char* courseCode2
)
{
    cout << "\n## ∪ Set Operation: Students in " << courseCode1 << " OR " << courseCode2 << " ##\n";

    bool found = false;
    int i = 0;
    while (i < *numStudents)
    {
        Student s = students[i];

        if (isStudentEnrolled(s, courseCode1) ||
            isStudentEnrolled(s, courseCode2))
        {
            cout << s.base.id << " - " << s.base.name << "\n";
            found = true;
        }
        i++;
    }

    if (!found)
    {
        cout << "No student found in the union.\n";
    }
}