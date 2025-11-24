#include "LogicEngine.h"
#include <iostream>
#include <cstring>

using namespace std;

// --- Helper Functions (Defined here for simplicity in context) ---
Student* getStudentById(Student* students, int count, int id)
{
    int i = 0;
    while (i < count)
    {
        if (students[i].base.id == id)
        {
            return &(students[i]);
        }
        i++;
    }
    return NULL;
}

Faculty* getFacultyById(Faculty* faculty, int count, int id)
{
    int i = 0;
    while (i < count)
    {
        if (faculty[i].base.id == id)
        {
            return &(faculty[i]);
        }
        i++;
    }
    return NULL;
}
// --- End Helper Functions ---

LogicEngine::LogicEngine
(
    Student* allStudents,
    int* studentCount,
    Faculty* allFaculty,
    int* facultyCount
)
    :
    students(allStudents),
    numStudents(studentCount),
    faculty(allFaculty),
    numFaculty(facultyCount)
{
    // Initialize
}

// Rule: Student cannot enroll in the same course twice (¬(P ∧ P))
bool LogicEngine::verifyStudentEnrollmentUniqueness
(
    int studentId,
    const char* newCourseCode
)
{
    Student* s = getStudentById(students, *numStudents, studentId);

    if (s == NULL)
    {
        return false;
    }

    int i = 0;
    while (i < s->courseCount)
    {
        if (strcmp(s->enrolledCourseCodes[i], newCourseCode) == 0)
        {
            cout << "Logic Violation: Student " << s->base.name << " is ALREADY enrolled in " << newCourseCode << ".\n";
            return false;
        }
        i++;
    }

    return true;
}

// Rule: Faculty cannot be assigned the same course twice
bool LogicEngine::verifyFacultyAssignmentUniqueness
(
    int facultyId,
    const char* newCourseCode
)
{
    Faculty* f = getFacultyById(faculty, *numFaculty, facultyId);

    if (f == NULL)
    {
        return false;
    }

    int i = 0;
    while (i < f->courseCount)
    {
        if (strcmp(f->assignedCourseCodes[i], newCourseCode) == 0)
        {
            cout << "Logic Violation: Faculty " << f->base.name << " is ALREADY assigned to " << newCourseCode << ".\n";
            return false;
        }
        i++;
    }

    return true;
}