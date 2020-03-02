
#ifndef __FILEWATCHER_H
#define __FILEWATCHER_H
#include <string>
#include <vector>
#include <functional>

class FileWatcher
{
public:
    FileWatcher(std::string directory, std::string subFilter);
    ~FileWatcher();

    void registerOnDeleteCb(std::function<void> cb);

private:
    std::string _directory;
    std::string _subFilter;

    std::function<void(FileWatcher *)> onDeleteCb;

    std::vector<FileWatcher> _subFiles;
};

#endif