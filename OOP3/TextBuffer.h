#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include <curses.h>
#include <vector>
#include <string>
#include <vector>

using namespace std;

class TextBuffer
{
    private:
    public:
       
        vector<string> lines;

        TextBuffer();

        void insertLine(string, int);
        void appendLine(string);
        void removeLine(int);
        string remTabs(string);

        int getLinesSize()
        {
            return lines.size();
        }
};
#endif