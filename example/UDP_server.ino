
#include <WiFiUDP.h>


unsigned int localPort = 12345;      // local port to listen for UDP packets
byte packetBuffer[512]; //buffer to hold incoming and outgoing packets
WiFiUDP Udp;// A UDP instance to let us send and receive packets over UDP

void UDPServerInit(){
  Udp.begin(localPort);
  SerialComunication::info("UDPServer", "UDPServer", "Created UDP server on port : ");
  SerialComunication::info("UDPServer", "UDPServer", String(localPort));
}

String UDPServerReadData()
{
  int noBytes = Udp.parsePacket();
  String received_command = "";

  if ( noBytes ) {

    SerialComunication::info("UDPServer", "readData", "Recived Packet from: ");
    SerialComunication::info("UDPServer", "readData", String(Udp.remoteIP() ));

    // We've received a packet, read the data from it
    Udp.read(packetBuffer,noBytes); // read the packet into the buffer

    // display the packet contents in HEX
    for (int i=1;i<=noBytes;i++)
    {
      //Serial.print(packetBuffer[i-1],HEX);
      received_command = received_command + char(packetBuffer[i - 1]);
      /*if (i % 32 == 0)
      {
        Serial.println();
      }
      else Serial.print(' ');*/
    } // end for
    /*Serial.println();*/

    SerialComunication::info("UDPServer", "readData", "Message :  ");
    SerialComunication::info("UDPServer", "readData", received_command);
    return received_command;
  } // end if
}
void UDPServerWriteData(String message){
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.print(message);
  Udp.endPacket();
}
