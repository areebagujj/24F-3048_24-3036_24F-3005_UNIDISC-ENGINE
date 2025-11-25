#include "RelationsModule.h"
#include <iostream>
#include <cstring>

using namespace std;

// --- Constructor ---
RelationsModule::RelationsModule
(
    Course* allCourses,
    int* courseCount
)
    :
    courses(allCourses),
    numCourses(courseCount)
{
    // Initialize matrices to zero
    int i = 0;
    int j = 0;
    while (i < MAX_RELATION_SIZE)
    {
        j = 0;
        while (j < MAX_RELATION_SIZE)
        {
            relationMatrix[i][j] = 0;
            transitiveClosure[i][j] = 0;
            j++;
        }
        i++;
    }
}

// --- Helper: Find Array Index from Course Code ---
int RelationsModule::getCourseIndex
(
    const char* code
)
{
    int i = 0;
    while (i < *numCourses)
    {
        if (strcmp(courses[i].courseCode, code) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}

// --- 1. Build Relation Matrix ---
void RelationsModule::buildPrerequisiteRelation()
{
    int i = 0;
    int j = 0;
    while (i < *numCourses) // i is the dependent course
    {
        Course dependentCourse = courses[i];

        j = 0;
        while (j < dependentCourse.prereqCount)
        {
            int prereqIndex = getCourseIndex(dependentCourse.prereqCodes[j]);
            if (prereqIndex != -1)
            {
                // relationMatrix[prereqIndex][i] = 1 means Course[prereqIndex] -> Course[i]
                relationMatrix[prereqIndex][i] = 1;
            }
            j++;
        }
        i++;
    }
}

// --- 2. Warshall's Algorithm for Transitive Closure ---
void RelationsModule::computeTransitiveClosure()
{
    // Copy relationMatrix to transitiveClosure
    int i = 0;
    int j = 0;
    while (i < *numCourses)
    {
        j = 0;
        while (j < *numCourses)
        {
            transitiveClosure[i][j] = relationMatrix[i][j];
            j++;
        }
        i++;
    }

    // Apply Warshall's Algorithm
    int k = 0;
    while (k < *numCourses)
    {
        i = 0;
        while (i < *numCourses)
        {
            j = 0;
            while (j < *numCourses)
            {
                // T[i][j] = T[i][j] V (T[i][k] ^ T[k][j])
                if (transitiveClosure[i][k] == 1 && transitiveClosure[k][j] == 1)
                {
                    transitiveClosure[i][j] = 1;
                }
                j++;
            }
            i++;
        }
        k++;
    }
}

// --- 3. Property Check: Anti-symmetric ---
bool RelationsModule::isAntiSymmetric()
{
    int i = 0;
    int j = 0;
    while (i < *numCourses)
    {
        j = 0;
        while (j < *numCourses)
        {
            if (i != j && relationMatrix[i][j] == 1 && relationMatrix[j][i] == 1)
            {
                cout << "Violation: Cycle detected (Anti-symmetry failed between "
                    << courses[i].courseCode << " and " << courses[j].courseCode << ").\n";
                return false;
            }
            j++;
        }
        i++;
    }
    return true;
}

// --- 4. Property Check: Irreflexive (No course is a direct prereq for itself) ---
bool RelationsModule::isIrreflexive()
{
    int i = 0;
    while (i < *numCourses)
    {
        if (relationMatrix[i][i] == 1)
        {
            cout << "Violation: Course " << courses[i].courseCode << " is a direct prerequisite for itself.\n";
            return false;
        }
        i++;
    }
    return true;
}

// --- 5. Main Consistency Check (Partial Order) ---
bool RelationsModule::checkPartialOrderProperties()
{
    cout << "\n--- Partial Order Relation Check (Prerequisites) ---\n";

    if (!isIrreflexive())
    {
        return false;
    }

    if (!isAntiSymmetric())
    {
        return false;
    }

    cout << "✅ Direct Prerequisite relation is consistent (Irreflexive & Anti-symmetric).\n";
    return true;
}

// --- 6. Cyclic Dependency (Indirect Conflict Check) ---
bool RelationsModule::checkCyclicDependency()
{
    computeTransitiveClosure();

    cout << "\n--- Cyclic Dependency Check (Indirect Conflicts) ---\n";

    int i = 0;
    while (i < *numCourses)
    {
        if (transitiveClosure[i][i] == 1)
        {
            cout << "❌ Cyclic Dependency Detected! Course " << courses[i].courseCode
                << " is an indirect prerequisite for itself.\n";
            return true;
        }
        i++;
    }

    cout << "✅ No indirect cyclic dependencies found (Valid Partial Order).\n";
    return false;
}

// --- 7. Transitive Closure Getter ---
int RelationsModule::getTransitiveClosureValue(int i, int j)
{
    return transitiveClosure[i][j];
}