#ifndef SET_OPERATIONS_MODULE_H
#define SET_OPERATIONS_MODULE_H

#include "Entities.h"

class SetOperationsModule
{
public:
    SetOperationsModule
    (
        Student* allStudents,
        int* studentCount,
        Course* allCourses,
        int* courseCount
    );

    // Functionality: Find Students in (Course A) ∩ (Course B)
    void findStudentsInIntersection
    (
        const char* courseCode1,
        const char* courseCode2
    );

    // Functionality: Find Students in (Course A) ∪ (Course B)
    void findStudentsInUnion
    (
        const char* courseCode1,
        const char* courseCode2
    );

private:
    Student* students;
    int* numStudents;
    Course* courses;
    int* numCourses;

    // Helper to check if a student is enrolled in a course
    bool isStudentEnrolled
    (
        Student s,
        const char* targetCode
    );
};

#endif // SET_OPERATIONS_MODULE_H