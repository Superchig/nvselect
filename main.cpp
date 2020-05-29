#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <curses.h>
#include "util.h"

int main()
{
    auto options = readConfig();

    // for (auto pair : options)
    // {
    //     std::cout << pair.first << ": " << pair.second << '\n';
    // }

    // return 0;

    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    curs_set(0);

    // std::vector<std::string> choice_displays{"NVIDIA", "Intel", "None (go to tty)"};
    std::vector<std::string> choice_displays;
    for (auto pair : options)
    {
        choice_displays.push_back(pair.first);
    }

    const auto waitTime = std::chrono::milliseconds(1);

    timeout(0);
    int selection = 0;
    int ch = '\0';
    int msCount = 5000; // Time to wait in milliseconds
    bool countTimeDown = true;
    while (true)
    {
        for (int i = 0; i < (int) choice_displays.size(); i++)
        {
            if (i == selection) { attron(A_REVERSE); }
            mvprintw(i, 0, choice_displays.at(i).c_str());
            if (i == selection) { attroff(A_REVERSE); }
        }

        if (countTimeDown)
        {
            const int secCount = (msCount / 1000) + 1;
            std::string remaining = "Seconds remaining: " + std::to_string(secCount);
            move(choice_displays.size(), 0);
            clrtoeol();
            mvprintw(choice_displays.size(), 0, remaining.c_str());
        }

        ch = getch();

        if (ch == ERR)
        {
            std::this_thread::sleep_for(waitTime);

            if (msCount > 0)
            {
                --msCount;
            }

            if  (countTimeDown && msCount <= 0)
            {
                break;
            }

            continue;
        }

        // If we've reached here, then at least one key has been pressed, so we
        // don't want to count time down any more
        countTimeDown = false;
        move(choice_displays.size(), 0);
        clrtoeol();

        if (ch == '\n')
        {
            break;
        }
        else if ((ch == KEY_UP || ch == 'k') && selection > 0)
        {
            selection--;
        }
        else if ((ch == KEY_DOWN || ch == 'j') && selection < (int) choice_displays.size() - 1)
        {
            selection++;
        }
    }

    endwin();

    std::string script = options.at(selection).second;
    if (!script.empty())
    {
        execl(script.c_str(), script.c_str(), nullptr);
    }

    return 0;
}
