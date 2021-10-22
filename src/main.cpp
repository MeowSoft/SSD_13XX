#include <Arduino.h>
#include "Tests/Menu.h"
#include "Tests/Tests.h"
#include "_includes/_common_16bit_colors.h"
#include "Tests/Utils.h"

// Tests and Menu objects.
Tests _tests = Tests();
Menu _menu = Menu();

// Vars to handle loop state.
Menu::Selections_t _state = Menu::Selections_t::NO_SELECTION;
uint8_t _subState = 0;
bool _waiting = false;

// Type for test method lambda.
typedef uint32_t (*testLambda_t)(uint8_t&);

// Method defs:
void fillScreenTest();
void clearScreen();
void diagonalLinesTest(uint8_t& subState);
void straightLinesTest(uint8_t& subState);
void filledRectanglesTest();

void allTests();

void subStateMachine(
    uint8_t& subState, 
    uint8_t numberOfSubStates, 
    const char* testName, 
    testLambda_t test
);

void setup() {
  
    // Init serial.
    Serial.begin(9600);
    while (!Serial) {;}

    #ifdef USE_MINE
    Serial.println("Using my SSD lib:");
    #else
    Serial.println("Using Sumotoy SSD lib:");
    #endif

    // Init menu and tests.
    _menu.setup();
    _tests.setup();
}

void loop() {

    // If no selection state...
    if (_state == Menu::Selections_t::NO_SELECTION) {

        // Get input.
        _state = _menu.loop();

        // Reset substate, result value, and waiting flag.
        _subState = 0;
        _waiting = false;
    }

    // If waiting flag is set...
    if (_waiting) {

        // Get input.
        Menu::Selections_t check = _menu.loop();

        // If not 'next step', then bail.
        if (check != Menu::Selections_t::NEXT_TEST_STEP) return;

        // Otherwise, clear the flag and go on.
        _waiting = false;
    }

    // Handle current state.
    switch (_state) {

        case Menu::Selections_t::CLEAR_SCREEN:
            clearScreen();
            break;

        case Menu::Selections_t::FILL_SCREEN_TEST:
            fillScreenTest();
            break;

        case Menu::Selections_t::DIAGONAL_LINES_TEST:
            diagonalLinesTest(_subState);
            break;

        case Menu::Selections_t::STRAIGHT_LINES_TEST:
            straightLinesTest(_subState);
            break;

        case Menu::Selections_t::FILLED_RECTANGLES_TEST:
            filledRectanglesTest();
            break;

        case Menu::Selections_t::ALL_TESTS:
            allTests();
            break;

        default:

            // Reset state if no valid selection.
            _state = Menu::Selections_t::NO_SELECTION;
            break;
    }
}

void clearScreen() {
    Serial.print("    Clear screen.");
    _tests.clearScreen();
    _state = Menu::Selections_t::NO_SELECTION;
}

void fillScreenTest() {
    Serial.print("    Fill screen: ");
    uint8_t result = _tests.testFillScreen(NAVY);
    SerialPrintf("    %d uSec\r\n", result);
    _state = Menu::Selections_t::NO_SELECTION;
}

void diagonalLinesTest(uint8_t& subState) {

    // The test to run.
    testLambda_t test = [](uint8_t& s) { 
        return _tests.testDiagonalLines(CYAN, s); 
    };

    // Run test.
    subStateMachine(subState, 4, "    Diagonal lines ", test);
}

void straightLinesTest(uint8_t& subState) {

    // The test to run.
    testLambda_t test = [](uint8_t& s) { 
        return _tests.testStraightLines(RED, s); 
    };

    // Run test.
    subStateMachine(subState, 2, "    Straight lines ", test);
}

void filledRectanglesTest() {
    Serial.print("    Filled rectangles: ");
    uint8_t result = _tests.testFilledRectangles(YELLOW, MAGENTA);
    SerialPrintf("    %d uSec\r\n", result);
    _state = Menu::Selections_t::NO_SELECTION;
}



void allTests() {

    // Var for substate tests.
    uint8_t subState;

    // Fill screen.
    fillScreenTest();

    // Diagonal lines.
    subState = 0;
    diagonalLinesTest(subState);
    diagonalLinesTest(subState);
    diagonalLinesTest(subState);
    diagonalLinesTest(subState);
    _waiting = false;

    // Straight lines.
    subState = 0;
    straightLinesTest(subState);
    straightLinesTest(subState);
    _waiting = false;
    
    // Filled rectangles
    filledRectanglesTest();

    // Reset state.
    _state = Menu::Selections_t::NO_SELECTION;
}

/**
 * @brief Run the given test with the given substate.
 * 
 * @param subState 
 * @param numberOfSubStates 
 * @param testName 
 * @param test 
 */
void subStateMachine(
    uint8_t& subState, 
    uint8_t numberOfSubStates, 
    const char* testName, 
    testLambda_t test
) {

    // Test results.
    static uint8_t result;

    // If first substate...
    if (subState == 0) {
        
        // Init results and print test name.
        result = 0;
        Serial.print(testName);
    }

    // Print substate.
    SerialPrintf("%d ", subState);

    // Run test.
    result += test(subState);

    // Inc substate.
    subState++;

    // If we've run all substates...
    if (subState >= numberOfSubStates) {

        // Print result. 
        SerialPrintf("    %d uSec\r\n", result);

        // Reset state and waiting flag.
        _state = Menu::Selections_t::NO_SELECTION;
        _waiting = false;
        return;
    }
        
    // Otherwise, set waiting flag.
    _waiting = true; 
}