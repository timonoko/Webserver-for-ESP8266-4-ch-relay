#include <ESP8266WiFi.h>

char* wifi_name = "Jorpakko"; // Your Wifi network name here
//char* wifi_pass = "***";    // Your Wifi network password here

const int JokuRele = 2;


WiFiServer server(80);    // Server will be at port 80

String header;


void rele(int rele, int paalla)
{  Serial.write(0xA0);
  Serial.write(rele);
  Serial.write(paalla);
  Serial.write(0xA0+rele+paalla);
  delay(200);}


void setup()
{
  pinMode(JokuRele, OUTPUT);
  digitalWrite(JokuRele, HIGH);

  Serial.begin (115200);

  WiFi.begin (wifi_name, wifi_pass);     // Connecting to the wifi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  server.begin();                           // Starting the server
} 

void loop() 
{
  WiFiClient client = server.available();     //Checking if any client request is available or not
  if (client)
  {
    boolean currentLineIsBlank = true;
    String buffer = "";  
    while (client.connected())
    {
      if (client.available())                    // if there is some client data available
      {
        char c = client.read(); 
        buffer+=c;                              // read a byte
        if (c == '\n' && currentLineIsBlank)    // check for newline character, 
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();    
          client.print("<HTML><title>ESP32</title>");
          client.print("<body><center><h1>ESP32 Relay Control </h1>");
          client.print("<p>Relay Control</p>");
          client.print("<a href=\"/?r1on\"\"> R1 <button>ON</button></a>");
          client.print("<a href=\"/?r1off\"\"> <button>OFF</button></a> <p>");
          client.print("<a href=\"/?r2on\"\"> R2 <button>ON</button></a>");
          client.print("<a href=\"/?r2off\"\"> <button>OFF</button></a> <p>");
          client.print("<a href=\"/?r3on\"\"> R3 <button>ON</button></a>");
          client.print("<a href=\"/?r3off\"\"> <button>OFF</button></a> <p>");
          client.print("<a href=\"/?r4on\"\"> R4 <button>ON</button></a>");
          client.print("<a href=\"/?r4off\"\"> <button>OFF</button></a> <p>");
          client.print("<a href=\"/?rKon\"\"> R* <button>ON</button></a>");
          client.print("<a href=\"/?rKoff\"\"> <button>OFF</button></a> <p>");
          client.print("</body></HTML>");
          break;        // break out of the while loop:
        }
        if (c == '\n') { 
          currentLineIsBlank = true;
          buffer="";       
        } 
        else 
          if (c == '\r') {     
	    if(buffer.indexOf("GET /?r1on")>=0) rele(1,1);
	    if(buffer.indexOf("GET /?r1off")>=0) rele(1,0);
	    if(buffer.indexOf("GET /?r2on")>=0) rele(2,1);
	    if(buffer.indexOf("GET /?r2off")>=0) rele(2,0);
	    if(buffer.indexOf("GET /?r3on")>=0) rele(3,1);
	    if(buffer.indexOf("GET /?r3off")>=0) rele(3,0);
	    if(buffer.indexOf("GET /?r4on")>=0) rele(4,1);
	    if(buffer.indexOf("GET /?r4off")>=0) rele(4,0);
	    if(buffer.indexOf("GET /?rKon")>=0) { rele(1,1); rele(2,1); rele(3,1); rele(4,1); }
	    if(buffer.indexOf("GET /?rKoff")>=0) { rele(1,0); rele(2,0); rele(3,0); rele(4,0); }
        }
        else {
          currentLineIsBlank = false;
        }  
      }
    }
    client.stop();
  }}
