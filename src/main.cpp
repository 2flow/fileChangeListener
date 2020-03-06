#include <iostream>
#include "FileWatcher.h"

#include <errno.h>     // for errno
#include <fcntl.h>     // for O_RDONLY
#include <stdio.h>     // for fprintf()
#include <stdlib.h>    // for EXIT_SUCCESS
#include <string.h>    // for strerror()
#include <sys/event.h> // for kqueue() etc.
#include <unistd.h>    // for close()
#include <dirent.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "usage: display_sync [branch name]" << std::endl;
        return 0;
    }
    size_t size = 200;
    char *path = new char[size];
    getcwd(path, size);
    std::cout << path << std::endl;
    std::string curDir(path);
    delete[] path;
    int pos = curDir.find_last_of("/\\");
    std::string repoDir = curDir.substr(0, pos) + "/temp";

    std::string filter = "";
    std::string initGitCommand = "./initGit.sh " + curDir + repoDir + " " + argv[1];
    std::string pushGit = "./pushGit.sh " + repoDir + " " + argv[1];

    system(initGitCommand.c_str());
    FileWatcher watcher{
        repoDir,
        filter};

    while (1)
    {
        watcher.watchForEvent(0);
        watcher.watchForEvent(2);
        system(pushGit.c_str());
    }
}
