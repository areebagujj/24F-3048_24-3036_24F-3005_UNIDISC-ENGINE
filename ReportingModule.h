#ifndef REPORTING_MODULE_H
#define REPORTING_MODULE_H

#include "Entities.h"
#include <iostream>

class ReportingModule
{
public:
    ReportingModule(Student* s, int* sc, Faculty* f, int* fc, Course* c, int* cc);

    // Main reporting function
    void generateComprehensiveReport();

private:
    Student* allStudents;
    int* studentCount;
    Faculty* allFaculty;
    int* facultyCount;
    Course* allCourses;
    int* courseCount;

    // Private Reporting Functions
    void reportEnrollmentSummary();
    void reportFacultyAssignments();
    void reportCoursePrerequisites();
    void reportSetIntersections();
};

#endif // REPORTING_MODULE_H