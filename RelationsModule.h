#ifndef RELATIONS_MODULE_H
#define RELATIONS_MODULE_H

#include "Entities.h"

class RelationsModule
{
public:
    RelationsModule
    (
        Course* allCourses,
        int* courseCount
    );

    bool checkPartialOrderProperties();
    void buildPrerequisiteRelation();
    bool checkCyclicDependency();

    // Getter for the Transitive Closure (used by Induction Module)
    int getTransitiveClosureValue(int i, int j);

private:
    Course* courses;
    int* numCourses;

    int relationMatrix[MAX_RELATION_SIZE][MAX_RELATION_SIZE];
    int transitiveClosure[MAX_RELATION_SIZE][MAX_RELATION_SIZE];

    int getCourseIndex(const char* code);
    void computeTransitiveClosure();
    bool isAntiSymmetric();
    bool isIrreflexive();
};

#endif // RELATIONS_MODULE_H