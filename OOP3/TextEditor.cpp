#include "TextEditor.h"

#include <fstream>
#include <iostream>
#include <sstream>


TextEditor::TextEditor(string fn)
{
    x = 0; y = 0; mode = 'n';
    cmd = "";
    lowerbound = 0;
    upstatus = true;
    status = "Normal Mode";
    textBuffer = new TextBuffer();
    saved = false;
    changed = false;
    clipboard = "";
    view = new View();

    if (openFile(fn) == false)
        openFile(WELCOME_FILE);
}

TextEditor::TextEditor()
{
    x = 0; y = 0; mode = 'n';
    cmd = "";
    lowerbound = 0;
    upstatus = true;
    status = "Normal Mode";
    textBuffer = new TextBuffer();
    saved = false;
    changed = false;
    clipboard = "";
    view = new View();

    openFile(WELCOME_FILE);
}

bool TextEditor::openFile(string fileName_)
{
    filename = fileName_;

    if (textBuffer->lines.size() > 0)
        textBuffer->lines.clear();

    ifstream infile(fileName_.c_str());
    if (infile.is_open())
    {
        while (!infile.eof())
        {
            string temp;
            getline(infile, temp);
            textBuffer->appendLine(temp);
        }
        infile.close();
        return true;
    }
    else
    {
        cerr << "Cannot open file: '" << fileName_ << endl;
        textBuffer->appendLine("");
        infile.close();
        return false;
    }
}

void TextEditor::openNewFile(string fileName)
{
    openFile(fileName);
    updateScreen();
}

void TextEditor::updateStatus()
{
    switch(mode)
    {
        case 'n':
            if(cmd.empty())
                status = string(CED_TITLE) + " " + string(CED_VERSION);
            else
                status = cmd;
            break;
        case 'i':
            status = "Insert Mode";
            break;
        case 'q':
            status = "Exiting";
            break;
    }
    status += "\tCOL: " + tos(x) + "\tROW: " + tos(y)/*tos(lowerbound)*/;
}

void TextEditor::updateScreen()
{
    view->updateScreen(textBuffer->lines, lowerbound, x, y, status);
}

void TextEditor::printScreen()
{
    view->printStatusLine(status);
    view->printBuffer(textBuffer->lines, lowerbound, x, y);
}

void TextEditor::setCursor(int xCoordinate, int yCoordinate)
{
    x = xCoordinate;
    y = yCoordinate;
    move(y, x);
}

int TextEditor::getSymbol()
{
    return view->getSymbol();
}

