#include "ReportingModule.h"
#include <iostream>
#include <cstring>

using namespace std;

// --- External Helper Definitions (Assumed to be defined globally in Main.cpp) ---
extern Course* getCourseByCode(const char *code);
extern Faculty* getFacultyById(int id);
extern Student* getStudentById(int id);
// -------------------------------------------------------------

// --- Constructor ---
ReportingModule::ReportingModule
(
    Student *s,
    int *sc,
    Faculty *f,
    int *fc,
    Course *c,
    int *cc
)
:
    allStudents(s),
    studentCount(sc),
    allFaculty(f),
    facultyCount(fc),
    allCourses(c),
    courseCount(cc)
{
    // Initialize
}

// --- Private Reporting Functions ---

void ReportingModule::reportEnrollmentSummary()
{
    cout << "\n### 📈 Student Enrollment Summary (Function & Relations) ###\n";
    cout << "--------------------------------------------------------\n";
    if (*courseCount == 0)
    {
        cout << "No courses defined.\n";
        return;
    }

    int i = 0;
    while (i < *courseCount)
    {
        Course *c = &allCourses[i];
        
        // Find assigned faculty (Function: Course -> Faculty)
        Faculty *f = getFacultyById(c->facultyId);
        const char* facultyName = (f != NULL) ? f->base.name : "Unassigned";

        cout << "**" << c->courseCode << " (" << c->courseName << ")**\n";
        cout << "  - Assigned Faculty: " << facultyName << "\n";
        cout << "  - Enrollment Count: " << c->enrolledCount << " / " << MAX_ENROLLMENT << "\n";
        
        if (c->enrolledCount > 0)
        {
            cout << "  - Enrolled Students:\n";
            int j = 0;
            while (j < c->enrolledCount)
            {
                Student *s = getStudentById(c->studentEnrollmentIds[j]);
                if (s != NULL)
                {
                    cout << "    * " << s->base.name << " (ID: " << s->base.id << ")\n";
                }
                j++;
            }
        }
        cout << "\n";
        i++;
    }
}

void ReportingModule::reportFacultyAssignments()
{
    cout << "\n### 👨‍🏫 Faculty Assignment Summary (Domain/Codomain) ###\n";
    cout << "------------------------------------------------------\n";
    if (*facultyCount == 0)
    {
        cout << "No faculty defined.\n";
        return;
    }

    int i = 0;
    while (i < *facultyCount)
    {
        Faculty *f = &allFaculty[i];
        cout << "**" << f->base.name << " (ID: " << f->base.id << ")**\n";
        cout << "  - Total Courses: " << f->courseCount << "\n";
        
        if (f->courseCount > 0)
        {
            cout << "  - Assigned Course Codes:\n";
            int j = 0;
            while (j < f->courseCount)
            {
                cout << "    * " << f->assignedCourseCodes[j] << "\n";
                j++;
            }
        }
        cout << "\n";
        i++;
    }
}

void ReportingModule::reportCoursePrerequisites()
{
    cout << "\n### 🔗 Course Prerequisites (Relations/Partial Order) ###\n";
    cout << "-------------------------------------------------------\n";
    if (*courseCount == 0)
    {
        cout << "No courses defined.\n";
        return;
    }

    int i = 0;
    while (i < *courseCount)
    {
        Course *c = &allCourses[i];
        cout << "**" << c->courseCode << " (" << c->courseName << ")**\n";
        
        if (c->prereqCount > 0)
        {
            cout << "  - Requires:\n";
            int j = 0;
            while (j < c->prereqCount)
            {
                Course *p = getCourseByCode(c->prereqCodes[j]);
                const char* prereqName = (p != NULL) ? p->courseName : "MISSING COURSE";
                cout << "    * " << c->prereqCodes[j] << " (" << prereqName << ")\n";
                j++;
            }
        }
        else
        {
            cout << "  - No prerequisites.\n";
        }
        cout << "\n";
        i++;
    }
}

void ReportingModule::reportSetIntersections()
{
    cout << "\n### 🧩 Enrollment Intersections (Set Theory) ###\n";
    cout << "-----------------------------------------------\n";
    
    if (*courseCount < 2)
    {
        cout << "Requires at least two courses to check intersections.\n";
        return;
    }

    // Outer loop selects Course A
    int i = 0;
    while (i < *courseCount)
    {
        Course *c1 = &allCourses[i];
        
        // Inner loop selects Course B, starting from A's successor to avoid duplicates and self-check
        int j = i + 1;
        while (j < *courseCount)
        {
            Course *c2 = &allCourses[j];

            int intersectionIds[MAX_ENROLLMENT];
            int intersectionCount = 0;

            // Find intersection: Students in C1 who are also in C2
            int k = 0;
            while (k < c1->enrolledCount)
            {
                int studentId1 = c1->studentEnrollmentIds[k];
                
                // Check if studentId1 exists in c2's enrollment list
                int l = 0;
                while (l < c2->enrolledCount)
                {
                    if (studentId1 == c2->studentEnrollmentIds[l])
                    {
                        if (intersectionCount < MAX_ENROLLMENT)
                        {
                            intersectionIds[intersectionCount] = studentId1;
                            intersectionCount++;
                        }
                        break;
                    }
                    l++;
                }
                k++;
            }

            cout << "Intersection: **{" << c1->courseCode << "} ∩ {" << c2->courseCode << "}**\n";
            cout << "  - Shared Students: " << intersectionCount << "\n";
            
            if (intersectionCount > 0)
            {
                cout << "  - List:\n";
                int m = 0;
                while (m < intersectionCount)
                {
                    Student *s = getStudentById(intersectionIds[m]);
                    if (s != NULL)
                    {
                        cout << "    * " << s->base.name << " (ID: " << s->base.id << ")\n";
                    }
                    m++;
                }
            }
            cout << "\n";
            j++;
        }
        i++;
    }
}

// --- Public Report Generator ---

void ReportingModule::generateComprehensiveReport()
{
    cout << "\n=======================================================\n";
    cout << "       📊 COMPREHENSIVE DISCRETE STRUCTURES REPORT      \n";
    cout << "=======================================================\n";

    reportEnrollmentSummary();
    reportFacultyAssignments();
    reportCoursePrerequisites();
    reportSetIntersections();
}