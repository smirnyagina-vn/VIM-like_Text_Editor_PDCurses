#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "TextBuffer.h"
#include "TextEditor.h"
#include "View.h"
#include <curses.h>

#define POWER_OFF 'q'

class Controller
{
private:
    
    TextEditor textEditor;
    TextBuffer* textBuffer;

public:

    void OnLoad();
    void cursesInit();
    Controller(TextEditor& textEditor);

};
#endif