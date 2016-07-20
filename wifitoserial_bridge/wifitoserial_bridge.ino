/*
 Ethernet -> Serial brige
 Nate Marx
 2016
 A simple ethernet to serial bridge designed to recieve single line   commands of 
 mall to medium length via HTTP GET requests formatted the following way:
	
	http://192.168.0.100/?=COMMANDGOESHEREZZZ

 The command is delimited in the front by ?= and end by ZZZ
 The query portion of the URL is parsed out andstandard Arduino serial port.
  Debug text can also be sent via serial,  but it should be prepended with "##"
  So the listening Arduino knows to ignore it.
*/

#include <SPI.h>
#include <EthernetV2_0.h>

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

EthernetServer server(80);
#define W5200_CS  10
#define SDCARD_CS 4


void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(9600);

    // We need to manually reset the W5200 chip on the ethernet shield before
    // it will be ready for us. To do this I have set up a 10k pulldown resistor
    // between RST on the sheild (NOT on the Arduino) and ground, then I use A1
    // to maintain a 3v signal on the pin at all times. When we reset we bring
    // that pin to ground to reset.
    Serial.println("##Resetting eth bridge...");
    pinMode(A1, OUTPUT);
    analogWrite(A1, 190);
    delay(100);
    analogWrite(A1, 0);
    delay(100);
    analogWrite(A1, 190);
    delay(100);

    //disable the sd card
    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);

    Serial.println("##Bringing up ethernet");
    int i = 0;
    while (Ethernet.begin(mac) == 0) {
        // If we fail to get an IP it's probably because the chip isn't fully coming up
        // full duplex and all that. Reset it and try again.
        Serial.println("##Failed to configure Ethernet using DHCP. Resetting chip...");
        analogWrite(A1, 0);
        delay(100);
        analogWrite(A1, 190);
        delay(100);
        ++i;
        // after 5 tries notify flit and stop blocking.
        if (i > 5)
        {
            Serial.println("ETH0");
            break;
        }

    }
     // let's get this show on the road
    Serial.println("Starting server...");
    server.begin();
    Serial.println("Server up.");
}

void loop()
{
    String request = "";
    EthernetClient client = server.available();
    if (client)
    {
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                request += c;

                // if request is complete, send back response
                if (c == '\n' && currentLineIsBlank)
                {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connnection: close");
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html><head>flit</head><body>look a webpage</body></html>");
                    break;
                }
                // watch for blank lines
                if (c == '\n')
                {
                    currentLineIsBlank = true;
                }
                else if (c != '\r')
                {
                    currentLineIsBlank = false;
                }
            }
        }
        delay(10);
        // close the connection:
        client.stop();

        //send our captured command out via serial
        if (request.indexOf("?=") >= 0)
        {
            //extract the command
            String command = request.substring(request.indexOf("?=")+2, request.indexOf("ZZZ", request.indexOf("?=")+3));
            //send it to flit
            Serial.println(command);
        }
    }
}