void TextEditor::handleInput(int c)
{
    upstatus = true;
    switch(mode)
    {
        case 'n':
            switch(c)
            {
                case KEY_LEFT:
                    moveLeft();
                    break;
                case KEY_RIGHT:
                    moveRight();
                    break;
                case KEY_UP:
                    moveUp();
                    break;
                case KEY_DOWN:
                    moveDown();
                    break;
                case KEY_ENTER:
                case KEY_CODE_ENTER:
                    execCmd();
                    break;
                case KEY_CODE_ESC:
                    cmd.clear();
                    break;
                case KEY_CODE_BACKSPACE:
                {
                    if (!cmd.empty())
                        cmd.erase(cmd.length() - 1, 1);
                    break;
                }
                default:
                    cmd += string(1, char(c));
                    break;
            }
            break;
        case 'i':
            switch(c)
            {
                case KEY_CODE_ESC:
                {
                    // Escape/Alt key
                    mode = 'n';
                    break;
                }
                case KEY_CODE_BACKSPACE:
                {
                    if (x == 0 && y > 0)
                    {
                        x = textBuffer->lines[y - 1].length();
                        // Bring line down
                        textBuffer->lines[y - 1] += textBuffer->lines[y];
                        // Delete the line
                        deleteLine();
                        moveUp();
                        updateScreen();
                    }
                    else if (x > 0)
                        textBuffer->lines[y].erase(--x, 1);
                    break;
                }
                case _KEY_CODE_DELETE:
                case KEY_CODE_DELETE:
                {
                    // The Delete key
                    if (x == textBuffer->lines[y].length() && y != textBuffer->lines.size() - 1)
                    {
                        // Bring line down
                        textBuffer->lines[y] += textBuffer->lines[y + 1];
                        // Delete the line
                        deleteLine(y + 1);
                        updateScreen();
                    }
                    else
                        textBuffer->lines[y].erase(x, 1);
                    break;
                }
                case KEY_LEFT:
                    moveLeft();
                    break;
                case KEY_RIGHT:
                    moveRight();
                    break;
                case KEY_UP:
                    moveUp();
                    break;
                case KEY_DOWN:
                    moveDown();
                    break;
                case KEY_CODE_ENTER:
                {
                    // Bring rest of line down
                    if (x < textBuffer->lines[y + lowerbound].length() - 1)
                    {
                        // Put rest of line on new line
                        textBuffer->insertLine(textBuffer->lines[y + lowerbound].substr(x, textBuffer->lines[y + lowerbound].length() - x), y + 1);
                        // Remove that part of the line
                        textBuffer->lines[y + lowerbound].erase(x, textBuffer->lines[y + lowerbound].length() - x);
                    }
                    else
                        textBuffer->insertLine("", y + lowerbound + 1);
                    moveDown();
                    break;
                }
                case KEY_CODE_TAB:
                {
                    // The tab
                    textBuffer->lines[y + lowerbound].insert(x, 4, ' ');
                    x += 4;
                    break;
                }
                default:
                {
                    textBuffer->lines[y + lowerbound].insert(x, 1, char(c));
                    x++;
                    break;
                }
            }
            changed = true;
            saved = false;
            break;
        default:;
    }
}

void TextEditor::deleteLine()
{
    textBuffer->removeLine(y);
}

void TextEditor::deleteLine(int i)
{
    textBuffer->removeLine(i);
}

void TextEditor::nullLine(int i)
{
    textBuffer->lines[i] = "";
}

void TextEditor::moveStartString()
{
    x = 0;
    move(y, x);
}

void TextEditor::moveEndString()
{
    x = textBuffer->lines[y].length() - 1;
    move(y, x);
}

void TextEditor::moveStartFile()
{
    x = 0;
    y = 0;
    move(y, x);
}

void TextEditor::moveEndFile()
{
    x = textBuffer->lines[textBuffer->lines.size() - 1].length() - 1;
    y = textBuffer->lines.size() - 1;
    move(y, x);
}

void TextEditor::moveLeft()
{
    if(x-1 >= 0)
    {
        x--;
        move(y, x);
    }
}

void TextEditor::moveRight()
{
    if(x+1 < COLS && x+1 <= textBuffer->lines[y].length())
    {
        x++;
        move(y, x);
    }
}

void TextEditor::moveUp()
{
    if(y-1 >= 0)
    {
        y--;
    }
    else if(y-1 < 0 && lowerbound > 0)
    {
        lowerbound--;
    }
    if(x >= textBuffer->lines[y].length())
        x = textBuffer->lines[y].length();
    move(y, x);
}

void TextEditor::moveDown()
{
    if(y+1 < LINES-1 && y+1 < textBuffer->lines.size())
    {
        y++;
    }
    else if(lowerbound+y+1 < textBuffer->lines.size())
    {
        //raiseflag = true;
        lowerbound++;
    }
    if(x >= textBuffer->lines[y].length())
        x = textBuffer->lines[y].length();
    move(y, x);
}

void TextEditor::moveToLine(int num)
{
    y = num;
    move(y, x);
}

void TextEditor::moveToRightWord()
{
    if (x != textBuffer->lines[y].size() - 1)
    {
        if (textBuffer->lines[y][x + 1] == ' ')
        {
            moveRight();
            moveRight();
        }
        while (x != textBuffer->lines[y].size() - 1 && textBuffer->lines[y][x + 1] != ' ')
            moveRight();
    }
}

