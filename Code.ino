// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <RtcDS3231.h> //Includes the library for the RTC(Real Time Clock) 

// Replace with your network credentials
const char* ssid     = "";
const char* password = "";
int relayPin = 2; // The input to the relay pin

String webPage="";

RtcDS3231<TwoWire> rtcModule(Wire);
WiFiServer server(80); // Set port
String header; //Global variable for headers

void setup() {
  Serial.begin(115200);
  Wire.begin(D1, D2); //Reads the information from pin D1 and D2(The clock)
  
  //OBS!!! ONLY TO SET RESET THE TIME ON THE RTC
    RtcDateTime compiled = RtcDateTime(__DATE__,__TIME__);
    rtcModule.SetDateTime(compiled);
    
   
 
//The HTML code for the wirless start/stop mechanism; minified
  webPage +="<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"><title>CoffeMcYeet</title><script>function toggle() {";
  webPage += "if(document.getElementById(\"onoff\").innerHTML === \"Kaffebryggaren är igång!\"){";
  webPage +="document.getElementById(\"onoff\").innerHTML=\"Kaffebryggaren är inte igång!\";";
  webPage +="fetch(`${location.origin}/toggle`)";
  webPage +=".then(r => console.log(r))";
  webPage +=".catch(e => console.error(e));";
  webPage +="}else{";
  webPage +="document.getElementById(\"onoff\").innerHTML=\"Kaffebryggaren är igång!\";";
  webPage +="fetch(`${location.origin}/toggle`)";
  webPage +=".then(r => console.log(r))";
  webPage +=".catch(e => console.error(e));";
  webPage +="function startTime(){var e=new Date,t=e.getHours(),c=e.getMinutes(),n=e.getSeconds();c=checkTime(c),n=checkTime(n),document.getElementById(\'clock\').innerHTML=t +\":\"+c+\":\"+n;setTimeout(startTime,500)};function checkTime(e){return e<10&& (e=\"0\"+e),e};";
  webPage +="}}</script><style> html{background-color:#fff;}.button{display:block;margin:auto;margin-top:30%;text-decoration:none;border:none;background-color: #4CAF50;color:white;padding:30px 64px;text-align:center;font-size:16px;}h1{text-align:center;margin-top:10%;}</style></head><body onload=\"startTime()\"><h1 id=\"onoff\">Kaffebryggaren är inte igång!</h1><button class=\"button\" onclick=\"toggle()\">Toggle</button> <div id=\'clock\'></div></body></html>";
  
  pinMode(relayPin, OUTPUT); // Initialize pin as OUTPUT

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void makeCoffeeBitch() {
  digitalWrite(relayPin, HIGH);
  delay(75);
  digitalWrite(relayPin, LOW);
}

void loop(){
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /toggle") >= 0) {
              makeCoffeeBitch();
            }
            
            client.println(webPage);  //Creates a html index file that the client can accsess

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

    RtcDateTime now = rtcModule.GetDateTime(); //Takes the current time from the RTC to the variable now
    byte hours = now.Hour();  //Gives the time in hours
    byte minutes = now.Minute();  //Gives the time in minutes
    byte seconds = now.Second();  //Gives the time in seconds
    char timeString[16];  //The ammount of characters timeString can store
    sprintf(timeString,"Time: %02u:%02u:%02u", hours, minutes, seconds);  //Writes the hours, minutes and seconds to the variable timeSting
    //Serial.println(timeString); //Prints timeString

    //if statement for timer
    if(hours == 3 && minutes == 50 && seconds == 0){
        makeCoffeeBitch();
        
    }

  Serial.println(timeString); //Used as trubleshoot
 
  
}
