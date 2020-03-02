#include "FileWatcher.h"

FileWatcher::FileWatcher(std::string directory, std::string subFilter)
    : _directory{directory}, _subFilter{subFilter}
{
}