#include "FunctionsModule.h"
#include <iostream>
#include <cstring>

using namespace std;

// --- Constructor ---
FunctionsModule::FunctionsModule
(
    Course* allCourses,
    int* courseCount,
    Faculty* allFaculty,
    int* facultyCount
)
    :
    courses(allCourses),
    numCourses(courseCount),
    faculty(allFaculty),
    numFaculty(facultyCount)
{
    // Initialize
}

// --- Check Injectivity (One-to-One) ---
// Rule: No two different courses are assigned to the same faculty.
// f(c1) = f(c2) => c1 = c2.
bool FunctionsModule::isFacultyAssignmentInjective()
{
    int i = 0;
    while (i < *numCourses)
    {
        int j = i + 1;
        while (j < *numCourses)
        {
            // If the same facultyId is assigned to two different courses (i != j)
            if (courses[i].facultyId != 0 &&
                courses[j].facultyId != 0 &&
                courses[i].facultyId == courses[j].facultyId)
            {
                cout << "Violation: " << courses[i].courseCode << " and " << courses[j].courseCode
                    << " are both assigned to Faculty ID " << courses[i].facultyId << ".\n";
                return false;
            }
            j++;
        }
        i++;
    }
    return true;
}

// --- Check Surjectivity (Onto) ---
// Rule: Every faculty member (codomain) is assigned at least one course (image).
// Image(f) = Codomain.
bool FunctionsModule::isFacultyAssignmentSurjective()
{
    // A temporary fixed array to track which faculty IDs have been assigned a course
    bool assigned[MAX_FACULTY] = { false };

    int i = 0;
    while (i < *numCourses)
    {
        int fid = courses[i].facultyId;
        int j = 0;
        while (j < *numFaculty)
        {
            if (faculty[j].base.id == fid)
            {
                assigned[j] = true;
                break;
            }
            j++;
        }
        i++;
    }

    bool result = true;
    int k = 0;
    while (k < *numFaculty)
    {
        if (assigned[k] == false)
        {
            cout << "Violation: Faculty ID " << faculty[k].base.id << " (" << faculty[k].base.name
                << ") is NOT assigned any course (Surjectivity failed).\n";
            result = false;
        }
        k++;
    }

    return result;
}

// --- Main Function Check ---
void FunctionsModule::checkCourseFacultyFunctionProperties()
{
    cout << "\n--- Function Properties: Course -> Faculty Assignment ---\n";
    cout << "1. Injectivity (One-to-One Check):\n";
    if (isFacultyAssignmentInjective())
    {
        cout << "✅ Injectivity holds: No two courses share the same faculty.\n";
    }
    else
    {
        cout << "❌ Injectivity failed: Multiple courses assigned to one faculty.\n";
    }

    cout << "\n2. Surjectivity (Onto Check):\n";
    if (isFacultyAssignmentSurjective())
    {
        cout << "✅ Surjectivity holds: Every faculty member is assigned at least one course.\n";
    }
    else
    {
        cout << "❌ Surjectivity failed: At least one faculty member is idle.\n";
    }
}