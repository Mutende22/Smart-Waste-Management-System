long duration, distance;      
const int MY_INTERVAL = 1000; 
#define TRIG_PIN D7
#define ECHO_PIN    D8
#define MAX_DUSTBIN_DISTANCE 80 
#define MIN_DISTANCE 10 //ALERT
#define SOUND D5
#define LED_FULL D6

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL2cE50m_Cw"
#define BLYNK_TEMPLATE_NAME "SmartBin"
#define BLYNK_AUTH_TOKEN "vFCcAqN1PxTKf0d7uUFDSDWrPvl5f78T"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

char ssid[] = "STUDYROOM 4G";
char pass[] = "STELLAH1000_";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V0, distance);
}
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}


void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SOUND, OUTPUT);
  pinMode(LED_FULL, OUTPUT);
   Blynk.virtualWrite(V2, 0);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  lcd.init();  // Initialize LCD
  lcd.backlight();// Turn on backlight
  lcd.setCursor(0, 0);
  Blynk.run();
  timer.run();
  Blynk.virtualWrite(V0, distance); 
  // Calculate waste level percentage
  int wasteLevelPercentage = map(distance, MIN_DISTANCE, MAX_DUSTBIN_DISTANCE, 0, 100);
  int binLevel_dist = (100 - wasteLevelPercentage);
  
  
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2);   
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW);  
  duration = pulseIn(ECHO_PIN, HIGH);  
  distance = (duration/2) / 29.1;  
  Blynk.virtualWrite(V1, binLevel_dist);
 
  // Output the distance on the Serial Monitor
  Serial.print("D-READING IN CM: ");
  Serial.print(distance);
  Serial.print("\n");
  Serial.println("BIN LEVEL: ");
 Serial.print(binLevel_dist); 
  Serial.print("%");
  Serial.print("\n ");   
  delay(1000);

  if (distance == 0 || distance > MAX_DUSTBIN_DISTANCE) {
    Serial.println("SYSTEM ISSUE");
    return;
  }


 if(distance < 10) {
  lcd.setCursor(0, 0); // Move cursor to the second line
    lcd.print("SMART_BIN SYSTEM"); 
    
    lcd.setCursor(0, 1); // Move cursor to the second line
    lcd.print("Dustbin is full");
    
    digitalWrite(LED_FULL, HIGH);
    digitalWrite(SOUND, HIGH);
     delay(200);
     digitalWrite(SOUND, LOW);
     delay(200);
     digitalWrite(SOUND, LOW);
      delay(200);
     digitalWrite(SOUND, LOW); 
     delay(200);
     digitalWrite(SOUND, LOW); 
     delay(200);
     digitalWrite(SOUND, LOW);
     
    Serial.print("Dustbin is full");
     //Blynk.virtualWrite(V2, 1);
  } else {
 lcd.setCursor(0, 0); // Move cursor to the second line
    lcd.print("SMART_BIN SYSTEM"); 
    
    lcd.setCursor(0, 1); // Move cursor to the second line
    lcd.print("Not full: "); 


     lcd.setCursor(10, 1); // Move cursor to the second line
    lcd.print(distance);

    lcd.setCursor(12, 1); // Move cursor to the second line
    lcd.print("cm");
    
    // Clear previous message
     Serial.print("Not full");
     digitalWrite(LED_FULL, LOW);

       Blynk.virtualWrite(V2, 1);
       
    
  }
  
  //delay(100); // Delay before next measurement


 
  


  
  
}
