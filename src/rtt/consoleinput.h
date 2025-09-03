#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#endif

class ConsoleInput
{
public:
    ConsoleInput()
    {
#ifndef _WIN32
        // Save old terminal settings
        tcgetattr(STDIN_FILENO, &old_tio);

        termios new_tio = old_tio;
        new_tio.c_lflag &= ~(ICANON | ECHO); // no canonical mode, no echo
        new_tio.c_cc[VMIN] = 1;
        new_tio.c_cc[VTIME] = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
#endif
    }

    ~ConsoleInput()
    {
#ifndef _WIN32
        // Restore terminal settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
#endif
    }

    char getChar()
    {
#ifdef _WIN32
        return _getch();
#else
        char c;
        if (read(STDIN_FILENO, &c, 1) < 0)
            return 0;
        return c;
#endif
    }

    bool isChar()
    {
#ifdef _WIN32
        return _kbhit();
#else
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv) > 0;
#endif
    }

private:
#ifndef _WIN32
    struct termios old_tio;
#endif
};