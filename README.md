# Keyboard Input ESP32 to LED Dot Matrix 8x32 MAX7219
## Introduction
This project aims to display keyboard input to an 8x32 LED dot matrix. This project has 2 modes, the first mode is to display information with regular looping, and ##the second mode displays input from the keyboard directly. To change the mode can be triggered by pressing a button.
## Demo Video
https://youtu.be/RXmIFWQub_s
## Component 
1. ESP32-S3
2. LED Dot Matrix 8x32 MAX7219
3. USB keyboard

## Schematic
![Schematic](https://github.com/zhafarullah/Keyboard2LED.Matrix/blob/main/Schematic.png)
## How the Code Works: Displaying Keyboard Input on an LED Dot Matrix
This code utilizes the MyEspUsbHost class to handle keyboard input via a USB Host. When a key is pressed, the code checks if the input is a valid printable ASCII character (between ' ' and '~'). Valid characters are added to a fullTextBuffer that stores the entire input history. If the text exceeds the LED matrix display capacity, it dynamically updates the visible buffer to show the most recent characters. The MD_Parola library is used to send the text to the LED dot matrix, ensuring smooth updates on the display. Special keys like backspace (\b) are handled to remove the last character from the buffer and update the display in real-time. Additionally, all input is mirrored in the Serial Monitor for debugging purposes.
