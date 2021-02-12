#include "TextEditor.h"
#include "Controller.h"
#include <curses.h>
#include <iostream>

using namespace std;


int main()
{

    TextEditor textEditor;
    textEditor = TextEditor(); 
    Controller controller(textEditor);
    controller.OnLoad();
  
    return 0;
}