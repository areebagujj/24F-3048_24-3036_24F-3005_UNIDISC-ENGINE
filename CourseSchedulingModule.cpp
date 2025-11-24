#include "CourseSchedulingModule.h"
#include <iostream>
#include <cstring>

using namespace std;

// --- Helper Definitions (must be linked from Main.cpp or defined here) ---
// Assuming getStudentById and getCourseByCode helpers are available globally or passed in.
// For simplicity, defining Course helper here:
Course* getCourseByCode(Course* courses, int count, const char* code)
{
    int i = 0;
    while (i < count)
    {
        if (strcmp(courses[i].courseCode, code) == 0)
        {
            return &(courses[i]);
        }
        i++;
    }
    return NULL;
}
// --------------------------------------------------------------------------

// --- Constructor ---
CourseSchedulingModule::CourseSchedulingModule
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

// --- 1. Prerequisite Check Helper ---
bool CourseSchedulingModule::arePrerequisitesMet
(
    const char* courseCode,
    char completedCourses[][MAX_CODE_LENGTH],
    int completedCount
)
{
    Course* c = getCourseByCode(courses, *numCourses, courseCode);
    if (c == NULL) return true; // Assume unknown courses have met prereqs

    int i = 0;
    while (i < c->prereqCount)
    {
        bool found = false;
        int j = 0;
        while (j < completedCount)
        {
            if (strcmp(c->prereqCodes[i], completedCourses[j]) == 0)
            {
                found = true;
                break;
            }
            j++;
        }

        if (!found)
        {
            return false; // Missing a prerequisite
        }
        i++;
    }
    return true;
}

// --- 2. Sequence Generation (Recursion/Backtracking) ---
void CourseSchedulingModule::sequenceRecursive
(
    int studentId,
    char sequence[][MAX_CODE_LENGTH],
    int sequenceLength,
    char remainingCourses[][MAX_CODE_LENGTH],
    int remainingCount
)
{
    if (remainingCount == 0)
    {
        // Base Case: All courses sequenced. Print result.
        cout << "Sequence: ";
        int i = 0;
        while (i < sequenceLength)
        {
            cout << sequence[i];
            if (i < sequenceLength - 1)
            {
                cout << " -> ";
            }
            i++;
        }
        cout << "\n";
        return;
    }

    int i = 0;
    while (i < remainingCount)
    {
        const char* nextCourse = remainingCourses[i];

        // Check feasibility (prerequisites must be met by courses currently in the sequence)
        if (arePrerequisitesMet(nextCourse, sequence, sequenceLength))
        {
            // --- 1. Choose (Place the course into the sequence) ---
            strcpy_s(sequence[sequenceLength], nextCourse);

            // --- 2. Prepare Remaining Set ---
            char newRemaining[MAX_COURSES][MAX_CODE_LENGTH];
            int newRemainingCount = 0;
            int j = 0;
            while (j < remainingCount)
            {
                if (j != i)
                {
                    strcpy_s(newRemaining[newRemainingCount], remainingCourses[j]);
                    newRemainingCount++;
                }
                j++;
            }

            // --- 3. Explore (Recursive call) ---
            sequenceRecursive(studentId, sequence, sequenceLength + 1, newRemaining, newRemainingCount);

            // --- 4. Unchoose (Backtrack - sequence is overwritten in the next iteration) ---
            // Not strictly necessary for fixed array/copying remaining list, but good practice.
        }
        i++;
    }
}

void CourseSchedulingModule::generateValidCourseSequence(int studentId)
{
    // Find student's enrolled courses to sequence
    Student* s = NULL;
    int k = 0;
    while (k < *numStudents)
    {
        if (students[k].base.id == studentId)
        {
            s = &(students[k]);
            break;
        }
        k++;
    }

    if (s == NULL || s->courseCount == 0)
    {
        cout << "Student not found or not enrolled in any courses.\n";
        return;
    }

    cout << "\n--- Valid Course Sequences for Student " << s->base.id << " ---\n";
    char initialSequence[MAX_COURSES][MAX_CODE_LENGTH];
    sequenceRecursive(studentId, initialSequence, 0, s->enrolledCourseCodes, s->courseCount);
}


// --- 3. Combination Generation (Project Groups) ---
void CourseSchedulingModule::combinationRecursive
(
    char studentsInCourse[][MAX_NAME_LENGTH],
    int totalStudents,
    int groupSize,
    int startStudentIndex,
    char currentGroup[][MAX_NAME_LENGTH],
    int currentGroupIndex
)
{
    if (currentGroupIndex == groupSize)
    {
        // Base Case: Group size reached. Print the combination.
        cout << "Group: ";
        int i = 0;
        while (i < groupSize)
        {
            cout << currentGroup[i];
            if (i < groupSize - 1)
            {
                cout << ", ";
            }
            i++;
        }
        cout << "\n";
        return;
    }

    // Stop searching if remaining students are not enough to complete the group
    if (startStudentIndex >= totalStudents)
    {
        return;
    }

    int i = startStudentIndex;
    while (i < totalStudents)
    {
        // 1. Choose: Include the current student
        strcpy_s(currentGroup[currentGroupIndex], studentsInCourse[i]);

        // 2. Explore: Recurse for the next slot, starting from the next student (i+1)
        combinationRecursive(studentsInCourse, totalStudents, groupSize, i + 1, currentGroup, currentGroupIndex + 1);

        // 3. Unchoose: Backtrack (not needed as combination is overwritten, but conceptually present)
        i++;
    }
}

void CourseSchedulingModule::generateProjectGroups
(
    const char* courseCode,
    int groupSize
)
{
    Course* c = getCourseByCode(courses, *numCourses, courseCode);
    if (c == NULL)
    {
        cout << "Error: Course not found.\n";
        return;
    }

    if (c->enrolledCount < groupSize)
    {
        cout << "Not enough students enrolled (" << c->enrolledCount << ") for group size " << groupSize << ".\n";
        return;
    }

    // Get the names of the enrolled students
    char enrolledNames[MAX_ENROLLMENT][MAX_NAME_LENGTH];
    int enrolledCount = 0;

    int i = 0;
    while (i < c->enrolledCount)
    {
        int studentId = c->studentEnrollmentIds[i];
        Student* s = NULL;
        int k = 0;
        while (k < *numStudents)
        {
            if (students[k].base.id == studentId)
            {
                s = &(students[k]);
                break;
            }
            k++;
        }
        if (s != NULL)
        {
            strcpy_s(enrolledNames[enrolledCount], s->base.name);
            enrolledCount++;
        }
        i++;
    }

    cout << "\n--- Project Group Combinations for " << courseCode << " (Size " << groupSize << ") ---\n";
    char currentGroup[MAX_ENROLLMENT][MAX_NAME_LENGTH];
    combinationRecursive(enrolledNames, enrolledCount, groupSize, 0, currentGroup, 0);
}