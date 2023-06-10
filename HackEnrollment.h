
#ifndef EX1_HACKERENROLLMENT_H
#define EX1_HACKERENROLLMENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IsraeliQueue.h"

#define FRIEND_PAR 20
#define RIVAL_PAR 0

#define FRIEND 20
#define RIVAL -20
#define NEUTRAL 0


typedef struct EnrollmentSystem_t* EnrollmentSystem;
//typedef struct EnrollmentSystem_t ET;

/** Applies a case awareness filter to EnrollmentSystem, for string comparison functions.
@param EnrollmentSystem sys holds the system of queues
@param bool ignoreCase helps to distinguish between UPPERCASE or LOWERCASE
*/
void considerCase(EnrollmentSystem sys, bool ignoreCase, FILE* courses);

//add documantation
void destroyEnrollment(EnrollmentSystem sys);

/**creates a new EnrollmetSystem
@param students a file that holds the students and there information
@param courses a file that holds the courses and there information
@param hackers a file that holds the hackers and there information
* if there is an error return NULL*/
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

/**Creates an IsraeliQueue in each course, according to the order described in FILE*
@param EnrollmentSystem sys holds the system of queues
@param FILE* queues hold the queues
if there is an error return NULL*/
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

/**Advances hackers as much as possible according to pdf.
@param EnrollmentSystem sys holds the system of queues
@param FILE* out the output of the queues in sys
*/
void hackEnrollment(EnrollmentSystem sys, FILE* out);

#endif // PROVIDED EX1_HACKERENROLLMENT_H