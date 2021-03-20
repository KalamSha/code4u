#include <ESP8266WiFi.h>     //Include Esp library
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
    #include <MFRC522.h>        //include RFID library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include"oled.h" 



//*******************Adafruit_SSD1306*****************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//*******************************BUTTONS CONFIG*******************************
const int button = 10; //sd3
const int button2=9;//sd2
int temp = 0;
int temp2=0;
//****************************************************************************

//*****************************WIFI CONFIG***********************************
const char *ssid = "AndroidAP606F";  //ENTER YOUR WIFI SETTINGS
const char *password = "tdyc7331";
char server[] = "192.168.43.60";

//***************************************************************************

//*****************************FINGERPRINT SENSOR****************************
int FingerID=0;
String BookID="";
SoftwareSerial mySerial(2, 16);// use 2,16 fixed
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//**************************************************************************

//****************************RFID*****************************************
#define SS_PIN D8 //RX slave select
#define RST_PIN D3
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

//*************************************************************************

void setup() {
  Serial.begin(9600);

 //-----------initiate OLED display-------------
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  // Display bitmap
  display.drawBitmap(0, 0, title , 128, 64, WHITE);
  display.display();
  delay(3000);
  
  display.clearDisplay();
  display.drawBitmap(0, 0,   coordinator, 128, 64, WHITE);
  display.display();
  delay(3000);
  
  display.clearDisplay();
  display.drawBitmap(0, 0,  guide, 128, 64, WHITE);
  display.display();
  delay(3000);
  
  display.clearDisplay();
  display.drawBitmap(0, 0,  we, 128, 64, WHITE);
  display.display();
  delay(3000);

  
  
  pinMode(button, INPUT);
  pinMode(button2, INPUT);
  

  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.print(ssid);// Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  display.clearDisplay();
  display.drawBitmap(0, 0,  Wifi_connected_bits, 63, 49, WHITE);
  display.display();
  
  Serial.println("");
  Serial.println("Connected");
  Serial.print("IP address: ");//If connection successful show IP address in serial monitor
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP
  
  finger.begin(57600);//Init Fingerprint sensor
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
}

void loop()
{f:int temp3=0;
int temp4=0;
  String action="";
  temp = digitalRead(button);
  temp2=digitalRead(button2);
display.clearDisplay();
  display.drawBitmap(0, 0,  intropic, 128, 64, WHITE);
  display.display();

if(temp == HIGH)
 {
  action="getid.php";
 }
 else if(temp2==HIGH){
  action="fine.php";
 }

if(action!=""){
  Serial.println("Action");

 
 display.clearDisplay(); 
 display.drawBitmap(0, 0,  placefinger, 128, 64, WHITE);
  display.display();
  delay(1000);

  
 FingerID=getFingerprintIDez();
 delay(1000); 
 while(temp3>50^FingerID<=0 )
 {
  temp3=temp3+1;
 
  FingerID=getFingerprintIDez();
 //No finger detected


 if(FingerID ==-1){
    display.clearDisplay();
    display.drawBitmap( 32, 0, FinPr_invalid_bits, FinPr_start_width, FinPr_start_height, WHITE);
    display.display();
  }
   else{
    display.clearDisplay();
    display.drawBitmap( 32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
    display.display();
  }

  
  }
  if(temp3>50 )
  {
    goto f;
  }
  Serial.println(FingerID);
    display.clearDisplay();
display.drawBitmap(34, 0,FinPr_valid_bits, 64, 64, WHITE);
  display.display();
delay(1000);
display.clearDisplay();
    display.drawBitmap(0, 0,placebook, 128, 64, WHITE);
  display.display();
  delay(2000);

n: display.clearDisplay();
  display.drawBitmap(0, 0,rfidscan, 128, 64, WHITE);
  display.display();
  
  BookID=RFID();
  
  while(temp4>70^BookID==""){
display.clearDisplay();
  display.drawBitmap(0, 0,rfidscan, 128, 64, WHITE);
  display.display();
  
  BookID=RFID();
  temp4++;
  }
  if(temp4>70){
    goto f;
  }
//  display.clearDisplay();
//  display.println("do you want to add more books");
//  if(temp==HIGH)
//    goto n;
//  else if(temp2==HIGH)
//  continue;
  Serial.println(BookID);
  WiFiClient client;
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  
  delay(1000);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
    display.println("Updating....");
    display.display();
  Serial.println("connecting...");
  if (client.connect(server, 80)) 
  {
  Serial.println("connected"); 
 
    delay(3000);
    
    Serial.print("GET /library/"+action+"?studentid=FingerID&bookid=CardIDHTTP/1.1");
    client.print("GET /library/"+action+"?FingerID=");
    client.print(FingerID);
    client.print("&Book_ID=");
    client.print(BookID);
    client.println(" HTTP/1.1");
    client.println("Host:192.168.43.60");
    client.println("Connection: close");
    client.println();
display.clearDisplay();
    display.drawBitmap(0, 0,last, 128, 64, WHITE);
  display.display();
  delay(3000);
  
  
  }
else{
  
display.clearDisplay();
    display.drawBitmap(0, 0,tryagain, 128, 64, WHITE);
  display.display();
  delay(2000);

  }


}
}
  


//****************************Finger print************************************************
uint8_t getFingerprintID() {
  display.clearDisplay();
  display.drawBitmap(34, 0,FinPr_scan_bits, 64, 64, WHITE);
  display.display();
  delay(2000);
  
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      //////////////
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
display.clearDisplay();
display.drawBitmap(34, 0,FinPr_valid_bits, 64, 64, WHITE);
  display.display();
  delay(2000);

  

    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {

    display.clearDisplay();
display.drawBitmap(34, 0,FinPr_invalid_bits, 64, 64, WHITE);
  display.display();
  
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
   return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int  getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return 0 ;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  return finger.fingerID; 
}


String RFID()
{
  
String CardID="";
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return"";//got to start of loop if there is no card present
 }
 // Select one of the cards  
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return"";//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }

 for (byte i = 0; i < mfrc522.uid.size; i++) {
     CardID+= mfrc522.uid.uidByte[i];
     
}
return (CardID);
}
