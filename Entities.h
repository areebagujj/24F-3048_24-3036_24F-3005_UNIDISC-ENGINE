#ifndef ENTITIES_H
#define ENTITIES_H

// --- Global Constants for Fixed Arrays ---
#define MAX_STUDENTS 100
#define MAX_FACULTY 20
#define MAX_COURSES 50
#define MAX_ROOMS 10
#define MAX_PREREQS 5
#define MAX_ENROLLMENT 50
#define MAX_NAME_LENGTH 30
#define MAX_CODE_LENGTH 10
#define MAX_RELATION_SIZE MAX_COURSES // Used for matrices

// --- Core Entity Structures ---

struct Course
{
    char courseCode[MAX_CODE_LENGTH];
    char courseName[MAX_NAME_LENGTH];
    int creditHours;
    char prereqCodes[MAX_PREREQS][MAX_CODE_LENGTH];
    int prereqCount;
    int facultyId; // ID of the assigned faculty
    int studentEnrollmentIds[MAX_ENROLLMENT];
    int enrolledCount;
};

struct Room
{
    char roomCode[MAX_CODE_LENGTH];
    int capacity;
    bool available;
};

struct Person
{
    int id;
    char name[MAX_NAME_LENGTH];
};

struct Student
{
    Person base;
    char enrolledCourseCodes[MAX_COURSES][MAX_CODE_LENGTH];
    int courseCount;
};

struct Faculty
{
    Person base;
    char assignedCourseCodes[MAX_COURSES][MAX_CODE_LENGTH];
    int courseCount;
};

#endif // ENTITIES_H