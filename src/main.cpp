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
    if (argc < 2)
    {
        std::cout << "usage: display_sync [branch name]" << std::endl;
        return 0;
    }
    std::string curDir(argv[0]);
    int pos = curDir.find_last_of("/\\");
    curDir = curDir.substr(0, pos) + "/temp";
    std::string filter = "";
    std::string initGitCommand = "./initGit.sh " + curDir + " " + argv[1];
    std::string pushGit = "./pushGit.sh " + curDir + " " + argv[1];

    system(initGitCommand.c_str());
    FileWatcher watcher{
        curDir,
        filter};

    while (1)
    {
        watcher.watchForEvent(0);
        watcher.watchForEvent(2);
        system(pushGit.c_str());
    }
}
