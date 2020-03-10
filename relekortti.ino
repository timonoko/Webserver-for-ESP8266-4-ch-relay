#include <ESP8266WiFi.h>

#include "password.h" /*
char* wifi_name = "****"; // Your Wifi network name here
char* wifi_pass = "****";    // Your Wifi network password here
*/

const int Buttoni = 2;

WiFiServer server(80);    // Server will be at port 80

int releet[5] = {0,0,0,0,0};

void rele2(int rele, int paalla)
{  Serial.write(0xA0);
  Serial.write(rele);
  Serial.write(paalla);
  Serial.write(0xA0+rele+paalla);
  delay(50);}

void rele(int rele, int paalla)
{ releet[rele]=paalla;
  rele2(rele,paalla); }

void rpaivitys()
{  for (int r=1;r<5;++r) {
    rele2(r,releet[r]); }
}

void setup()
{
  pinMode(Buttoni, INPUT);
  Serial.begin (115200);
  WiFi.begin (wifi_name, wifi_pass);     // Connecting to the wifi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);  }
  server.begin();                           // Starting the server
  WiFi.softAPdisconnect(true);
} 

const int DOWN = 1;
const int UP = 2;
const int DONTKNOW = 0;
  
int suunta = DONTKNOW ;

void loop() 
{
  /*  if (digitalRead(Buttoni)==LOW) {
    if (suunta==UP)   { rele(1,0); rele(2,1); delay(100); rele(1,0); rele(2,0); suunta=DONTKNOW; }
    if (suunta==DOWN) { rele(1,1); rele(2,0); delay(100); rele(1,0); rele(2,0); suunta=DONTKNOW; }
    if (suunta==DONTKNOW) { rele(1,0); rele(2,0); delay(1000); } } */
  if (digitalRead(Buttoni)==LOW) {
    int loc=0; int hic=0; int nummer=0;
    while (hic<50) {
      while (digitalRead(Buttoni)==LOW) { delay(10); ++loc; hic=0;}
      if (loc > 100) { nummer=5; }
      else if (loc > 3) { loc=0; ++nummer;}
      if (digitalRead(Buttoni)==HIGH) { delay(10); ++hic; }
    }
    if (4<nummer) {rele(1,0);rele(2,0);rele(3,0);rele(4,0); }
    else if (0<nummer) { rele(nummer,1); }
  }
  WiFiClient client = server.available();     
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
	  client.println("<!DOCTYPE html><html>");
 /*          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html"); */
	  client.println("<!DOCTYPE html><html>");
	  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
	  client.println("<link rel=\"icon\" href=\"data:,\">");
	  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
	  client.println(".button { background-color: #f3db3b; border: 30 ; color: black; padding: 16px 40px;");
	  client.println("text-decoration: none; font-size: 50px; margin: 2px; cursor: pointer;}");
	  client.println(".button2 {background-color: #ff3425;}</style></head>");

          client.print("<HTML><title>4 relay</title>");
          client.print("<center>");
          client.print("<body><h1>Releet</h1>");
	  for (int r=1;r<5;++r) {
	    if (releet[r]==0) {
              client.println("<p><a href=\"/r"+String(r)+"on\"><button class=\"button\">"+String(r)+"</button></a></p>");}
	    else {
              client.println("<p><a href=\"/r"+String(r)+"off\"><button class=\"button button2\">"+String(r)+"</button></a></p>"); } }
          client.print("<a href=\"/rKon\"\"> R* <button>ON</button></a>");
          client.print("<a href=\"/rKoff\"\"> <button>OFF</button></a> <p> <p>");
	  /*         client.print("<a href=\"/up\"\"> SERVO <button>UP</button></a> ");
          client.print("<a href=\"/stop\"\"> <button>STOP</button></a>");
          client.print("<a href=\"/down\"\"> <button>DOWN</button></a> <p> "); */
          client.print("</body></HTML>");
          break;        // break out of the while loop:
        }
        if (c == '\n') { 
          currentLineIsBlank = true;
          buffer="";       
        } 
        else 
          if (c == '\r') {     
	    if(buffer.indexOf("GET /up")>=0) { rele(1,1); rele(2,0); suunta=UP; };
	    if(buffer.indexOf("GET /down")>=0) { rele(1,0); rele(2,1); suunta=DOWN; };
	    if(buffer.indexOf("GET /stop")>=0) { rele(1,0); rele(2,0); suunta=DONTKNOW; };
	    if(buffer.indexOf("GET /r1on")>=0) rele(1,1);
	    if(buffer.indexOf("GET /r1off")>=0) rele(1,0);
	    if(buffer.indexOf("GET /r2on")>=0) rele(2,1);
	    if(buffer.indexOf("GET /r2off")>=0) rele(2,0);
	    if(buffer.indexOf("GET /r3on")>=0) rele(3,1);
	    if(buffer.indexOf("GET /r3off")>=0) rele(3,0);
	    if(buffer.indexOf("GET /r4on")>=0) rele(4,1);
	    if(buffer.indexOf("GET /r4off")>=0) rele(4,0);
	    if(buffer.indexOf("GET /rKon")>=0) { rele(1,1); rele(2,1); rele(3,1); rele(4,1); }
	    if(buffer.indexOf("GET /rKoff")>=0) { rele(1,0); rele(2,0); rele(3,0); rele(4,0); }
	    if(buffer.indexOf("GET /r")>=0) {rpaivitys();}
	  }
        else {
          currentLineIsBlank = false;
        }  
      }
    }
    client.stop();
  }}
