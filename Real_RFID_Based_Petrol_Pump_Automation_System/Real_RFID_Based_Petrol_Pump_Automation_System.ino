#include <SPI.h>  //synchronous serial data
#include <MFRC522.h>  //rfid library
#include <Wire.h> // allows to communicate with i2c/twi devices
#include <LiquidCrystal_I2C.h> // lcd i2c library
LiquidCrystal_I2C lcd(0x27,20,4); //i2c address and lcd layout
#define SS_PIN 10 // sda on rfid
#define RST_PIN 9  // rst on rfid
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
#define led 2 
int markp;
#include <Keypad.h> //keypad library
boolean process, sell = false; 
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
   {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
  };
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int relayTrig = 7;  //trigger the base transistor
boolean buzin =true;

const String password="1234";

int pass[] ={4,3,2,1};

String input_password;


void setup() 
{

  input_password.reserve(32);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
 pinMode( relayTrig,OUTPUT);
 Serial.println("Put your card to the reader...");
 Serial.println();
  lcd.init();                      // initialize the lcd 
 // Print a message to the LCD
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Welcome!");
  lcd.setCursor(2,1);
  lcd.print("Swipe your card!");
  
 

}

 




void loop() {
  // Look for new cards




  
  
   
 { if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  
  if (content.substring(1) =="12 DE D7 34" ) //change here the UID of the card/cards that you want to give access
  {
    process=true;
       lcd.clear();
 
    
    Serial.println("Card is identified");
     lcd.print("Card is identified");
     lcd.setCursor(2,1);

     checkpass();
   
       
  }
 else   {
  lcd.clear();
    lcd.print(" Access denied");
digitalWrite(relayTrig,0);
    delay(2000) ;}
   
}



      
     


}


void checkpass(){

 lcd.print("Input  password");  
    int input_count=0;
    while(input_count<4){
    
       char customKey = customKeypad.getKey();
       if(customKey){
        Serial.println(customKey);
           int cm=(customKey);
           cm=cm-48;
         
       
         //Serial.println(input_count);
         //Serial.println(pass[input_count]);
        
     if(cm==pass[input_count]){
      Serial.println(pass[input_count]);
      markp++;

       //Serial.println(markp);
     
      
      
     }
     //Serial.println(markp);
     customKey=' ';
      input_count++;
    }
    }

    if(markp==4){

       Serial.println("Granted");

       sell=true;
      
      
    }

    if(markp!=4){

         Serial.println("Wrong password");

           Serial.println("Press '#' to retry");
         char customKey = customKeypad.getKey();
         while(customKey!='#'){

           char customKey = customKeypad.getKey();
          if(customKey=='#'){
           checkpass();
     
          }
           
          
         }
         
      
    }

  
}


void sell(){







  
}





 
