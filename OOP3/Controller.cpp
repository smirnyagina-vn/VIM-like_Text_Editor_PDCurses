#include "TextEditor.h"
#include "Controller.h"
#include "TextBuffer.h"
#include <curses.h>
#include <iostream>

using namespace std;

Controller::Controller(TextEditor& textEditor) {
    this->textEditor = textEditor;
    this->textBuffer = textEditor.getTextBuffer();
}

void Controller::cursesInit()
{
    initscr(); //Enable curses-mode
    noecho();  //Turning off the display of input characters
    cbreak();  // Disable line buffering
    keypad(stdscr, true);//maybe delete
}

void Controller::OnLoad() {
    
    cursesInit();

    while (textEditor.getMode() != POWER_OFF)
    {
        if (textEditor.upstatus)
            textEditor.updateStatus();
        textEditor.printScreen();
        int input = textEditor.getSymbol();
        textEditor.handleInput(input);
    }

    refresh();
    endwin(); //Exit drom curses-mode
}