void TextEditor::moveToLeftWord()
{
    if (x != 0)
    {
        if (textBuffer->lines[y][x - 1] == ' ')
        {
            moveLeft();
            moveLeft();
        }
        while (x != 0 && textBuffer->lines[y][x - 1] != ' ')
            moveLeft();
    }
}

void TextEditor::saveFile()
{
    if(filename == "")
    {
        // Set filename to untitled
        filename = "untitled";
    }

    ofstream f(filename.c_str());
    if(f.is_open())
    {
        for(int i = 0; i < textBuffer->lines.size(); i++)
        {
            f << textBuffer->lines[i];
            if (i < textBuffer->lines.size() - 1)
                f << endl;
        }
        status = "Saved to file!";
    }
    else
    {
        status = "Error: Cannot open file for writing!";
    }
    f.close();
}

void TextEditor::helper()
{
    vector<string> buf;
    buf.push_back("          Text Editor");
    buf.push_back("-------------------------------");
    buf.push_back("i - Insert Mode");
    buf.push_back("w - Save file");
    buf.push_back("wq! - Save file and quit");
    buf.push_back("q! - Quit without saving");
    buf.push_back("y - Copy cur string");
    buf.push_back("p - Paste");
    buf.push_back("yw - Copy cur word");
    buf.push_back("/text - Find word in right way");
    buf.push_back("dd - Cut cur string");
    buf.push_back("-------------------------------");
    buf.push_back("    Press Enter to return");

    clear();
    refresh();
    view->printBuffer(buf, lowerbound, x, y);
    refresh();

    system("pause>0");
    updateScreen();
}

