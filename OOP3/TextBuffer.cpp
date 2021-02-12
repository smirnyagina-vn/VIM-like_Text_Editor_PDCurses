#include "TextBuffer.h"

TextBuffer::TextBuffer()
{
}

string TextBuffer::remTabs(string line)
{
    int tab = line.find("\t");
    if(tab == line.npos)
        return line;
    else
        return remTabs(line.replace(tab, 1, "    "));
}

void TextBuffer::insertLine(string line, int x)
{
    line = remTabs(line);
    lines.insert(lines.begin()+x, line);
}

void TextBuffer::appendLine(string line)
{
    line = remTabs(line);
    lines.push_back(line);
}

void TextBuffer::removeLine(int i)
{
    lines.erase(lines.begin()+i);
}