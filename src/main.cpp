#include <Arduino.h>
#include "Tests/Tests.h"
#include "_includes/_common_16bit_colors.h"
#include "Tests/Utils.h"
#include "SSD/SSD_Util.h"

#include "ConsoleMenu.h"

// Tests and Menu objects.
Tests _tests = Tests();
ConsoleMenu _menu = ConsoleMenu();

// Vars to handle loop state.
uint8_t _subState = 0;
bool _waiting = false;

// Vars to hold color values.
uint16_t color1 = CYAN;
uint16_t color2 = MAGENTA;

// Number of menu items.
#define NUM_MENU_ITEMS 11

// Menu item selection values.
typedef enum Selections {
    NO_SELECTION = -1,
    PRINT_MENU,
    CLEAR_SCREEN,
    SET_COLOR_1,
    SET_COLOR_2,
    FILL_SCREEN_TEST,
    DIAGONAL_LINES_TEST,
    STRAIGHT_LINES_TEST,
    RECTANGLES_TEST,
    FILLED_RECTANGLES_TEST,
    GRADIENTS_TEST,
    ALL_TESTS
} Selections_t;

static const MenuItem MenuItems[NUM_MENU_ITEMS] = {
    MenuItem("h", "Print this menu", PRINT_MENU),
    MenuItem("c", "Clear screen", CLEAR_SCREEN),
    MenuItem("c1", "Set color 1", SET_COLOR_1),
    MenuItem("c2", "Set color 2", SET_COLOR_2),
    MenuItem("f", "Fill screen test", FILL_SCREEN_TEST),
    MenuItem("d", "Diagonal lines test", DIAGONAL_LINES_TEST),
    MenuItem("s", "Straight lines test", STRAIGHT_LINES_TEST),
    MenuItem("r", "Rectangles test", RECTANGLES_TEST),
    MenuItem("fr", "Filled rectangles test", FILLED_RECTANGLES_TEST),
    MenuItem("g", "Gradients test", GRADIENTS_TEST),
    MenuItem("aa", "Run all tests in auto mode", ALL_TESTS)
};

// Last entered menu command.
Selections_t _lastCommand = NO_SELECTION;

// Type for test method lambda.
typedef uint32_t (*testLambda_t)(uint8_t&);

// Method defs:
void fillScreenTest();
void clearScreen();
void diagonalLinesTest(uint8_t& subState);
void straightLinesTest(uint8_t& subState);
void rectanglesTest();
void filledRectanglesTest();
void gradientsTest();
void handleCommand(int cmd);
void handleData(int cmd, char* data);

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

    // Indicate if using this SSD lib or Sumotoy.
    #ifdef USE_MINE
    Serial.println("Using my SSD lib:");
    #else
    Serial.println("Using Sumotoy SSD lib:");
    #endif

    Serial.println("");

    // Init menu and tests.
    _menu.setup(MenuItems, NUM_MENU_ITEMS);
    _tests.setup();

    // Print menu.
    _menu.printMenuItems();

    // Start reading menu command.
    _menu.readCommand();
}

void loop() {

    // If we're waiting for the next substate...
    if (_waiting) {

        if (!Serial.available()) return;

        // If ' ' is entered...
        if(Serial.read() == ' ') {
            
            // And run last command with next substate.
            handleCommand(_lastCommand);
        }

        return;
    }

    // If command or data is ready...
    if(_menu.loop()) {

        // If we get data...
        char buf[100];
        if (_menu.getData(buf)) {

            // Handle data.
            handleData(_lastCommand, buf);

            // Read next command.
            _menu.readCommand();
            return;
        }

        // If we get a command...
        int i = _menu.getCommand();
        if (i >= 0) {

            // Reset substate.
            _subState = 0;

            // Save command.
            _lastCommand = i;
                
            // Handle the command.    
            handleCommand(_lastCommand);

        // If no valid command...
        } else {

            // Then reset and read next command.
            _menu.readCommand();
        }
    }
}

