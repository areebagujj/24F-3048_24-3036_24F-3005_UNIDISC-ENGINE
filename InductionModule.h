#ifndef INDUCTION_MODULE_H
#define INDUCTION_MODULE_H

#include "Entities.h"
#include "RelationsModule.h"

class InductionModule
{
public:
    InductionModule
    (
        Course* allCourses,
        int* courseCount,
        RelationsModule* relMod
    );

    // Uses Transitive Closure for fast strong induction verification
    bool verifyPrereqChainByInduction
    (
        const char* prereqCode, // Course that MUST be completed (starting)
        const char* dependentCode // Course that the student wants to take (ending)
    );

private:
    Course* courses;
    int* numCourses;
    RelationsModule* relModule;

    int getCourseIndex(const char* code);
};

#endif // INDUCTION_MODULE_H