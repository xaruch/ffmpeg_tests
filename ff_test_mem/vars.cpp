#include "../arlogging/arlogging.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __arm__
  using namespace arlogging;
#else
  using namespace arlogging_i386;
#endif

char str[2048];
int cfdn;
Logging *Log;
size_t Count = 0;
