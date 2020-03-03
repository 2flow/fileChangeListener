#include <iostream>
#include "FileWatcher.h"

#include <errno.h>     // for errno
#include <fcntl.h>     // for O_RDONLY
#include <stdio.h>     // for fprintf()
#include <stdlib.h>    // for EXIT_SUCCESS
#include <string.h>    // for strerror()
#include <sys/event.h> // for kqueue() etc.
#include <unistd.h>    // for close()

int main(int argc, char *argv[])
{
    std::string curDir(argv[0]);
    int pos = curDir.find_last_of("/\\");
    curDir = curDir.substr(0, pos) + "/temp";
    std::string filter = "";

    FileWatcher watcher{
        curDir,
        filter};

    while (1)
    {
        watcher.watchForEvent();
    }
}
