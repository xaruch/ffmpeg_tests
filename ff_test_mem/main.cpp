#include "include/main.h"
#include "include/ff_writer_engine.h"
#include "include/vars.h"

int main()
{
	FfWriterEngine *Writer = new FfWriterEngine();
	Log = new Logging("ff_test_mem.log", LOG_LEVEL);
	MainLoop = g_main_loop_new(NULL, FALSE);
	Writer->FirstStartRec();
	g_timeout_add_seconds_full(1000, 1, TimeoutCallback, (gpointer) Writer, NULL);
	Memory = Writer->GetMemoryConsumption();
	g_main_loop_run(MainLoop);

	delete Log;
	delete Writer;
}

gboolean TimeoutCallback(gpointer data)
{
	FfWriterEngine *PWriter = (FfWriterEngine *) data;

	PWriter->TimerCounter++;
	PWriter->AppendMemoryData();

	if (PWriter->TimerCounter >= REC_TIMEOUT)
	{
		uint64_t mem = PWriter->GetMemoryConsumption();
		sprintf(str, "DEBUG: Timeout callback. Memory: %lld. Delta: %lld\n", mem, mem - Memory);
		AppendLog(str, "MAIN");
		Memory = mem;
		PWriter->TimerCounter = 0;
		PWriter->Continue();
	}
	return true;
}
