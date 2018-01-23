#include <ArduinoJson.h>

//Polypark Beta Arduino Code !!!!
//BAD UPDATE LATER


#include <SPI.h>
#include <Ethernet.h>


/******************** ETHERNET SETTINGS ********************/

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x85, 0xD9 };   //physical mac address
byte ip[] = { 192, 168, 0, 112 };                   // ip in lan can change
byte subnet[] = { 255, 255, 255, 0 };              //subnet mask
byte gateway[] = { 192, 168, 0, 1 };              // default gateway
EthernetServer server(80);                       //server port
const int trigPin = 3;
const int echoPin = 4;
long duration;
int distance;

void setup()
{
Ethernet.begin(mac,ip,gateway,subnet);     // initialize Ethernet device
server.begin();                                // start to listen for clients
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);                            // input pin for switch
Serial.begin(9600);


}

void loop()
{

EthernetClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connnection: close");
client.println();
// Clears the trigPin

// All for sensing distance//

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance = duration*0.034/2;
// Prints the distance on the Serial Monitor
// Debug
Serial.print("Distance: ");
Serial.println(distance);

StaticJsonBuffer<200> jsonBuffer;
JsonArray& array = jsonBuffer.createArray(); // Create the JSON do not make global or it will grow like crazy!
array.add("distance"); // push distance to our JSON can change this to spot number for multiple spots.... 
array.add(distance); // push values
array.printTo(Serial); // debug serial print
array.printTo(client); //raw JSON to server
    // look for the client
// send a standard http response heade

/* 
This portion is the webpage which will be
sent to client web browser one can use html , javascript
and another web markup language to make particular layout 
*/

client.println("<!DOCTYPE html>");      //web page is made using html
client.println("<html>");
client.println("<head>");
client.println("<title>PolyPark Beta</title>");
client.println("<meta http-equiv=\"refresh\" content=\"1\">");

/*
The above line is used to refresh the page in every 1 second
This will be sent to the browser as the following HTML code:
<meta http-equiv="refresh" content="1">
content = 1 sec i.e assign time for refresh 
*/

client.println("</head>");
client.println("<body>");
client.println("<h1>PolyPark Test </h1>");

client.print("<h2>Parking Space is:  </2>");

if (distance < 50) // can change, 50 seems good.
{
client.println("<h3>full</h3>");
}
else
{
client.println("<h3>empty</h3>");
}

client.println("</body>");
client.println("</html>");

delay(1);         // giving time to receive the data

/*
The following line is important because it will stop the client
and look for the new connection in the next iteration i.e
EthernetClient client = server.available();
*/
client.stop();
}
