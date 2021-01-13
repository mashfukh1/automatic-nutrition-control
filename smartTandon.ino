#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>
#include "GravityTDS.h"
//OLED//
boolean backlight = true;
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET     4 
#define NUMFLAKES     10 
/////////////////
int yel = 0;
//TDS SENSOR//
#define TdsSensorPin A0
GravityTDS gravityTds;
float temperature = 25,tdsValue = 0,tdsValue2 = 0;
/////////////////

int addr = 0;
//FLOW SENSOR//
volatile int flow_frequency; // Measures flow sensor pulses
double vol = 0.00,l_minute;
unsigned long currentTime;
unsigned long cloopTime;
/////////////////
int vaL=0;
int valB=0;
// RELAY CONTROL //
#define MTAND 12
#define VTAND 11
#define VMIXB 10
#define MMIXB 9
#define VMIXA 8
#define MMIXA 7
//////////////////

//ENCODER//
#define encoder0PinA 18 // A
#define encoder0PinB  19 // B
#define SW  17 // switch
int counter = 0;
int counterL = 0;
int aState;
int aLastState;
double ppm =0.00;
double ppmL =0.00;
double volume = 0.0;
double volumeB = 0.0;
double volumeRealA = 0.0;
double volumeRealB = 0.0;
int Switch = 'V';
int SwitchMenu = 'M';
int SwitchMenuUtama = 'K';
int flowCounter = 0;
int choose;
int memori = 0;
int saveMemori=0;
//////////////////

//INPUT//
#define L2TAND  A1
#define L1TAND  A2
#define L2MIXB  A3
#define L1MIXB  A4
#define L2MIXA  A5
#define L1MIXA  A6
#define FLOWB    3
#define FLOWA    2
/////////////////

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
/////////////////////
pinMode(MTAND,OUTPUT);
pinMode(VTAND,OUTPUT);
pinMode(VMIXB,OUTPUT);
pinMode(MMIXB,OUTPUT);
pinMode(VMIXA,OUTPUT);
pinMode(MMIXA,OUTPUT);
digitalWrite(MTAND,HIGH);
digitalWrite(VTAND,HIGH);
digitalWrite(VMIXB,HIGH);
digitalWrite(MMIXB,HIGH);
digitalWrite(VMIXA,HIGH);
digitalWrite(MMIXA,HIGH);
/////////////////////

/////////////////////
pinMode(encoder0PinA,INPUT);
pinMode(encoder0PinB,INPUT);
pinMode(SW,INPUT_PULLUP);
/////////////////////

/////////////////////
pinMode(L2TAND,INPUT); ////////
pinMode(L1TAND,INPUT);
pinMode(L2MIXB,INPUT_PULLUP);
pinMode(L1MIXB,INPUT);
pinMode(L2MIXA,INPUT); ////////
pinMode(L1MIXA,INPUT_PULLUP);
pinMode(FLOWB,INPUT);
pinMode(FLOWA,INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(FLOWA), flow, RISING); // Setup Interrupt
currentTime = millis();
cloopTime = currentTime;
////////////////////

////////////////////////////
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  ////////////////////////////

  
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.begin();      
display.clearDisplay();
delay(1000);
Serial.begin(9600);
Serial3.begin(9600);
aLastState = digitalRead(encoder0PinA);
digitalWrite(MTAND,HIGH);
digitalWrite(VTAND,HIGH);
digitalWrite(VMIXB,HIGH);
digitalWrite(MMIXB,HIGH);
digitalWrite(VMIXA,HIGH);
digitalWrite(MMIXA,HIGH);
STARTING();
delay(2000);
LOADING();
STARTING2();
awal();
}
void loop(){
yangTersimpan();
loop1();
  }
void loopDTS(){
 while(digitalRead(17)==HIGH){
   DTS();
   }
  }
void loop2(){
 while(digitalRead(17)==HIGH){
   valueChoose();
   }
  }
