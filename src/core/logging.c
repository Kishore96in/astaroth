#include <stdio.h>
// Logging utils
    if (pid == 0) {
        time_t now       = time(NULL);
#else
    (void)pid;  // Unused
    (void)msg;  // Unused
    (void)args; // Unused
#else
    (void)pid;  // Unused
    (void)msg;  // Unused
    (void)args; // Unused
void
acDebugFromRootProc(const int pid, const char* msg, ...)
