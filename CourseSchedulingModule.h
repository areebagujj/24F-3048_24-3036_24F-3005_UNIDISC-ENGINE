#ifndef COURSE_SCHEDULING_MODULE_H
#define COURSE_SCHEDULING_MODULE_H

#include "Entities.h"

class CourseSchedulingModule
{
public:
    CourseSchedulingModule
    (
        Student* allStudentss,
        int* studentCountt,
        Course* allCoursess,
        int* courseCountt
    );

    // Functionality: Generate all valid sequences for a student based on prereqs
    void generateValidCourseSequence(int studentId);

    // Functionality: Assign students to project groups (Combinations)
    void generateProjectGroups(const char* courseCode, int groupSize);

private:
    Student* students;
    int* numStudents;
    Course* courses;
    int* numCourses;

    // Recursive helper for generating sequences
    void sequenceRecursive
    (
        int studentId,
        char sequence[][MAX_CODE_LENGTH],
        int sequenceLength,
        char remainingCourses[][MAX_CODE_LENGTH],
        int remainingCount
    );

    // Helper to check if a course's prerequisites are met by courses in a sequence
    bool arePrerequisitesMet
    (
        const char* courseCode,
        char completedCourses[][MAX_CODE_LENGTH],
        int completedCount
    );

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

#endif // COURSE_SCHEDULING_MODULE_H