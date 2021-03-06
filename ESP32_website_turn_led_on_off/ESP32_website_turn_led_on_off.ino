/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * LED attached to pin @ GPIO PIN_NUMBER

 created 25 Nov 2012 by Tom Igoe
 modified 6th July 2018 to run on ESP-WROOM-32 by Matthew Taylor
 confirmed working w GPIO pin 2 lighting external led and onboard led.

 https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
 
 */
#include <SPI.h>
#include <WiFi.h>

char ssid[] = "sleepyPluto";      //  your network SSID (name)
char pass[] = "rfv45891";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
int PIN_NUMBER = 2;
int PIN_NUMBER2 = 4;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);      // initialize serial communication
  pinMode(PIN_NUMBER, OUTPUT);      // set the LED pin mode
  pinMode(PIN_NUMBER2, OUTPUT);      // set the LED pin mode


  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait for connection:
    delay(5000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/L\">here</a> turn the LED on pin1 off<br>");
            client.print("Click <a href=\"/H\">here</a> turn the LED on pin1 on<br>");            
            client.print("Click <a href=\"/2L\">here</a> turn the LED on pin2 off<br>");
            client.print("Click <a href=\"/2H\">here</a> turn the LED on pin2 on<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(PIN_NUMBER, HIGH);               // GET /H turns the LED on
          Serial.println("GET /H, pin set to HIGH");
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(PIN_NUMBER, LOW);                // GET /L turns the LED off
          Serial.println("GET /L, pin set to LOW");
        }
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /2H")) {
          digitalWrite(PIN_NUMBER2, HIGH);               // GET /H turns the LED on
          Serial.println("GET /2H, pin2 set to HIGH");
        }
        if (currentLine.endsWith("GET /2L")) {
          digitalWrite(PIN_NUMBER2, LOW);                // GET /L turns the LED off
          Serial.println("GET /2L, pin2 set to LOW");
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
