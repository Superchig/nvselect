#include <iostream>
#include <vector>
#include <thread>
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

    const auto wait_time = std::chrono::milliseconds(1);

    timeout(0);
    int selection = 0;
    int ch = '\0';
    int msCount = 5000; // Time to wait in milliseconds
    bool countTimeDown = true;
    while (true)
    {
        for (int i = 0; i < (int) choices.size(); i++)
        {
            if (i == selection) { attron(A_REVERSE); }
            mvprintw(i, 0, choices.at(i).c_str());
            if (i == selection) { attroff(A_REVERSE); }
        }

        if (countTimeDown)
        {
            const int secCount = (msCount / 1000) + 1;
            std::string remaining = "Seconds remaining: " + std::to_string(secCount);
            move(choices.size(), 0);
            clrtoeol();
            mvprintw(choices.size(), 0, remaining.c_str());
        }

        ch = getch();

        if (ch == ERR)
        {
            std::this_thread::sleep_for(wait_time);

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
        move(choices.size(), 0);
        clrtoeol();

        if (ch == '\n')
        {
            break;
        }
        else if ((ch == KEY_UP || ch == 'k') && selection > 0)
        {
            selection--;
        }
        else if ((ch == KEY_DOWN || ch == 'j') && selection < (int) choices.size() - 1)
        {
            selection++;
        }
    }

    endwin();

    std::string choice = choices.at(selection);
    if (choice == "NVIDIA")
    {
        execl("/usr/bin/nvidia-xrun", "/usr/bin/nvidia-xrun", nullptr);
    }
    else if (choice == "Intel")
    {
        execl("/home/chiggie/dotfiles/msi_gs65/startx-wrapper", "/home/chiggie/dotfiles/msi_gs65/startx-wrapper", nullptr);
    }

    return 0;
}
