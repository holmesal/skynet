/*

  This script uses the WiFi shield to link a device with the Skynet network.
  First, it tries to connect to every WiFi network in range, using the password "skynet".
  Eventually, it will connect to the personal hotspot running on the phone, with WPA encryption and
    the password "skynet"
  Then, it calls the skynet server and says "Awww yeah. I'm online." It also passes a list of SSIDs found.
  The server tells the phone that the device is up and running, and passes the list of SSIDs.
  The user selects their SSID from the list and enters their password.
  The phone passes the PW to the server, which passes it to the device.
  The device disconnects from the phone's personal hotspot and attempts to connect using the SSID and PW provided.
  When it succeeds, it calls the server and says "Woohoo! I'm up and running on a home network!"
  
  So here's the layout for the script:
  Setup:
  Set mode to "find-phone"
  (In the future, this is where previous mode and such will be stored)
  Loop:
  Dial WiFi networks until you find one that accepts the password "skynet"
  Set mode to "linked-phone"
  Call server and pass list of WiFi networks in range
  Wait for response SSID and password
  (cycle)
  Get response SSID and password
  Set mode to "find-home"
  Connect to the home network using the SSID and pass provided by the server
  Set mode to "linked-home"
  Call server and pass OK status
*/
  
  //Libraries
  #include <SPI.h>
  #include <WiFi.h>
  
  //Static variables
  char pass[50] = "skynetinc";
  char ssid[50] = "somessid";
  WiFiClient client;
  char server[] = "www.skynet-alonso.appspot.com";
//  String mode = FIND_PHONE;
  
  //modes
  /*
  int FIND_PHONE = 0;
  int LINKED_PHONE = 1;
  int FIND_HOME = 2;
  int LINKED_HOME = 3;
  */
  int mode = 0;
  int status = 0;
  
  //led pins
  int blue = 9;
  int green = 8;
  
  void setup(){
    //Init serial comms
    Serial.begin(9600);
    while (!Serial) {
      ; //wait (for Leonardo)
    }
    
    //Set initial mode
    //later on, read mode from some other source
    mode = 0;
    
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present"); 
      // don't continue:
      while(true);
    }
    
    // Print WiFi MAC address:
    printMacAddress();
    
    //led pins
    pinMode(blue,OUTPUT);
    pinMode(green,OUTPUT);
    
//    digitalWrite(blue,HIGH);
//    digitalWrite(green,HIGH);
    
    Serial.println(WL_IDLE_STATUS);
    Serial.println(WL_CONNECTED);
    Serial.println(WL_DISCONNECTED);
    Serial.println(WL_CONNECT_FAILED);
    Serial.println(WL_SCAN_COMPLETED);
    
  }
  
  void loop(){
    
    //switch on the current mode
    switch(mode){
      case 0:
        digitalWrite(blue,HIGH);
        Serial.println("Mode: find-phone");
        dialskynet();
        digitalWrite(blue,LOW);
        break;
      case 1:
        digitalWrite(blue,HIGH);
        Serial.println("Mode: linked-phone");
        getssidpass();
        break;
      case 2:
        digitalWrite(green,LOW);
        digitalWrite(blue,LOW);
        Serial.println("Mode: find-home");
        connecttohome();
        digitalWrite(green,HIGH);
        break;
      case 3:
        digitalWrite(blue,LOW);
        digitalWrite(green,HIGH);
        Serial.println("Mode: linked-home");
//        connecttohome();
        break;
    }
    
    //don't loop too fast
    delay(2000);
    
  }
  
  void dialskynet(){
    //scan for networks
    int numSsid = WiFi.scanNetworks();
    if (numSsid == -1)
    { 
      Serial.println("Couldn't get a wifi connection");
      while(true);
    } 
    
    // print the list of networks seen:
    Serial.print("number of available networks:");
    Serial.println(numSsid);
    
    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
      Serial.print(thisNet);
      Serial.print(") ");
      Serial.print(WiFi.SSID(thisNet));
//      Serial.print(ssid);
      Serial.print("\tSignal: ");
      Serial.print(WiFi.RSSI(thisNet));
      Serial.print(" dBm");
      Serial.print("\tEncryption: ");
      printEncryptionType(WiFi.encryptionType(thisNet));
    }
    
    //attempt to connect to each network
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(WiFi.SSID(thisNet));
      
      //convert the password
//      char charBuf[50];
//      pass.toCharArray(charBuf,50);
      
      //attempt to connect
      status = WiFi.begin(WiFi.SSID(thisNet), pass);
      
      delay(10000);
      Serial.println(status);
      
      if (status == WL_CONNECTED){
        Serial.println("HOORAY.");
        mode = 1;
        break;
      } 
      
    }
    
  }
  
  void getssidpass(){
    String ssidstring = "CarlTheRouter";
    ssidstring.toCharArray(ssid,50);
    
    String passstring = "thebatcave72";
    passstring.toCharArray(pass,50);
    
    //manually connect to server
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client.println("GET /search?q=arduino HTTP/1.1");
      client.println("Host:www.google.com");
      client.println("Connection: close");
      client.println();
    }
    
    //disconnect the wifi link
//    WiFi.disconnect();
    //no status returned, have to manually set
//    status = WL_IDLE_STATUS;
//    mode = 2;
  }
  
  void connecttohome(){
    while (status != WL_CONNECTED){
      Serial.print("Attempting to connect to: ");
      Serial.println(ssid);
      status = WiFi.begin(ssid, pass);
      Serial.println(status);
      delay(10000);
      Serial.println(status);
    }
    
    mode = 3;
  }
  
  void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];                     

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
  case ENC_TYPE_WEP:
    Serial.println("WEP");
    break;
  case ENC_TYPE_TKIP:
    Serial.println("WPA");
    break;
  case ENC_TYPE_CCMP:
    Serial.println("WPA2");
    break;
  case ENC_TYPE_NONE:
    Serial.println("None");
    break;
  case ENC_TYPE_AUTO:
    Serial.println("Auto");
    break;
  } 
}