void handleData(int cmd, char* data) {

    switch (cmd) {

        // Get data for colors.
        case SET_COLOR_1:
        case SET_COLOR_2:

            // Var to hold color data.
            uint16_t c;

            // Get r, b, and g strings.
            char* rStr = strtok(data, " ");
            char* bStr = strtok(NULL, " ");
            char* gStr = strtok(NULL, " ");

            // If no b string...
            if (bStr == NULL) {

                // Then get hex value.
                c = strtol(rStr, NULL, 16);
                
            // Otherwise...
            } else {

                // Get r, b, and g int values.
                long r = strtol(rStr, NULL, 10);
                long b = strtol(bStr, NULL, 10);
                long g = strtol(gStr, NULL, 10);

                // And convert to 565.
                c = SSD_Util::Color565(r, g, b);
            }

            // Print result and set color value.
            if (cmd == SET_COLOR_1) {
                Serial.printf("Color1 set: %d\r\n", c);
                color1 = c;
            } else {
                Serial.printf("Color2 set: %d\r\n", c);
                color2 = c;
            }
            
            return;
           
    default:
        break;
    }
}

void handleCommand(int cmd) {

    switch (cmd) {

        // Print menu.
        case PRINT_MENU:
            _menu.printMenuItems();
            Serial.println("");
            break;
    
        // Clear screen.
        case CLEAR_SCREEN:
            clearScreen();
            break;

        // Set color 1.
        case SET_COLOR_1:
            Serial.print("Enter color 1: ");
            _menu.readData();
            return;

        // Set color 2.
        case SET_COLOR_2:
            Serial.print("Enter color 2: ");
            _menu.readData();
            return;

        // Run fill screen test.
        case FILL_SCREEN_TEST:
            fillScreenTest();
            break;

        // Run diagonal lines test.
        case DIAGONAL_LINES_TEST:
            diagonalLinesTest(_subState);
            break;

        // Run straight lines test.
        case STRAIGHT_LINES_TEST:
            straightLinesTest(_subState);
            break;

        // Run rectangles test.
        case RECTANGLES_TEST:
            rectanglesTest();
            break;

        // Run filled rectangles tes.
        case FILLED_RECTANGLES_TEST:
            filledRectanglesTest();
            break;

        case GRADIENTS_TEST:
            gradientsTest();
            break;

        // Run all tests.
        case ALL_TESTS:
            allTests();
            break;

        default:
            break;
    }

    // Read next command.
    _menu.readCommand();
}

void clearScreen() {
    Serial.print("Clear screen.\r\n");
    _tests.clearScreen();
}

void fillScreenTest() {
    Serial.print("Fill screen : ");
    uint32_t result = _tests.testFillScreen(color1);
    SerialPrintf("%d uSec\r\n", result);
}

void diagonalLinesTest(uint8_t& subState) {

    // The test to run.
    testLambda_t test = [](uint8_t& s) { 
        return _tests.testDiagonalLines(color1, s); 
    };

    // Run test.
    subStateMachine(subState, 4, "Diagonal lines ", test);
}

void straightLinesTest(uint8_t& subState) {

    // The test to run.
    testLambda_t test = [](uint8_t& s) { 
        return _tests.testStraightLines(color1, s); 
    };

    // Run test.
    subStateMachine(subState, 2, "Straight lines ", test);
}

void rectanglesTest() {
    Serial.print("Rectangles : ");
    uint32_t result = _tests.testRectangles(color1);
    SerialPrintf("%d uSec\r\n", result);
}

void filledRectanglesTest() {
    Serial.print("Filled rectangles : ");
    uint32_t result = _tests.testFilledRectangles(color1, color2);
    SerialPrintf("%d uSec\r\n", result);
}

void gradientsTest() {
    Serial.print("Gradients : ");
    uint32_t result = _tests.testGradients(color1, color2);
    Serial.printf("%d uSec\r\n", result);
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
    
    // Rectangles.
    rectanglesTest();

    // Filled rectangles
    filledRectanglesTest();

    // Gradients
    gradientsTest();
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
    static uint32_t result;

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
        SerialPrintf(": %d uSec\r\n", result);

        // Reset state and waiting flag.
        _waiting = false;
        return;
    }
        
    // Otherwise, set waiting flag.
    _waiting = true; 
}