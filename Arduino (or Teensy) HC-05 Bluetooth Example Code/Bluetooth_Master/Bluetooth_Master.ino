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
     Any Color LED                | D8                      |  Connects to the short leg of the LED (not required for master)
   -----------------------------------------------------------------------------------------------------------------------------
  COMMANDS:
    If "FLIP" is typed into the monitor, the state of the slave LED is changed
    If "BL#" is typed into the monitor, the slave LED blinks "#" number of times
    If "NUM" is typed into the monitor, the slave tells master how many times the LED has blinked
    If "TIME" is typed into the monitor, the slave tells master how long the LED has been on
*/
#include <SoftwareSerial.h>
//SoftwareSerial MyBlue(2, 3); // RX | TX for Arduino
SoftwareSerial MyBlue(9, 10); // RX | TX for Teensy
int ASCII[10]; // Any incoming transmissions (10 = max amount of characters here, can change)
int incomingByte = 0; // latest character of an incoming transmission (in ASCII)
String output = ""; String out = "";    // Initialize Output strings to zero
String command = "";                    // Initialize command string to zero

//Stream &BlueSerial = MyBlue;
void setup() 
{ 
 Serial.begin(9600); 
 MyBlue.begin(9600);  //Baud Rate for AT-command Mode.  
 Serial.println("***Master1 ON***"); 
} 

void loop() 
{ 
 // listen for slave bluetooth transmissions and parse return signal
 while(MyBlue.available() > 0)          // while the slave device sends information back to Master
 { 
   delay(50);                           // necessary in order to give buffer time to receive all bytes
  // read in that information 
   incomingByte = MyBlue.read();        // one byte at a time
   //Serial.println(incomingByte);
   //if(32 <= incomingByte <= 127)
   if(incomingByte != 10) // don't include endlines since they mess with the output sometimes (should only be nessecary for slave code though) --> remove if you intend to have multiple lines in a single transmission though 
    output += (char)incomingByte;        // concatenate until final version of transmission ("output") is fully decoded and rebuilt //Important: note that the (char) in front of the received int/byte converts it back from ASCII to regular text!
 }

 // Display slave return signal, if available
 if(output != "")
 {
    Serial.println(output); output = "";
 }

 // transmissions from termial to bluetooth, if available
 if (Serial.available())        // If you type a command into the Arduino Serial monitor
 {
    //types(Serial.read());
    int phi = Serial.read();
    Serial.println(phi);
    //MyBlue.write(Serial.read()); // Take in and send that command to the slave unit
 }
}

String GetCommand()
{
  
}
/*
void SendCommand(int bytes,Stream &BlueSerial,Stream &SerialPort,String ToSend)
{
  if(serialport2.available())
    BlueSerial.write(SerialPort.read());
  else
    BlueSerial.write(ToSend.c_str());
}
*/
void types(char t){Serial.println("It's a char!!!");}
void types(String t){Serial.println("It's a String!!!");}
void types(byte t){Serial.println("It's a byte!!!");}
void types(int t){Serial.println("It's an int!!!");}
