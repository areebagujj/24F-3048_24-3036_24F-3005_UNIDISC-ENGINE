#ifndef STUDENT_GROUP_MODULE_H
#define STUDENT_GROUP_MODULE_H

#include "Entities.h"

class StudentGroupModule
{
public:
    StudentGroupModule
    (
        Student* allStudents,
        int* studentCount,
        Course* allCourses,
        int* courseCount
    );

    // Functionality: Assign students to project groups (Combinations)
    void generateProjectGroups
    (
        const char* courseCode,
        int groupSize
    );

private:
    Student* students;
    int* numStudents;
    Course* courses;
    int* numCourses;

    // Helper for combinations (grouping) - uses recursion/fixed array logic
    void combinationRecursive
    (
        char studentsInCourse[][MAX_NAME_LENGTH],
        int totalStudents,
        int groupSize,
        int startStudentIndex,
        char currentGroup[][MAX_NAME_LENGTH],
        int currentGroupIndex
    );
};

#endif // STUDENT_GROUP_MODULE_H