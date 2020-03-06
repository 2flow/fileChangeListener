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
#include <array>

std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

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
    std::string repoDir = curDir + "/temp";

    std::string filter = "";
    std::string initGitCommand = "./initGit.sh " + curDir + " " + repoDir + " " + argv[1];
    std::string pushGit = "./pushGit.sh " + repoDir + " " + argv[1];
    std::string changesGit = "./changesGit.sh " + repoDir;

    system(initGitCommand.c_str());

    FileWatcher watcher{
        repoDir,
        filter};

    while (1)
    {
        watcher.watchForEvent(0);
        //sleep(1000);

        std::string out = exec(changesGit.c_str());
        // nothing to commit, working tree clean
        if (out.find("nothing to commit, working tree clean") == std::string::npos)
        {
            std::cout << "Push!!" << std::endl;
            system(pushGit.c_str());
        }
    }
}
