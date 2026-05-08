# ESP-12F-LED-Controller
This project is to create a Wifi enabled LED controller that leverages thst ESP-12F microcontroller, and was originally concevied of as a learning project.

## Functionality:
 - Power on/off switch for the LED strip and the Controller
 - Toggle Swtich to switch between Static White LEDs and Colors/Effects
 - Potenteometer to control the brightness of the LED strips
 - Wifi enabled with endpoints that allow the Colors/Effect profile to be updated through the web browser

## Components:
 - ESP-12F D1 mini
 - 3.3v to 5v Level Shifter
 - 2 pin 2 Pos (on-off) Toggle Switch
 - 6 pin 2 Pos (on-on) Toggle Switch (Used as a 4 pin 2 Pos (on-ff))
 - 100k Ω Potentometer
 - 470 Ω Resistor
 - Solderable Prototype Board (Optional)

## Notes:
This project was initially coded in Python (since it was easy) and runs the intrepreted code on the device.

## TODO:
 - Add Instructions on how to deploy the code to the device

## Future Updates:
 - Rewrite in other low level compatiable langauges:
   -  C++
   -  Go
 -  Update endpoitns to return JSON and use a seperate service to manage the web interface.
 -  Deploy the Compiled Python code to the device
   -  GitHub Actions to compile and upload the binary for easy deployment
 
