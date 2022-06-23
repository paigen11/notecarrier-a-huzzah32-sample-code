// sample code for Huzzah32 with Notecarrier A attached via breadboard and jumper wires
#include <Notecard.h>

#include <NotecardPseudoSensor.h>
using namespace blues;

#define usbSerial Serial
#define txRxPinsSerial Serial1

Notecard notecard; // not used but required by NotecardPseudoSensor library
NotecardPseudoSensor sensor(notecard);

#define NOTE_PRODUCT_UID "com.blues.pniedringhaus:huzzah32_notecarrier_a"

void setup()
{
  usbSerial.begin(115200);
  usbSerial.println("Starting...");

  // note-arduino library implementation
  notecard.begin(txRxPinsSerial, 9600);
  notecard.setDebugOutputStream(usbSerial);
  J *req = notecard.newRequest("hub.set");
  JAddStringToObject(req, "product", NOTE_PRODUCT_UID);
  JAddStringToObject(req, "mode", "continuous");
  notecard.sendRequest(req);

  // serial implementation (no note-arduino library required)
  // txRxPinsSerial.begin(9600);
  // notecard.begin();
  // txRxPinsSerial.println("\n");

  // txRxPinsSerial.println("{\"req\":\"hub.set\",\"product\":\"" NOTE_PRODUCT_UID "\",\"mode\":\"continuous\"}");

  // delay(250);
  // while(txRxPinsSerial.available() > 0) {
  //   char incomingByte = txRxPinsSerial.read();
  //   if(incomingByte != '\r' && incomingByte != '\n') {
  //     usbSerial.print(incomingByte);
  //   }
  // }
  // usbSerial.println();
}

void loop()
{
  float temperature = sensor.temp();
  float humidity = sensor.humidity();

  usbSerial.print("Temperature = ");
  usbSerial.print(temperature);
  usbSerial.println(" *C");
  usbSerial.print("Humdity = ");
  usbSerial.print(humidity);
  usbSerial.println(" %");

  delay(15000);

  // note-arduino library implementation
  J *req = notecard.newRequest("note.add");
  if (req != NULL)
  {
    JAddStringToObject(req, "file", "sensors.qo");
    JAddBoolToObject(req, "sync", true);
    J *body = JAddObjectToObject(req, "body");
    if (body)
    {
      JAddNumberToObject(body, "temp", temperature);
      JAddNumberToObject(body, "humidity", humidity);
    }
    notecard.sendRequest(req);
  }

  // serial implementation (no note-arduino library required)
  // char json_request[150];
  // snprintf(json_request, sizeof(json_request),
  //   "{"
  //     "\"req\":\"note.add\","
  //     "\"file\":\"sensors.qo\","
  //     "\"sync\":true,"
  //     "\"body\":{"
  //       "\"temp\":%d.%02d,"
  //       "\"humidity\":%d.%02d"
  //     "}"
  //   "}",
  //     (int)temperature, abs(((int)(temperature*100.0)%100)),
  //     (int)humidity, (int)(humidity*100.0)%100
  // );

  // txRxPinsSerial.println(json_request);
  // delay(250); // wait for the Notecard to respond
  // while (txRxPinsSerial.available() > 0) {
  //     char incomingByte = txRxPinsSerial.read();
  //     if (incomingByte != '\r' && incomingByte != '\n') {
  //         usbSerial.print(incomingByte);
  //     }
  // }
  // usbSerial.println();
}
