#include <ESP8266WiFi.h>
#include <Wire.h>
/*
ESP8266 WIFI connecting and homepage
*/

// Replace with your network credentials
const char* ssid     = "/*Ditt nätverksnamn*/";
const char* password = "/*Ditt nätverkslösenord*/";

String webSite="";

WiFiServer server(80); // Set port
String header; //Global variable for headers

void setup() {
Serial.begin(115200);

//Homepage
  webSite +="<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"><title>CoffeMcYeet</title><script>function toggle() {";//Webpag
  webSite +="fetch(`${location.origin}/toggle`)";
  webSite +=".then(r => console.log(r))";
  webSite +=".catch(e => console.error(e));";
  webSite +="}</script><style>; body{width:100%;height:100%}html{background-color:#f0f;width:100%;height:100%;} button{display:block;margin:auto;margin-top:40%;}</style></head><body><button onclick=\"toggle()\">Toggle</button></body></html>";

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
  
  //Mimics the starting and stopping on a coffeemachine
  void makeCoffee() {
  digitalWrite(relayPin, HIGH);
  delay(75);
  digitalWrite(relayPin, LOW);
}
}

void loop() {
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
              makeCoffee();
            }
            
            client.println(webSite);

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
}
