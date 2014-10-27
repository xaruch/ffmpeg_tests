#ifndef ARLOGGING_H
#define ARLOGGING_H

#include <ftw.h>
#include <iostream>

using namespace std;

#define PATH_MAX 4096
#define DT_BUF_SIZE 254
#define LOG_STR_MAX 2024
#define LOG_DIR_NAME Logs

#ifdef __arm__
namespace arlogging
#else
namespace arlogging_i386
#endif
{
	#define EMERGENCY 0
	#define ALERT 1
	#define CRITICAL 2
	#define ERROR 3
	#define WARNING 4
	#define NOTICE 5
	#define INFO 6
	#define DEBUG 7

  class Logging
  {
	  private: char *LogPath;
	  private: int LLevel;
	  public: string txt;
	  private: bool ExistDirs;
	  public:  string LogName;
	  private: string FullLogName;

	  public:
	  Logging();
	  Logging(string);
	  Logging(string, int);
	  Logging(char *, string);
	  ~Logging();
	  void Init(string , int);
	  void AppendLog();
	  void AppendData(string);
	  string AppendLog(string, int);
	  void AppendLog(string);
	  char * GetCurrDateTimeFixedLength(char *, size_t);
	  static int CheckLogDirExist(const char *, const struct stat *, int , struct FTW *);
  };
}
#endif // ARLOGGING_H

