
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int kq, fd, nev;
    struct kevent change;
    struct kevent event;
    
    kq = kqueue();

    std::string curDir(argv[0]);
    int  pos = curDir.find_last_of("/\\");
    curDir = curDir.substr(0, pos) + "/temp";

    // open folder to watch
    fd = open(curDir.c_str(), O_RDONLY);
    if (fd	== -1)
	    perror("Failed to open directory");

    EV_SET(&change, fd, EVFILT_VNODE,
          EV_ADD | EV_ENABLE | EV_ONESHOT,
          NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_ATTRIB,
           0, 0);
         
    std::cout << "Hello World";

    for (;;) {
        nev = kevent(kq, &change, 1, &event, 1, NULL);
        if (nev == -1) {
            // perror("kevent");
        }
        else if (nev > 0) {
            if (event.fflags & NOTE_DELETE) {
                std::cout << "update\n"; // File deleted
                break;
            }
            if (event.fflags & NOTE_EXTEND ||
                event.fflags & NOTE_WRITE)
                std::cout << "update\n"; // File modified
                
            if (event.fflags & NOTE_ATTRIB)
                std::cout << "update\n"; // File attributes modified
                
        }
    }
    
    close(kq);
    close(fd);
    return EXIT_SUCCESS;
}
