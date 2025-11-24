#include "InductionModule.h"
#include <iostream>
#include <cstring>

using namespace std;

// --- Constructor ---
InductionModule::InductionModule
(
    Course* allCourses,
    int* courseCount,
    RelationsModule* relMod
)
    :
    courses(allCourses),
    numCourses(courseCount),
    relModule(relMod)
{
    // Initialize
}

// --- Helper: Find Array Index from Course Code ---
int InductionModule::getCourseIndex
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

// --- Main Induction/Strong Induction Verification ---
// P(k): The prerequisite chain of length k from Course A to Course B is valid.
// Strong Induction is verified here by checking the Transitive Closure.
bool InductionModule::verifyPrereqChainByInduction
(
    const char* prereqCode,
    const char* dependentCode
)
{
    cout << "\n--- Strong Induction Verification: Prerequisite Chain ---\n";
    cout << "Checking if " << prereqCode << " is required for " << dependentCode << " (directly or indirectly).\n";

    int i_prereq = getCourseIndex(prereqCode);
    int j_dependent = getCourseIndex(dependentCode);

    if (i_prereq == -1 || j_dependent == -1)
    {
        cout << "Error: One or both course codes are invalid.\n";
        return false;
    }

    // Must build the relation first to ensure Transitive Closure is computed
    relModule->buildPrerequisiteRelation();

    // The Transitive Closure T[i][j] = 1 proves that there is a path (a chain of length 1 or more)
    // from course i to course j. This is the formal proof of induction/transitivity.
    int result = relModule->getTransitiveClosureValue(i_prereq, j_dependent);

    if (result == 1)
    {
        cout << "Base Case P(1) or Inductive Step P(k) Verified:\n";
        cout << "✅ Proof Complete: A valid chain exists. " << prereqCode << " MUST be taken before " << dependentCode << ".\n";
        return true;
    }
    else
    {
        cout << "❌ Proof Failed: No prerequisite chain found.\n";
        return false;
    }
}