#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>

// Initialize the LCD display with I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int numBulbs = 3; // Number of bulbs used in the game
const int sequenceLength = 100; // Maximum length of the sequence
const int displayPins[numBulbs] = {A1, A2, A3}; // Pins for the output bulbs
const int inputPins[numBulbs] = {12, 11, 10}; // Pins for the input bulbs
int sequence[sequenceLength]; // Array to store the generated sequence
int userSequence[sequenceLength]; // Array to store the user's input sequence
int currentLevel = 1; // Starting level of the game
int buzzerPin = 13; // Pin for the buzzer
int note = ((1 / 261.0) * 1000000) / 2; // Frequency for note 
bool gameExited = true; // Variable to track if the game was exited

const byte ROWS = 4; // Number of rows in the keypad
const byte COLS = 4; // Number of columns in the keypad

// Define the key map for the keypad
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Define the row and column pins for the keypad
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Initialize the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() 
{
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the backlight
    lcd.setCursor(0, 0); // Set the cursor to the top left corner
    randomSeed(analogRead(A0)); // Seed the random number generator

    // Set the pin modes for the display and input pins
    for (int i = 0; i < numBulbs; i++) 
    {
        pinMode(displayPins[i], OUTPUT);
        pinMode(inputPins[i], INPUT_PULLUP);
    }

    pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as output
    displayLCD("Memory Bulb GameMenu", true, 16); // Display the game title on the LCD
}

void loop() 
{
    char key = keypad.getKey(); // Get the key pressed on the keypad

    // Start a new game if 'A' is pressed
    if (key == 'A') 
    {
        gameExited = false; // Reset the game exit flag
        currentLevel = 1; // Reset the level to 1
        resetUserSequence(); // Clear user's sequence
        displayLCD("Memory Bulb GameStarting...", true, 16); // Display starting message
        delay(2000); // Wait for 2 seconds
        mainGame(); // Start the main game loop
    } 
    // Exit the game if 'C' is pressed
    if (key == 'C') 
        exitGameText();
}

void resetGameText() 
{
    resetUserSequence(); // Clear the user's input sequence
    currentLevel = 1; // Reset the level to 1
    gameExited = false; // Reset the game exit flag
    displayLCD("Memory Bulb GameResetting...", true, 16); // Display resetting message
    delay(2000); // Wait for 2 seconds
    mainGame(); // Start a new game
}

void exitGameText() 
{
    resetUserSequence(); // Clear the user's input sequence
    currentLevel = 1; // Reset the level to 1
    gameExited = true; // Set the game exit flag
    displayLCD("Memory Bulb GameExiting...", true, 16); // Display exiting message
    delay(2000); // Wait for 2 seconds
    displayLCD("Memory Bulb Game.", false, 0); // Display the game title again
}

void mainGame() 
{
    displayLCD("Level: " + String(currentLevel), false, 0); // Display the current level
	
  	delay(1250); // delay to allow user to see pattern
  
    generateSequence(); // Generate a new sequence for the current level

    // Show the sequence to the user, if interrupted, exit
    if (!showSequence()) return;

    showRoundDone(); // Indicate that the sequence display is done

    displayLCD("Your turn!", false, 0); // Prompt the user for input

    // Check the user's input
    if (getUserInput()) 
    {
        displayLCD("Correct!", false, 0); // Display correct message
        currentLevel++; // Increase the level
        delay(1000); // Wait for 1 second before proceeding
    } 
    else 
    {
        // Only display "Wrong!" if the game was not exited
        if (!gameExited) 
        {
            displayLCD("Wrong!", false, 0); // Display wrong message
            soundBuzzer(note); // Sound the buzzer
            currentLevel = 1; // Reset the level to 1
            waitForKeyPress(); // Wait for user to decite whether to exit or play again
            delay(1000); // Wait for 1 second before proceeding
        }
    }

    // Check if the game was not exited and proceed to the next round
    if (!gameExited) 
    {
        resetUserSequence(); // Clear the user's input sequence
        delay(1000); // Wait for 1 second before the next round
        mainGame();
    }
}

