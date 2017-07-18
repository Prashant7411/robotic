#include <SoftwareSerial.h>  // include the software serial library 
#include<dht.h> // include the dht library
dht DHT; // initiate the DHT obeject
int a; // a variable to store the raw sensor value
String temp; // Variable to store the temperature
String humid; // Variable to store the humidity

SoftwareSerial esp(0,1); // set the software serial pins to 0,1
#define SSID "prashant" // Replace the x with your wifi network name
#define PASS "whitemoon" // Replace x with the network password


/*
 Function to send AT commands to ESP8266
*/
void sendAT(String cmd)  
{
  Serial.print("SEND:");
  esp.println(cmd);
  Serial.println(cmd);
}

/*
 Function to connect to the Wifi network
 */
boolean connectwifi() 
{
  Serial.println("AT+CWMODE=1"); // Set the esp8266 to station mode
  delay(2000);
  Serial.println("AT+CWLAP"); // AT command to look for available networks
  String cmd="AT+CWJAP=\""SSID"\",\""PASS"\""; // AT command to connect wit the wifi network
    
  sendAT(cmd); // call the function to send the AT command to ESP8266
  delay(5000); // Set delay to connect to the network
 
  if (Serial.find("Error"))
    {
      Serial.println("Received : Error coundn't connect to network");
      return false;
    }
  else
    {
      Serial.print("Wifi connected \n");
    }
 
  cmd="AT+CIPMUX=0";// Set the connection mode to single connection
  sendAT(cmd); // Send the ABove AT command to ESP8266
  if(Serial.find("Error"))
    {
      esp.print("Received Error");
      return false;
    }
}
/*
 Function to initialize arduino and esp communication
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// begin the serial communication with baud of 115200 
  esp.begin(9600); // begin the software serial communication with baud rate of 115200
  sendAT("AT"); // send AT to ESP8266 using the sendAT function
  delay(5000); // wait for esp to respond
  if (Serial.find("OK"))
    {
      Serial.print("Received: OK \n ESP ready to send data \n");
      connectwifi(); //call the connect wifi function to connect to network
    }
}

/*
 * Function to read data from DHT11 sensor
 */
void loop() {
  // put your main code here, to run repeatedly:
  a=DHT.read11(5); // read the sensor data and store it in variable a
  temp=DHT.temperature; // store the temperature values in temp variable
  humid=DHT.humidity;// store the humidity values in humid variable
  Serial.print("Temperature :"); // print the temperature on serial monitor
  Serial.println(temp); // Print the temperature value
  Serial.print("Humidity : ");//Print Humidity on serial monitor
  Serial.print(humid); // Print the humidity value on serial monitor
  Serial.print(" \n "); 
  updateTS(temp,humid); // call the function to update Thingspeak channel
  delay(3000);
}

/*
 * function to update thingspeak
 */
void updateTS(String T,String H){    
  String cmd="AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80"; //AT command to Start TCP connection with thingspeak
  sendAT(cmd); // Call the send AT command to send the above command to ESP8266
  delay(2000);
  if (Serial.find("Error"))
  {
    Serial.print("Received : Error \n EXIT1");
    return;
  }
  cmd="GET /update?key=P8LN0752YZMQBMOD&field1="+T+"&field2="+H+"\r\n"; // update the temprature and humidity values on thingspeak url,replace xxxxxxx with your write api key
  Serial.print("AT+CIPSEND="); //AT command to send the data through the TCP connection opened
  Serial.println(cmd.length());// find the length of the url
  if(Serial.find(">")) // check if the url length is greater than 2024 bytes
    {
    esp.print(">");
    esp.print(cmd);
    Serial.println(cmd);   
    }
  else
    {
    sendAT("AT+CIPCLOSE"); // Close the TCP connection
    }
  if (Serial.find("Error"))
    {
    Serial.print("Received: Error /n EXIT2");
    }
    Serial.println();
}