void loop1(){ 
 while(valB<1){
     Serial.println("awal");
     valueINPPM();
   if (digitalRead(SW)==LOW){
      Serial.println(saveMemori);
      int normal = saveMemori/10;
      EEPROM.write(3,normal);
      delay(1000);
      int no = EEPROM.read(3);
      Serial.println(no);
      delay(2000);
      LOADING();
      display.setTextSize(1);
      display.clearDisplay();
      display.setTextColor(WHITE,BLACK );
      display.setCursor(10, 2);
      display.println("VALUE");
      display.setTextSize(2);
      display.setCursor(24, 15);
      display.println("SAVED");
      display.display();
      delay(1000);
      valB=3;     
       }
     if(valB>1)
      vaL=0; 
      Switch = 'V';
      break; 
      }  
    }
void utama(){
while(vaL < 1){
while(Switch == 'U'){
  STARTING2();
  delay(2000);
  awal();
  delay(1000);
  cekListrik();
  Switch = 'A';
  break;
  }
while(Switch == 'A'){
  while(digitalRead(L1MIXA)==LOW && yel < 101 ){
    yel++;
    Serial.println(yel);
    if(yel >= 100){
    Switch = 'C';
    digitalWrite(VMIXA,HIGH);
    break;
    }
  }
   while(digitalRead(L1MIXA)==HIGH){
    yel = 0; 
    digitalWrite(VMIXA,LOW);
    displayString(1,"LOAD WATER",35,10);
    }
 }
while(Switch == 'C'){
    displayString(1,"Procesing...",35,10);
    delay(1000);
    cekListrik();
    cekNutrisi();
    isiNutrisi();
    digitalWrite(MMIXA,HIGH);
    digitalWrite(MMIXB,HIGH);
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE,BLACK );
    display.setCursor(10, 2);
    display.println("A & B NUTRITION");
    display.setTextSize(2);
    display.setCursor(24, 15);
    display.println("FINISH");
    display.display();
    delay(2000);
    mixing(); 
    digitalWrite(VTAND,HIGH);
    Switch = 'D';
    break; 
  }
 while(Switch == 'D'){
    if(digitalRead(L1MIXB)==HIGH){
       Switch = 'V';
       digitalWrite(VMIXB,HIGH);
       display.setTextSize(1);
       display.clearDisplay();
       display.setTextColor(WHITE,BLACK );
       display.setCursor(10, 2);
       display.println("LOAD WATER");
       display.setTextSize(2);
       display.setCursor(24, 15);
       display.println("FINISH");
       display.display();
       break;
       } 
       else{
        digitalWrite(VMIXB,LOW);
        displayString(1,"LOAD WATER",35,10);
        }
      }
  while(Switch == 'V'){
    Serial.println(vaL);
      if(digitalRead(L2MIXB)== HIGH){
         Switch ='A';
      }
      else if(digitalRead(SW)== LOW){
          Switch = 'Z';         
         }
       else{
         STARTING2DTS();
         Serial.println("nang V");
        }
      }
    while(Switch == 'Z'){
          vaL++;       
      if(vaL > 1){
          break;
       } 
      }
    Serial.print("GOOD BYe");    
  }
}
void yangTersimpan(){
while(vaL < 1){
while(Switch == 'U'){
  STARTING2();
  delay(2000);
  awal();
  delay(1000);
  cekListrik();
  Switch = 'A';
  break;
  }
while(Switch == 'A'){
  while(digitalRead(L1MIXA)==LOW && yel < 101 ){
    yel++;
    Serial.println(yel);
    if(yel >= 100){
    Switch = 'C';
    digitalWrite(VMIXA,HIGH);
    break;
    }
  }
   while(digitalRead(L1MIXA)==HIGH){
    yel = 0; 
    digitalWrite(VMIXA,LOW);
    displayString(1,"LOAD WATER",35,10);
    }
  }
while(Switch == 'C'){
    displayString(1,"Procesing...",35,10);
    delay(1000);
    cekListrik();
    cekNutrisi();
    isiNutrisiSave();
    digitalWrite(MMIXA,HIGH);
    digitalWrite(MMIXB,HIGH);
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE,BLACK );
    display.setCursor(10, 2);
    display.println("A & B NUTRITION");
    display.setTextSize(2);
    display.setCursor(24, 15);
    display.println("FINISH");
    display.display();
    delay(2000);
    mixing(); 
    digitalWrite(VTAND,HIGH);
    Switch = 'D';
    break; 
  }
 while(Switch == 'D'){
    if(digitalRead(L1MIXB)==HIGH){
       Switch = 'V';
       digitalWrite(VMIXB,HIGH);
       digitalWrite(MTAND,HIGH);
       display.setTextSize(1);
       display.clearDisplay();
       display.setTextColor(WHITE,BLACK );
       display.setCursor(10, 2);
       display.println("LOAD WATER");
       display.setTextSize(2);
       display.setCursor(24, 15);
       display.println("FINISH");
       display.display();
       break;
       } 
       else{
        digitalWrite(VMIXB,LOW);
        digitalWrite(MTAND,LOW);
        displayString(1,"LOAD WATER 2",35,10);
        }
      }
  while(Switch == 'V'){
    Serial.println(vaL);
      if(digitalRead(L2MIXB)== HIGH){
         Switch ='A';
      }
      else if(digitalRead(SW)== LOW){
          Switch = 'Z';         
         }
       else{
         STARTING2DTS();
         Serial.println("nang V");
        }
      }
    while(Switch == 'Z'){
      Serial.println(vaL);
        if(digitalRead(SW)==LOW){
          vaL++;       
         }
        else if(vaL > 1){
          valB=0;
          break;
       } 
      }
    Serial.print("GOOD BYe");    
  }
}
void mixing(){
 for(int i=0;i<20;i++){
   digitalWrite(VTAND,LOW);
   for(int i=0;i<100;i++){   
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE,BLACK );
    display.setCursor(17, 5);
    display.println("MIXING PROCESS");   
    drawProgressbar(0,15,120,10,i);
    display.display();
    delay(8);
     }
    for(int i=0;i<100;i++){   
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE,BLACK );
    display.setCursor(17, 5);
    display.println("MIXING PROCESS");   
    drawProgressbar(0,15,120,10,i);
    display.display();
    delay(8);
     }
   }
  }
