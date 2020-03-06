
#ifndef __FILEWATCHER_H
#define __FILEWATCHER_H
#include <string>
#include <vector>
#include <functional>
#include <sys/event.h>

class FileWatcher
{
public:
    FileWatcher(std::string directory, std::string subFilter = "*", int kq = kqueue());
    ~FileWatcher();

    void registerOnDeleteCb(std::function<void(FileWatcher *)> cb);

    /**
     * @brief   blocking wait untill the file changes
     */
    void watchForEvent(size_t timeout);

private:
    typedef struct kevent TKevent;
    std::string _directory;
    std::string _fileFilter;
    int _folderHandler;
    int _kq;
    TKevent _dirEvent;

    std::function<void(FileWatcher *)> onDeleteCb;

    std::vector<std::shared_ptr<FileWatcher>> _subFiles;

    void registerSubdirectoris();
    void onEvent(TKevent *event);
};

#endif