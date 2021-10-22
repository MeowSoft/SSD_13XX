#ifndef MENU_H
#define MENU_H

class Menu {

    public:

        /**
         * @brief Menu selections.
         */
        typedef enum Selections {
            NO_SELECTION,
            CLEAR_SCREEN,
            NEXT_TEST_STEP,
            FILL_SCREEN_TEST,
            DIAGONAL_LINES_TEST,
            STRAIGHT_LINES_TEST,
            FILLED_RECTANGLES_TEST,

            ALL_TESTS
        } Selections_t;

        /**
         * @brief Set up the menu system.
         */
        void setup();

        /**
         * @brief Main menu loop.
         * 
         * @return MenuSelections_t The current menu selection.
         */
        Selections_t loop();

    private:
    
        static const int _inBufferSize = 10;
        char _inBuffer[_inBufferSize + 1];
        char* _inBufferPointer;

        void _printHelpMenu();
        void _resetInBuffer();
};

#endif
