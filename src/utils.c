/*
 * utils.c
 *
 * Created on: 2015-04-01
 *     Author: Clemens Hammacher <hammacher@cs.uni-saarland.de>
 */

#include "utils.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void rand_init() {
  /* get three integers for seeding the RNG */
  unsigned long a = clock();
  unsigned long b = time(NULL);
  unsigned long c = getpid();

  /* now mix them
   * (this is Robert Jenkins' 96 bit Mix Function) */
  a=a-b;  a=a-c;  a=a^(c >> 13);
  b=b-c;  b=b-a;  b=b^(a << 8);
  c=c-a;  c=c-b;  c=c^(b >> 13);
  a=a-b;  a=a-c;  a=a^(c >> 12);
  b=b-c;  b=b-a;  b=b^(a << 16);
  c=c-a;  c=c-b;  c=c^(b >> 5);
  a=a-b;  a=a-c;  a=a^(c >> 3);
  b=b-c;  b=b-a;  b=b^(a << 10);
  c=c-a;  c=c-b;  c=c^(b >> 15);

  /* use this mix to initialize RNG */
  srand(c);
}

unsigned randu(unsigned max) {
  const unsigned int buckets = RAND_MAX / max;
  const unsigned int limit = buckets * max;

  unsigned int r;
  do {
    r = rand();
  } while (r >= limit);

  return r / buckets;
}
