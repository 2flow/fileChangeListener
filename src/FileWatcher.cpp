
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <iostream>
#include "FileWatcher.h"
using namespace std;

FileWatcher::FileWatcher(std::string directory, std::string fileFilter, int kq)
    : _directory{directory}, _fileFilter{fileFilter}, _kq{kq}
{
    // open folder to watch
    _folderHandler = open(_directory.c_str(), O_RDONLY);
    if (_folderHandler == -1)
        throw std::string("Unable to open directory: " + _directory);

    EV_SET(&_dirEvent, _folderHandler, EVFILT_VNODE,
           EV_ADD | EV_ENABLE | EV_ONESHOT | EV_CLEAR,
           NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_ATTRIB | NOTE_LINK | NOTE_RENAME | NOTE_REVOKE,
           0, this);

    kevent(_kq, &_dirEvent, 1, NULL, 0, NULL);
    cout << "setup: " << this->_directory << endl;
    registerSubdirectoris();
}

FileWatcher::~FileWatcher()
{
}

void FileWatcher::registerOnDeleteCb(std::function<void(FileWatcher *)> cb)
{
    onDeleteCb = cb;
}

void FileWatcher::watchForEvent(size_t timeout)
{
    timespec time;
    time.tv_sec = timeout;
    timespec *pTime = &time;
    if (timeout == 0)
        pTime = NULL;

    struct kevent change;
    if (kevent(_kq, NULL, 0, &change, 1, NULL) == -1)
    {
        printf("FAILL\n");
        exit(1);
    }

    FileWatcher *file = (FileWatcher *)(change.udata);
    if (file == NULL)
        throw std::string("unknowen event");
    file->onEvent(&change);
}

void FileWatcher::registerSubdirectoris()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(_directory.c_str())) != NULL)
    {
        // std::stringstream test(_fileFilter);
        // std::string segment;
        // std::vector<std::string> seglist;

        // while (std::getline(test, segment, '*'))
        // {
        //     seglist.push_back(segment);
        // }

        while ((ent = readdir(dir)) != NULL)
        {
            string dirname = ent->d_name;
            if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0) &&
                (dirname.find(".git") == string::npos))
            {
                this->_subFiles.push_back(make_shared<FileWatcher>(_directory + "/" + ent->d_name, "*", _kq));
            }
        }
    }
}

void FileWatcher::onEvent(TKevent *event)
{
    std::cout << "Dir: " << this->_directory << std::endl
              << "EV " << event->flags << std::endl;
    // Finally register the event again.
    kevent(_kq, &_dirEvent, 1, NULL, 0, NULL);
}