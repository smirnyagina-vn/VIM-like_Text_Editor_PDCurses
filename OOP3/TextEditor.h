#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "View.h"
#include "TextBuffer.h"

#include <curses.h>

#define CED_TITLE "VIM text editor"
#define CED_VERSION "Author: Smirnyagina V."

#define WELCOME_FILE "test.txt"

#define KEY_CODE_TAB 9
#define KEY_CODE_ESC 27
#define KEY_CODE_ENTER 10
#define KEY_CODE_BACKSPACE 8
#define _KEY_CODE_DELETE 330
#define KEY_CODE_DELETE 46
#define KEY_CODE_START_STRING 48
#define KEY_CODE_PG_UP -61
#define KEY_CODE_PG_DOWN -55

class TextEditor
{
    private:
        int x, y;
        int lowerbound;
        char mode;
        bool changed;
        bool saved;
        //bool raiseflag;
        TextBuffer* textBuffer;
        string clipboard;
        View* view;
        string status;
        string cmd;
        string filename;

        string tos(int);

        bool execCmd();

        void moveUp();
        void moveDown();
        void moveLeft();
        void moveRight();
        void moveStartString();
        void moveEndString();
        void moveStartFile();
        void moveEndFile();
        void moveToLine(int num);
        void moveToRightWord();
        void moveToLeftWord();

        void deleteLine();
        void deleteLine(int i);
        void nullLine(int i);
        void copyLine();
        void cutLine();

        void deleteWord();
        void copyWord();

        void deleteSymbol();
        void replaceSymbol();

        void insertClipboard();

        void findTextInRight(string text);
        void findTextInLeft(string text);

        bool openFile(string fileName_);
        void openNewFile(string fileName);
        void saveFile();

        void helper();


    public:
        bool upstatus;

        TextEditor();
        TextEditor(string fn);

        char getMode() {return mode;}

        void handleInput(int);
        void updateStatus();
        void updateScreen();
        void printScreen();
        void setCursor(int xCoordinate, int yCoordinate);
        int getSymbol();

        TextBuffer* getTextBuffer();

        int getLowerBound();
        int getXCoordinate();
        int getYCoordinate();

        string getStatus();

        vector<string> getTextEditorData();

};
#endif