void waitForKeyPress() 
{
    char key;
    while (true) 
    {
        key = keypad.getKey(); // Get the key pressed on the keypad
        // Exit the game if 'C' is pressed
        if (key == 'C') 
        {
            exitGameText();
            break;
        } 
        // Reset the game if 'B' is pressed
        else if (key == 'B') 
        {
            resetGameText();
            break;
        }
    }
}

void generateSequence() 
{
    // Generate a random sequence for the current level
    for (int i = 0; i < currentLevel; i++) 
        sequence[i] = random(numBulbs);
}

bool showSequence() 
{
    // Display each bulb in the sequence to the user
    for (int i = 0; i < currentLevel; i++) 
    {
        // Exit the game if 'C' is pressed during the sequence display
        if (keypad.getKey() == 'C') 
        {
            exitGameText();
            return false;
        }
        int bulb = sequence[i]; // Get the current bulb in the sequence
        analogWrite(displayPins[bulb], 255); // Turn on the bulb
        delay(500); // Wait for half a second
        analogWrite(displayPins[bulb], 0); // Turn off the bulb
        delay(500); // Wait for half a second
    }
    return true;
}

void showRoundDone() 
{
    // Flash all bulbs twice to indicate the end of the sequence display
    for (int i = 0; i < 2; i++) 
    {
        for (int j = 0; j < numBulbs; j++) 
            analogWrite(displayPins[j], 255);
        
        delay(500);
        
        for (int j = 0; j < numBulbs; j++) 
            analogWrite(displayPins[j], 0);
        
        delay(500);
    }
}

bool getUserInput() 
{
    // Get the user's input sequence and check it against the generated sequence
    for (int i = 0; i < currentLevel; i++) 
    {
        bool validInput = false;
        while (!validInput) 
        {
            // Exit the game if 'C' is pressed during user input
            if (keypad.getKey() == 'C') 
            {
                exitGameText();
                return false;
            }
            // Check each input pin for a pressed button
            for (int j = 0; j < numBulbs; j++) 
            {
                if (digitalRead(inputPins[j]) == LOW) 
                {
                    userSequence[i] = j; // Store the user's input
                    validInput = true;
                    analogWrite(displayPins[userSequence[i]], 255); // Light up the bulb
                    delay(500);
                    analogWrite(displayPins[userSequence[i]], 0); // Turn off the bulb
                    delay(500);
                    break;
                }
            }
        }
        // If the user's input does not match the generated sequence, return false
        if (userSequence[i] != sequence[i])
            return false;
    }
    return true; // If the user's input matches the sequence, return true
}

void resetUserSequence() 
{
    // Clear the generated and user input sequences
    for (int i = 0; i < sequenceLength; i++) 
    {
        sequence[i] = -1;
        userSequence[i] = -1;
    }
}

void soundBuzzer(int duration) 
{
    // Sound the buzzer for the given duration
    digitalWrite(buzzerPin, HIGH);
    delay(duration);
    digitalWrite(buzzerPin, LOW);
}

void displayLCD(String message, bool split, int length) 
{
    lcd.clear(); // Clear the LCD
    lcd.setCursor(0, 0); // Set the cursor to the top left corner

    // If the message is too long, split it across two lines
    if (split)
        splitMessage(message, length);
    else
        lcd.print(message); // Print the message on the LCD
    delay(200); // Wait for 200 milliseconds
}

void splitMessage(String str, int numChar) 
{
    // Split the message into two lines on the LCD
    lcd.print(str.substring(0, numChar)); // Print the first part on the first line
    lcd.setCursor(0, 1); // Move the cursor to the second line
    lcd.print(str.substring(numChar)); // Print the remaining part on the second line
}