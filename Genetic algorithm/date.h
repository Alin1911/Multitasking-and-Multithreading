#ifndef DATE_H
#define DATE_H

#include "sack_object.h"
#include "individual.h"

typedef struct date {
	int thread_id;
	int N;
	int P;
	individual *current_generation;
} date;

#endif
