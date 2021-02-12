#include "View.h"

int View::getSymbol()
{
    return getch();
}

void View::updateScreen(vector<string> lines, int lowerbound, int x, int y, string status)
{
    clear();
    refresh();
    printBuffer(lines, lowerbound, x, y);
    printStatusLine(status);
}

void View::printDecoration()
{
    //not implemented yet
}

void View::printStatusLine(string status)
{
    attron(A_REVERSE);
    mvprintw(LINES - 1, 0, status.c_str());
    clrtoeol();
    attroff(A_REVERSE);
}

void View::printBuffer(vector<string> lines, int lowerbound, int x, int y)
{
    int lineCounter = 0;
    for (int i = lowerbound; lineCounter < LINES - 1; i++)
    {
        if (i < lines.size())
            mvprintw(lineCounter, 0, lines[i].c_str());

        clrtoeol();
        lineCounter++;
    }
    move(y, x);
}