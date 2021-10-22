#include <Arduino.h>
#include "Menu.h"

void Menu::_printHelpMenu() {
    Serial.println("Main menu:");
    Serial.println("    h - Print this menu");
    Serial.println("    c - clear screen");
    Serial.println("    space - next step in test");
    Serial.println("    f - run fill screen test");
    Serial.println("    d - Diagonal lines test");
    Serial.println("    s - Straight lines test");
    Serial.println("    fr - Filled rectangles test");
    Serial.println("    a - Run all tests");
    Serial.println("    l1 - Run lines test");
    Serial.println("");
}

void Menu::_resetInBuffer() {
    _inBufferPointer = _inBuffer;
    for (int i = 0; i <= _inBufferSize; i++) {
        _inBuffer[i] = 0;
    }
}

void Menu::setup() {
    _printHelpMenu();
    _resetInBuffer();
}

Menu::Selections_t Menu::loop() {

    // Init result value to 'no selection'
    Selections_t result = Selections_t::NO_SELECTION;

    // If no serial input, then return 'no selection'.
    if (Serial.available() <= 0) {
        return Selections_t::NO_SELECTION;
    }

    // Otherwise, read serial input.
    char inChar = Serial.read();

    // If last character is not a newline...
    if (inChar != '\r' && inChar != '\n') {

        // Echo.
        Serial.print(inChar);

        // Add the char to the buffer.
        *_inBufferPointer = inChar;
        _inBufferPointer++;

        // If there's still room in the buffer, then quit and wait for the next character.
        if (_inBufferPointer - _inBuffer < _inBufferSize) return result;
    }

    // Otherwise, print a newline.
    Serial.println("");

    // And handle the entered command.
    if (strcmp("h", _inBuffer) == 0) {
        _printHelpMenu();
    }

    else if (strcmp("c", _inBuffer) == 0) {
        result = Selections_t::CLEAR_SCREEN;
    }

    else if (strcmp(" ", _inBuffer) == 0) {
        result = Selections_t::NEXT_TEST_STEP;
    }
    
    else if (strcmp("f", _inBuffer) == 0) {
        result = Selections_t::FILL_SCREEN_TEST;
    }
    
    else if (strcmp("d", _inBuffer) == 0) {
        result = Selections_t::DIAGONAL_LINES_TEST;
    }

    else if (strcmp("s", _inBuffer) == 0) {
        result = Selections_t::STRAIGHT_LINES_TEST;
    }

    else if (strcmp("fr", _inBuffer) == 0) {
        result = Selections_t::FILLED_RECTANGLES_TEST;
    }

    else if (strcmp("a", _inBuffer) == 0) {
        result = Selections_t::ALL_TESTS;
    }

    // Reset the buffer and return the result.
    _resetInBuffer();
    return result;
}
