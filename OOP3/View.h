#ifndef VIEW_H
#define VIEW_H

#include <curses.h>
#include <vector>
#include <string>

using namespace std;

class View
{
private:

public:

    int getSymbol();
    void updateScreen(vector<string> lines, int lowerbound, int x, int y, string status);
    void printDecoration();
    void printStatusLine(string status);
    void printBuffer(vector<string> lines, int lowerbound, int x, int y);
};
#endif