#include <iostream>
#include <vector>
#include <unistd.h>
#include <curses.h>

int main()
{
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    curs_set(0);

    std::vector<std::string> choices{"NVIDIA", "Intel", "None (go to tty)"};
    int selection = 0;

    int ch = '\0';
    do
    {
        if (ch == KEY_UP && selection > 0)
        {
            selection--;
        }
        else if (ch == KEY_DOWN && selection < (int) choices.size() - 1)
        {
            selection++;
        }

        for (int i = 0; i < (int) choices.size(); i++)
        {
            if (i == selection) { attron(A_REVERSE); }
            mvprintw(i, 0, choices.at(i).c_str());
            if (i == selection) { attroff(A_REVERSE); }
        }
    } while ((ch = getch()) != '\n');

    endwin();

    std::string choice = choices.at(selection);
    if (choice == "NVIDIA")
    {
        execl("/usr/bin/nvidia-xrun", "/usr/bin/nvidia-xrun", nullptr);
    }
    else if (choice == "Intel")
    {
        execl("/usr/bin/startx", "/usr/bin/startx", nullptr);
    }

    return 0;
}
