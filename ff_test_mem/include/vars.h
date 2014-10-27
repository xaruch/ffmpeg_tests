#ifndef VARS_H_
#define VARS_H_

#include "../../arlogging/arlogging.h"
using namespace arlogging;

#define REC_TIMEOUT				30

#define PRINT_LEVEL				7
#define LOG_LEVEL				7

#define CAM_URL				"rtsp://192.168.0.11:554/mpeg4/media.amp?audio=0"
//#define CAM_URL					"rtsp://10.7.1.237:554/mpeg4/media.amp?fps=250&audio=0"

#define SAVE_PATH				"/media/hdd/video/test"
#define BUF_SIZE				2048

#define EMERGENCY               0
#define ALERT                   1
#define CRITICAL                2
#define ERROR                   3
#define WARNING                 4
#define NOTICE                  5
#define INFO                    6
#define DEBUG                   7

extern char str[BUF_SIZE];
extern int cfdn;
extern Logging *Log;
extern size_t Count;

#endif /* VARS_H_ */
