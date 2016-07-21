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
#include <UIPEthernet.h>
//#include <EthernetDHCP.h>

uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server = EthernetServer(80);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("##Bringing up ethernet");
  UIPEthernet.begin(mac,IPAddress(192,168,0,100));
  //UIPEthernet.begin(mac);
  
  // let's get this show on the road
  Serial.println("##Starting server...");
  server.begin();
  Serial.print("##Server up: ");
  Serial.println(Ethernet.localIP());
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
          client.write("HTTP/1.1 200 OK", 16);
          client.write("Content-Type: text/html", 24);
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html><head><title>flit</title></head><body>look a webpage</body></html>");
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
      String command = request.substring(request.indexOf("?=") + 2, request.indexOf("ZZZ", request.indexOf("?=") + 3));
      //send it to flit
      Serial.println(command);
    }
  }
}


