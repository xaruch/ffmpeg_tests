#include "include/logs.h"
#include "include/vars.h"

using namespace arlogging;

/*
string AppendLog(string txt, const char *module)
{
	return "";
}
*/

string AppendLog(string txt, const char *module)
{
  size_t level = 0;
  string print_str = string("<") + string(module) + string("> ") + txt;


  if (txt.find("EMERGENCY") != string::npos)
    level = 0;
  else if (txt.find("ALERT") != string::npos)
    level = 1;
  else if (txt.find("CRITICAL") != string::npos)
    level = 2;
  else if (txt.find("ERROR") != string::npos)
    level = 3;
  else if (txt.find("WARNING") != string::npos)
    level = 4;
  else if (txt.find("NOTICE") != string::npos)
    level = 5;
  else if (txt.find("INFO") != string::npos)
    level = 6;
  else if (txt.find("DEBUG") != string::npos)
    level = 7;
  else
    level = 7;

  if (Log == NULL)
    {
      printf("ERROR! Log pointer is NULL! Skip.\n");
      return "";
    }

  Log->AppendLog(print_str, level);
  if (level <= PRINT_LEVEL)
    {
      std::printf("%s", print_str.c_str());
    }
  return txt;
}

