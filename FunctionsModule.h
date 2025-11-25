#ifndef FUNCTIONS_MODULE_H
#define FUNCTIONS_MODULE_H

#include "Entities.h"

class FunctionsModule
{
public:
    FunctionsModule
    (
        Course* allCourses,
        int* courseCount,
        Faculty* allFaculty,
        int* facultyCount
    );

    // Checks properties of the mapping f: Course -> Faculty (Assignment)
    void checkCourseFacultyFunctionProperties();

private:
    Course* courses;
    int* numCourses;
    Faculty* faculty;
    int* numFaculty;

    // Helper: Checks if f(c1) = f(c2) implies c1 = c2
    bool isFacultyAssignmentInjective();

    // Helper: Checks if every faculty member is assigned at least one course
    bool isFacultyAssignmentSurjective();
};

#endif // FUNCTIONS_MODULES_H