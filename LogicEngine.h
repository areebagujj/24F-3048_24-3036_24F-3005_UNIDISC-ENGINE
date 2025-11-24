#ifndef LOGIC_ENGINE_H
#define LOGIC_ENGINE_H
#include "Entities.h"

class LogicEngine
{
public:
    LogicEngine
    (
        Student* allStudents,
        int* studentCount,
        Faculty* allFaculty,
        int* facultyCount
    );

    // Rule 1: Student cannot enroll in the same course twice (¬(P ∧ P))
    bool verifyStudentEnrollmentUniqueness
    (
        int studentId,
        const char* newCourseCode
    );

    // Rule 2: Faculty cannot be assigned the same course twice
    bool verifyFacultyAssignmentUniqueness
    (
        int facultyId,
        const char* newCourseCode
    );

private:
    Student* students;
    int* numStudents;
    Faculty* faculty;
    int* numFaculty;
};

#endif // LOGIC_ENGINE_H