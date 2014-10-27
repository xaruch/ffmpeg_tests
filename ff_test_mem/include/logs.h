/*
 * logs.h
 *
 *  Created on: Aug 6, 2013
 *      Author: romar
 */

#ifndef LOGS_H_
#define LOGS_H_

#include <string>

using namespace std;

#ifdef __arm__
  #include "../../arlogging/arlogging.h"
#else
  #include "../../arlogging-i386/arlogging-i386.h"
#endif

#include <string>

#ifdef __arm__
  using namespace arlogging;
#else
  using namespace arlogging_i386;
#endif

string AppendLog(string, const char *);

#endif /* LOGS_H_ */
