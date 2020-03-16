#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library
#define FIREBASE_HOST "data2-fc835.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "5Bnam9GnpshXujLMz3egafIQoYlkcNsim0jRaMez"                    // the secret key generated from firebase
#define WIFI_SSID "Marfi"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "maruf451"                                    //password of wifi ssid
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 

#define S0 D4
#define S1 D5
#define S2 D6
#define S3 D7
#define sensorOut D8

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

#include <HX711_ADC.h>
HX711_ADC LoadCell(D3, D0);
long t;

void setup() {


  Serial.begin(9600); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
   // Initiate a serial communication
   lcd.setCursor(0,0);
   lcd.println("SMART WEIGHT        ");
   lcd.setCursor(0,1);
   lcd.println("MEASUREMENT MACHINE ");


  


  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);


  
  
  float calValue; // calibration value
  calValue = 696.0; // uncomment this if you want to set this value in the sketch 
  #if defined(ESP8266) 
 
  #endif

  
  delay(10);
  Serial.println();
  Serial.println("Starting...");
  LoadCell.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  LoadCell.start(stabilisingtime);
  if(LoadCell.getTareTimeoutFlag()) {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calValue); // set calibration value (float)
    Serial.println("Startup + tare is complete");
  }
}

void loop() {

  LoadCell.update();


  if (millis() > t + 250) {
    float i = LoadCell.getData();
    //Serial.print("Load_cell output val: ");
   
   if(i*(-1)>1)
   {   
   
    if(i>0)
    {
    Serial.println(i);
    }
     else
    {

       Serial.print("Load_cell output val: ");
    Serial.println(i*(-1));

digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Printing the RED (R) value
    Serial.print("R = ");
    Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value  
    Serial.print(" G = ");
    Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  //Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);
  
  if(950>=blueFrequency && blueFrequency>=850 && 1150>=greenFrequency && greenFrequency>=1050 && 660>=redFrequency&&redFrequency>=560)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Name: Apple         ");
    int p1= Firebase.getInt("p1p");
    Serial.print("p1:");
    Serial.print(p1);
    Serial.println("p1:");
    if(i<0)
    {
      
lcd.setCursor(2,1);
lcd.print("Weight: ");
lcd.setCursor(9,1);
lcd.println(i*(-1));
lcd.setCursor(0,2);
lcd.print("Price: ");
lcd.print(p1*i*(-1));
lcd.println("Tk.    ");
    Serial.print("Apple    ");
    Serial.print("Price is:");
    Serial.println(p1*i*(-1));

    }
    else
    {
lcd.setCursor(2,1);
lcd.print("Weight: ");
lcd.setCursor(9,1);
lcd.println(i);
lcd.setCursor(15,1);
lcd.print("g");
lcd.setCursor(0,2);
lcd.print("Price: ");
lcd.print(p1*i*(1));
lcd.println("Tk.    ");
    Serial.print("Apple    ");
    Serial.print("Price is:");
    Serial.println(p1*i*(-1));

    }   
    
  }


    
    }
    t = millis();
    
}
  }


}
