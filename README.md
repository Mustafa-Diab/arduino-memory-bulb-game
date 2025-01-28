# Arduino Memory Bulb Game  

![License](https://img.shields.io/badge/license-MIT-blue.svg)  
![Version](https://img.shields.io/badge/version-1.0.0-brightgreen.svg)  

## 📖 Overview  

The Arduino Memory Bulb Game is a fun, interactive game where players must replicate an increasingly complex pattern of illuminated bulbs.  
- Bulbs light up in a sequence that the player must memorize and repeat.  
- Each correct sequence increases the difficulty by adding more bulbs to the pattern.  
- The game continues until the player fails to replicate the pattern.  

This project uses Arduino and basic electronic components to create an engaging memory game.  

## 🛠️ Features  

- Randomized patterns for endless replayability.  
- Adjustable difficulty by changing the number of bulbs or speed of the sequence.  
- Feedback for correct and incorrect inputs (e.g., sounds or LED indicators).  

## 🛠️ Components Required  

- Arduino board (e.g., Arduino Uno or similar).  
- LED bulbs (4 or more, depending on your game design).  
- Resistors (220 ohms, one for each LED).  
- Push buttons (one for each LED).  
- Breadboard and jumper wires.  

## 🚀 Getting Started  

### Prerequisites  

- Arduino IDE installed on your computer.  
- Basic knowledge of Arduino programming and circuits.  

### Circuit Diagram  

Set up the LEDs and buttons on a breadboard as shown:  
- Connect each LED to a digital pin on the Arduino via a resistor.  
- Connect each button to a digital pin and ground with a pull-down resistor.  

### Installation  

1. Clone the repository:  
   ```bash  
   git clone https://github.com/your-username/memory-bulb-game.git  
