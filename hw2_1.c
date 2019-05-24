#include <stdio.h>
#include <windows.h>

unsigned int count = 0;
const int iterations = 1000000; // one billion
HANDLE tmutex;

DWORD WINAPI incr(void* arg) {
	int i;
	for (i = 0; i < iterations; i++) {
		WaitForSingleObject(tmutex, INFINITE);
		count++;
		ReleaseMutex(tmutex);
	}

	return 0;
}

int main() {
	// set so only one processor is used (doesn't seem to be working)
	//DWORD_PTR processAffinityMask = 1;
	//BOOL success = SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask);
	//printf("Affinity Success: %s\n", success ? "true" : "false");

	const int numThreads = 1;
	HANDLE threads[1];
	tmutex = CreateMutex(0, FALSE, 0);
	int i;
	int threadId;

	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	double elapsedTime;

	// get ticks per second
	QueryPerformanceFrequency(&frequency);

	// start timer
	QueryPerformanceCounter(&t1);

	// execute main, long running part of code
	for (i = 0; i < numThreads; i++)
		threads[i] = CreateThread(0, 0, incr, 0, 0, &threadId);

	WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);

	printf("Count: %d\n", count);

	// stop timer
	QueryPerformanceCounter(&t2);

	// compute and print the elapsed time in millisec
	elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
	printf("Elapsed time (ms): %f\n", elapsedTime);

	return 0;
}