/****************************************************************************
 * Copyright � 2003-2012 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#if !defined(integer_addition_h )
#define integer_addition_h 1

#include "mrnet/Types.h"
#include <unistd.h>

typedef enum { PROT_EXIT=FirstApplicationTag,
    PROT_CONCAT
} Protocol;

void initBuffer(int* sendAr);

#define TOTAL_PACKET_SIZE 20
#define TOTAL_STREAM_SIZE 100

//#define DEBUG_ON

#endif /* integer_addition_h */