void valueINPPM() {
aState = digitalRead(encoder0PinA);
if (aState != aLastState){
if (digitalRead(encoder0PinB) != aState) {
counter += 10;
display.clearDisplay();
} else {
counter -=10;
display.clearDisplay();
}
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.drawRect(0,0,128,32,WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("INPUT PPM :");
    display.setTextSize(2);
    display.setCursor(30,12);
    display.println(counter); 
    display.setTextSize(1);
    display.setCursor(100, 17);
    display.println("PPM");
    display.display();
}
 aLastState = aState;
 saveMemori = counter;
 ppm = counter;
}
int valueChoose() {
aState = digitalRead(encoder0PinA);
if (aState != aLastState){
if (digitalRead(encoder0PinB) != aState) {
counterL += 1;
display.clearDisplay();
} else {
counterL -=1;
display.clearDisplay();
}
}
if(counterL>6){
  counterL=6;
  }
else if(counterL< 0){
  counterL=0;
  }
else if(counterL >= 2 && counterL<=4){
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,3);
    display.print("NEW SET"); 
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print("PPM:");
    display.setTextSize(1);
    display.setCursor(20,25);
    display.println(tdsValue,0); 
    display.display();  
    choose=2;
}
else if (counterL >=0 && counterL<=2){
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,3);
    display.print("LAST SET"); 
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print("PPM:");
    display.setTextSize(1);
    display.setCursor(20,25);
    display.println(tdsValue,0); 
    display.display();
    choose=1;
  }
 else if (counterL >=4 && counterL <=6){
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,3);
    display.print("DTS"); 
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print("PPM:");
    display.setTextSize(1);
    display.setCursor(20,25);
    display.println(tdsValue,0); 
    display.display();
    choose=3;
  }
 else {
  choose = choose;
  }
 aLastState = aState;
 return choose;
}
void displayString(int size,String kata, int x, int y)
{
    display.setTextColor( WHITE,BLACK);
    display.setTextSize(size);
    display.clearDisplay();
    display.setCursor(x, y);
    display.println(kata); 
    display.setTextSize(size);
    display.display();
}
void STARTING()
{
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE,BLACK );
    display.setTextSize(2);
    display.setCursor(20, 10);
    display.println("WELCOME");
    display.display();
}
void STARTING2()
{
DTS(); 
}
void STARTING2DTS()
{
DTS();
}
void awal(){
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.drawRect(0,0,128,32,WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("INPUT PPM :");
    display.setTextSize(2);
    display.setCursor(30,12);
    display.println(0); 
    display.setTextSize(1);
    display.setCursor(100, 17);
    display.println("PPM");
    display.display();
  }
  void awalL(){
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.drawRect(0,0,128,32,WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("INPUT mL :");
    display.setTextSize(2);
    display.setCursor(30,12);
    display.println(0); 
    display.setTextSize(1);
    display.setCursor(100, 17);
    display.println("PPM");
    display.display();
  }
 void awal2(){
       display.clearDisplay();  
       display.drawRect(0,0,128,32,WHITE);
       display.setTextSize(1);
       display.setCursor(0,0);
       display.println("PENGISIAN :");
       display.setTextSize(1);
       display.setCursor(80,0);
       display.println(0);
       drawProgressbar(0,10,120,10,flowCounter);
       display.setTextSize(1);
       display.setCursor(50,25);
       display.println(0);
       display.display();
  }
void LOADING(){
  for(int i=0;i<100;i++){  
    display.clearDisplay();    
    drawProgressbar(0,10,120,10,i);
    display.display();
    delay(8);
    }
  }
 void drawProgressbar(int x,int y, int width,int height, int progress)
{
   float bar = ((float)(width-1) / 100) * progress;
   display.drawRect(x, y, width, height, WHITE);
   display.fillRect(x+2, y+2, bar , height-4, WHITE); // initailize the graphics fillRect(int x, int y, int width, int height)
  }
void isiNutrisi()
 {
  volume = (ppm/1000)*4;
  volumeB = (volume*30)/1000;
  volumeRealA = volumeB*8;
  Serial.print(ppm);
  Serial.print("\t");
  Serial.print(volume);
  Serial.print("\t");
  Serial.print(volumeB);
  Serial.print("\t");
  Serial.print(flowSensor());
  Serial.print("\t");
  Serial.println(vol);
    vol = 0.00;
    currentTime = 0;
    cloopTime = 0;
    l_minute = 0;
  delay(2000);
  Serial.print(ppm);
  Serial.print("\t");
  Serial.print(volume);
  Serial.print("\t");
  Serial.print(volumeB);
  Serial.print("\t");
  Serial.print(flowSensor());
  Serial.print("\t");
  Serial.println(vol);
  while( flowSensor() < volumeRealA ){
    digitalWrite(MMIXA,LOW);
    digitalWrite(MMIXB,LOW);
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(80,0);
    display.println(volumeB); 
    display.setTextSize(1);
    display.setCursor(110,0);
    display.println("L");  
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("PENGISIAN :");
    display.setTextSize(2);
    display.setCursor(30,12);
    display.println(flowSensor());
    display.setTextSize(2);
    display.setCursor(100,12);
    display.println("L"); 
    display.display();
    delay(8);
      }
    } 
  void isiNutrisiSave()
 {
  int ppmVal = EEPROM.read(3);
  int oke = ppmVal * 10 ;
  Serial.print("didalam isi nutrisi save");
  Serial.println(oke);
  volume = ((double)oke/1000)*4;
  volumeB = (volume*30)/1000;
  volumeRealB = volumeB*8;
  Serial.print(ppm);
  Serial.print("\t");
  Serial.print(volume);
  Serial.print("\t");
  Serial.print(volumeB);
  Serial.print("\t");
  Serial.print(flowSensor());
  Serial.print("\t");
  Serial.println(vol);
    vol = 0.00;
    currentTime = 0;
    cloopTime = 0;
    l_minute = 0;
  delay(2000);
  Serial.print(ppmVal);
  Serial.print("\t");
  Serial.print(volume);
  Serial.print("\t");
  Serial.print(volumeB);
  Serial.print("\t");
  Serial.print(flowSensor());
  Serial.print("\t");
  Serial.println(vol);
  while( flowSensor() < volumeRealB ){
    digitalWrite(MMIXA,LOW);
    digitalWrite(MMIXB,LOW);
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(80,0);
    display.println(volumeB); 
    display.setTextSize(1);
    display.setCursor(110,0);
    display.println("L");  
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("PENGISIAN :");
    display.setTextSize(2);
    display.setCursor(30,12);
    display.println(flowSensor());
    display.setTextSize(2);
    display.setCursor(100,12);
    display.println("L"); 
    display.display();
    delay(8);
      }
    } 
void flow () // Interrupt function
{
   flow_frequency++;
}
float flowSensor(){
   currentTime = millis();
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      Serial.print("Rate: ");
      Serial.print(l_minute);
      Serial.print(" L/M");
      Serial.print("\t");
      l_minute = l_minute/60;
      vol = vol +l_minute;
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.print(" L");
      Serial.print("\t");
      flow_frequency = 0; // Reset Counter
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
    }
    else {
      Serial.print("Rate: ");
      Serial.print( flow_frequency );
      Serial.print(" L/M");
      Serial.print("\t");
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.println(" L");
    }
   }
   return vol;
  }
 void DTS() {
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
    tdsValue2 = tdsValue - 200;
    display.setTextColor( WHITE,BLACK);
    display.clearDisplay();
    display.drawRect(0,0,128,32,WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("TDS Meter");
    display.setTextSize(2);
    display.setCursor(30,12);
    display.println(tdsValue2,0); 
    display.setTextSize(1);
    display.setCursor(100, 17);
    display.println("PPM");
    display.display();
    Serial3.print(tdsValue2,0);Serial3.print("#");Serial3.println("");
    delay(1000); 
}
void cekNutrisi(){
     while(digitalRead(L2TAND)== HIGH){
        Serial3.println("0*");
        delay(5000);
        digitalWrite(MTAND,HIGH);
        digitalWrite(VTAND,HIGH);
        digitalWrite(VMIXB,HIGH);
        digitalWrite(MMIXB,HIGH);
        digitalWrite(VMIXA,HIGH);
        digitalWrite(MMIXA,HIGH);
        display.clearDisplay();
        display.setTextColor(WHITE,BLACK );
        display.setCursor(10, 2);
        display.println("A & B NUTRITION");
        display.setTextSize(2);
        display.setCursor(24, 15);
        display.println("EMPTY");
        display.display();
        delay(5000);
        if(digitalRead(L2TAND)== LOW){
          break;
          }
        }
      while(digitalRead(L2MIXA)== HIGH){
        Serial3.println("0*");
        delay(5000);
        digitalWrite(MTAND,HIGH);
        digitalWrite(VTAND,HIGH);
        digitalWrite(VMIXB,HIGH);
        digitalWrite(MMIXB,HIGH);
        digitalWrite(VMIXA,HIGH);
        digitalWrite(MMIXA,HIGH);
        display.clearDisplay();
        display.setTextColor(WHITE,BLACK );
        display.setCursor(10, 2);
        display.println("A & B NUTRITION");
        display.setTextSize(2);
        display.setCursor(24, 15);
        display.println("EMPTY");
        display.display();
        delay(5000);
        if(digitalRead(L2MIXA)== LOW){
          break;
          }
        }
       } 
void cekListrik(){
  while(digitalRead(L1TAND)==LOW){
        Serial3.println("0!");
        delay(5000);
        digitalWrite(MTAND,HIGH);
        digitalWrite(VTAND,HIGH);
        digitalWrite(VMIXB,HIGH);
        digitalWrite(MMIXB,HIGH);
        digitalWrite(VMIXA,HIGH);
        digitalWrite(MMIXA,HIGH);
        display.clearDisplay();
        display.setTextColor(WHITE,BLACK );
        display.setCursor(10, 2);
        display.println("LISTRIK");
        display.setTextSize(2);
        display.setCursor(24, 15);
        display.println("PADAM");
        display.display();
        delay(5000);
        if(digitalRead(L1TAND)== HIGH){
          break;
          }
    }
  }
