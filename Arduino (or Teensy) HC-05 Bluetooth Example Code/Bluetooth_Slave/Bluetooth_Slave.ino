/*
  /-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\
  \        HC-05 Bluetooth With Arduino or Teensy 4.0/3.5 Example Code          /
  /          Written by Jacob Meyer (meye2497) Winter Break 2019-20             \                                                                            
  \-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/-\-/
   Arduino NANO/UNO/MICRO/Teensy 3.5/Teensy 4.0 pin connections:
   -----------------------------------------------------------------------------------------------------------------------------
     Component                    | Pins used               | Notes
     -                              -                         -
     SD card reader               |                         |  SD card reader built into teensy 3.5/4.0 boards
     HC-05 Bluetooth serial       | D2 D3                   |  HC-05 RX goes to D3, TX goes to D2 (for normal operation)
     Any Color LED                | D8                      |  Connects to the short leg of the LED
   -----------------------------------------------------------------------------------------------------------------------------
  COMMANDS:
    If "FLIP" is typed into the monitor, the state of the slave LED is changed
    If "BL#" is typed into the monitor, the slave LED blinks "#" number of times
    If "NUM" is typed into the monitor, the slave tells master how many times the LED has blinked
    If "TIME" is typed into the monitor, the slave tells master how long the LED has been on
*/
#include <SoftwareSerial.h> 
SoftwareSerial MyBlue(2, 3); // RX | TX 
int incoming = 0; 
String command = ""; 
int LED = 8; 
int LED_state = 0;                       // start with the LED off
int NUM = 0;                             // number of times that the LED has been turned on
double TIME = 0;                         // total amount of time that the LED has been on (seconds)
double t = 0;                            // Time between intervals
byte incomingByte = 0;                   // The latest Byte received from serial

void setup() 
{   
 Serial.begin(9600); 
 MyBlue.begin(9600); 
 pinMode(LED, OUTPUT); 
 MyBlue.write("Slave1 ON (Get that 'Star Wars' (Boba/Jango Fett) reference?)");  // lol i get it
} 

void loop() 
{ 
 command = GetCommand();              // obtain the command from the bluetooth device through software serial (if data is available)
 action(command);                     // make a choice based on the command that we received (or do nothing if no command is received)
 delay(100);                          //  necessary here, otherwise the incoming data stream will be too fast and large to read!
}





// Functions 
// Obtain the latest data from the master bluetooth module (or an empty string, if not available)
String GetCommand()
{
 command = "";                         // reset the command so we don't concatenate on the previous command(s)
 while(MyBlue.available() > 0)         // while the slave device sends information back to Master
 { 
   // read in that information 
   incomingByte = MyBlue.read();      // one byte at a time
   Serial.println(incomingByte);
   Serial.println((char)incomingByte);
   if(32 <= incomingByte <= 127){      // 32-127 is simply the range of ASCII characters we expect (a-z, A-Z, 0-9, some special characters)
      command += (char)incomingByte;   // concatenate until final version of transmission ("output") is fully decoded and rebuilt //Important: note that the (char) in front of the received int/byte converts it back from ASCII to regular text!
      if (incomingByte ==  10)         // If the incoming transmission incidentally ends in an annoying endline (if writing directly from the Serial Monitor, for example)
        command = command.substring(0,command.length()-1); // clip the endline
   }
 }
 return command;
}


// Make a choice based on the command received, if received (specific to this code only)
void action(String command){

 if (command == "")
  return;
 else if (command == "FLIP") // FLIP the LED switch to opposite state
 {
   if (LED_state == 0) // LED previously off, so turn it on now
   { 
     NUM += 1;
     digitalWrite(LED, HIGH); 
     t = millis()/1000; // Time between intervals
     delay(200);
     MyBlue.println("LED On"); // Send back to master
     delay(200);
     
     LED_state = 1; 
   } 
   else if (LED_state == 1) // LED previously on, so turn it off now
   { 
     TIME += millis()/1000 - t;  // total time LED has been ON
     digitalWrite(LED, LOW); 
     delay(200);
     MyBlue.println("LED Off"); 
     delay(200);
     LED_state = 0;
   }
 }
 else if ((command.substring(0,2)) == "BL") // Blink "n" number of times command
 {
     int n = (command.substring(2, command.length())).toInt(); //check to see how many times to blink
     for(int i = 1; i <= n; i++) // LED blinks "n" number of times, 1 second for each full cycle (0.5 seconds off, 0.5 seconds on per full cycle for n cycles)
     {
        digitalWrite(LED, LOW);
        delay(500);
        digitalWrite(LED, HIGH); NUM += 1;
        delay(500);
     }

     // Simply return to the original LED State recycling the "FLIP" code
     if (LED_state == 0) // LED previously off, so let's keep it off
     { 
       TIME += n*0.5; // Adjust total ON LED time as appropriate (on for an extra 0.5 seconds * n cycles since it was off before)
       digitalWrite(LED, LOW); 
       MyBlue.println("LED Back Off"); 
     } 
     else if (LED_state == 1) // LED previously on, so let's keep it on
     { 
       TIME -= n*0.5; // Adjust total ON LED time as appropriate (off for 0.5 seconds * n cycles less since it was on before)
       digitalWrite(LED, HIGH); 
       MyBlue.println("LED Back On"); 
     }
 }
 else if (command == "NUM") // Number of times the LED has been turned on
 {
      MyBlue.println(String(NUM).c_str());
 }
 else if (command == "TIME") // Amount of seconds the LED has been turned on
 {
      MyBlue.println(String(TIME).c_str());
 }
 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////Original Version of the code////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