string TextEditor::tos(int n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

void TextEditor::deleteWord()
{
    moveToLeftWord();

    while (x < textBuffer->lines[y].length() - 1 && textBuffer->lines[y][x] != ' ')
        textBuffer->lines[y].erase(x, 1);
    textBuffer->lines[y].erase(x, 1);
    x--;
    move(y, x);
}

void TextEditor::copyWord()
{
    moveToLeftWord();

    clipboard = "";

    int counter = x;
    while (counter < textBuffer->lines[y].length() && textBuffer->lines[y][counter] != ' ')
    {
        clipboard.push_back(textBuffer->lines[y][counter]);
        counter++;
    }
    
}

void TextEditor::copyLine()
{
    clipboard = textBuffer->lines[y];
}

void TextEditor::cutLine()
{
    clipboard = textBuffer->lines[y];
    deleteLine();
    updateScreen();
}

void TextEditor::insertClipboard()
{
    textBuffer->lines[y].insert(x + 1, clipboard);
    updateScreen();
}

void TextEditor::deleteSymbol()
{
    if (x == textBuffer->lines[y].length() && y != textBuffer->lines.size() - 1)
    {
        textBuffer->lines[y] += textBuffer->lines[y + 1];
        deleteLine(y + 1);
        updateScreen();
    }
    else
        textBuffer->lines[y].erase(x, 1);
}

void TextEditor::replaceSymbol()
{
    deleteSymbol();
    insertClipboard();
}

void TextEditor::findTextInRight(string text)
{
    clipboard = text;

    //Search in text massive from x, y coordinates
    bool find = true;

    for (int yCounter = y; yCounter < textBuffer->lines.size(); yCounter++)
    {
        for (int xCounter = x; xCounter < textBuffer->lines[yCounter].size(); xCounter++)
        {
            //If first symbols are equal and we are not out of range
            if (textBuffer->lines[yCounter][xCounter] == text[0] 
                && textBuffer->lines[yCounter].size() - xCounter >= text.size())
            {
                //Compare words
                for (int counter = 1; counter < text.size(); counter++)
                {
                    //If words are different, break the cycle
                    if (textBuffer->lines[yCounter][xCounter + counter] != text[counter])
                    {
                        find = false;
                        break;
                    }
                }
                //If words are equal, end the function
                if (find)
                {
                    y = yCounter;
                    x = 0;
                    move(y, x);
                    return;
                }
                //Return "find" value and go to new iteration
                find = true;
            }
        }
    }
}

void TextEditor::findTextInLeft(string text)
{
    clipboard = text;

    //Search in text massive from x, y coordinates
    bool find = true;

    for (int yCounter = y; yCounter >= 0; yCounter--)
    {
        for (int xCounter = x; xCounter >= 0; xCounter--)
        {
            //If first symbols are equal and we are not out of range
            if (textBuffer->lines[yCounter][xCounter] == text[0]
                && textBuffer->lines[yCounter].size() - xCounter >= text.size())
            {
                //Compare words
                for (int counter = 1; counter < text.size(); counter++)
                {
                    //If words are different, break the cycle
                    if (textBuffer->lines[yCounter][xCounter + counter] != text[counter])
                    {
                        find = false;
                        break;
                    }
                }
                //If words are equal, end the function
                if (find)
                {
                    y = yCounter;
                    x = 0;
                    move(y, x);
                    return;
                }
                //Return "find" value and go to new iteration
                find = true;
            }
        }
    }
}

bool TextEditor::execCmd()
{
    if (cmd == "i")
        mode = 'i';
    else if (cmd == "q!")
        mode = 'q';
    else if (cmd == "q" && (saved || !changed))
        mode = 'q';
    else if (cmd == "^")
        moveStartString();
    else if (cmd == "$")
        moveEndString();
    else if (cmd == "gg" || int(cmd[0]) == KEY_CODE_PG_UP)
        moveStartFile();
    else if (cmd == "G" || int(cmd[0]) == KEY_CODE_PG_DOWN)
        moveEndFile();
    else if (cmd == "I")
    {
        moveStartString();
        mode = 'i';
    }
    else if (cmd == "A")
    {
        moveEndString();
        mode = 'i';
    }
    else if (cmd == "S")
    {
        nullLine(y);
        moveStartString();
        mode = 'i';
    }
    else if (cmd == "h")
        helper();
    else if (cmd == "e")//right word
        moveToRightWord();
    else if (cmd == "b")
        moveToLeftWord();
    else if (cmd == "diw")
        deleteWord();
    else if (cmd == "yw")
        copyWord();
    else if (cmd == "y")
        copyLine();
    else if (cmd == "dd")
        cutLine();
    else if (cmd == "p")
        insertClipboard();
    else if (cmd == "x")
        deleteSymbol();
    else if (cmd == "r")
        replaceSymbol();
    else if(cmd == "w")
    {
        upstatus = false;
        saveFile();
        saved = true;
        changed = false;
    }
    else if (cmd == "wq!")
    {
        upstatus = false;
        saveFile();
        saved = true;
        changed = false;
        mode = 'q';
    }
    else if (cmd[0] == '/' && cmd.size() > 2)
        findTextInRight(cmd.substr(1));
    else if (cmd[0] == '?' && cmd.size() > 2)
        findTextInLeft(cmd.substr(1));
    else if (cmd == "n")
        findTextInRight(clipboard);
    else if (cmd == "N")
        findTextInLeft(clipboard);
    else if (cmd[0] == 'w' && cmd.size() > 2)
    {
        upstatus = false;
        filename = cmd.substr(2);
        saveFile();
        saved = true;
        changed = false;
    }
    else if (cmd[0] == 'o' && cmd.size() > 2)
        openNewFile(cmd.substr(2));
    else if (cmd[cmd.size() - 1] == 'G')
    {
        cmd = cmd.substr(0, cmd.size() - 1);
        moveToLine(stoi(cmd));
    }

    cmd = "";
    return true;
}

TextBuffer* TextEditor::getTextBuffer()
{
    return this->textBuffer;
}

int TextEditor::getLowerBound()
{
    return this->lowerbound;
}

int TextEditor::getXCoordinate()
{
    return this->x;
}

int TextEditor::getYCoordinate()
{
    return this->y;
}

string TextEditor::getStatus()
{
    return this->status;
}

vector<string> TextEditor::getTextEditorData() 
{
    return textBuffer->lines;
}