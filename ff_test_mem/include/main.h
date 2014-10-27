#ifndef MAIN_H_
#define MAIN_H_

#include <glib.h>
#include <stdint.h>

GMainLoop *MainLoop;
static gboolean TimeoutCallback(gpointer);
int64_t Memory;

#endif /* MAIN_H_ */
