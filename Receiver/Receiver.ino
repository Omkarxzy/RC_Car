#include<nRF24L01.h>
#include<RF24.h>
#include<SPI.h>

RF24 radio(7,8);
const byte address[]="node0";

//Front Motors Pins
#define ENA1 5
#define ENB1 6
#define IN1 4
#define IN2 3
#define IN3 2
#define IN4 A4

//Back Motors Pins
#define ENA2 9
#define ENB2 10
#define IN5 A0
#define IN6 A1
#define IN7 A2
#define IN8 A3

float PWM1;
float PWM2;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

#define buzzPin A5


void setup()
{
  pinMode(buzzPin , OUTPUT);
  digitalWrite(buzzPin , HIGH);
  delay(400);
  digitalWrite(buzzPin , LOW);
  Serial.println("Car Initiating: ");
  Serial.println(" ");
  //The above Code will tell us when the Car gets Powered ON

  radio.begin();
  radio.openReadingPipe(0 , address);
  radio.setPALevel(RF24_PA_HIGH);
  /* this function is used to set the value of PowerAmplifier Range
   you can either set PALevel to MIN , LOW , HIGH , MAX
*/
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  Serial.begin(9600);
pinMode(ENA1,OUTPUT);
pinMode(ENB1,OUTPUT);
pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);
pinMode(IN3,OUTPUT);
pinMode(IN4,OUTPUT);
pinMode(ENA2,OUTPUT);
pinMode(ENB2,OUTPUT);
pinMode(IN5,OUTPUT);
pinMode(IN6,OUTPUT);
pinMode(IN7,OUTPUT);
pinMode(IN8,OUTPUT);

}

struct datapack {
  float x1Val;
  float y1Val;
  float x2Val;
  float y2Val;
  float potVal;
  bool toggle1Val; // bool is a DataType , which it  either stores 0 or 1
  bool toggle2Val;
  bool button1Val;
  bool button2Val;
  bool button3Val;
  bool button4Val;
};

datapack data;

void loop()
{
  /*while(radio.available()) {
  radio.read(&data , sizeof(data));
}
*/

  if(radio.available()) {
    radio.read(&data , sizeof(data)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if(currentTime - lastReceiveTime > 1000) {  // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
    Serial.println("Connection Lost");
  }

  if(data.toggle1Val==1) {
    digitalWrite(buzzPin , HIGH);
    Serial.println("Horn!!!");
  }
  if(data.toggle1Val==0){
    digitalWrite(buzzPin , LOW);
  }

  if(data.x1Val==125){
    PWM1=PWM2=0;
    analogWrite(ENA1,0);
    analogWrite(ENB1,0);
    analogWrite(ENA2,0);
    analogWrite(ENB2,0);
    Serial.println("Car Stopped");
    //CAR IS STOPPED
  }
  if(data.x1Val>125 && (data.y2Val==127||data.y2Val==128)  ) {
    PWM1 = 255.*(data.x1Val-125.)/130.;
    PWM2 = 255.*(data.x1Val-125.)/130.;

    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    analogWrite(ENA1 , PWM1);

    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    analogWrite(ENB1 , PWM1);

    digitalWrite(IN5,HIGH);
    digitalWrite(IN6,LOW);
    analogWrite(ENA2 , PWM2);

    digitalWrite(IN7,HIGH);
    digitalWrite(IN8,LOW);
    analogWrite(ENB2 , PWM2);

    Serial.println("forward");
    //Car moves Forward

   }
  if(data.x1Val<125 && (data.y2Val==127||data.y2Val==128) ) {
    PWM1 = (-255./125)*(data.x1Val-125);
    PWM2 = (-255./125)*(data.x1Val-125);

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    analogWrite(ENA1 , PWM1);

    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(ENB1 , PWM1);

    digitalWrite(IN5,LOW);
    digitalWrite(IN6,HIGH);
    analogWrite(ENA2 , PWM2);

    digitalWrite(IN7,LOW);
    digitalWrite(IN8,HIGH);
    analogWrite(ENB2 , PWM2);

    Serial.println("backward");
    //car Moves Backward


  }

 if(data.x1Val>125 && data.y2Val<127)
  {
    PWM1 = 255.*(data.x1Val-125.)/130.;
    PWM2 = 255.*(data.x1Val-125.)/130.;

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    analogWrite(ENA1 , 0);

    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    analogWrite(ENB1 , PWM1);

    digitalWrite(IN5,HIGH);
    digitalWrite(IN6,LOW);
    analogWrite(ENA2 , PWM2);

    digitalWrite(IN7,LOW);
    digitalWrite(IN8,LOW);
    analogWrite(ENB2 , 0);

    Serial.println("Turn Forward Left");
    //To make the Car Turn Forward Left

   } 
  if(data.x1Val>125 && data.y2Val>128)
   {
    PWM1 = 255.*(data.x1Val-125.)/130.;
    PWM2 = 255.*(data.x1Val-125.)/130.;

    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    analogWrite(ENA1 , PWM1);

    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    analogWrite(ENB1 , 0);

    digitalWrite(IN5,LOW);
    digitalWrite(IN6,LOW);
    analogWrite(ENA2 , 0);

    digitalWrite(IN7,HIGH);
    digitalWrite(IN8,LOW);
    analogWrite(ENB2 , PWM2);

    Serial.println("Turn Forward Right");
    //To make the Car Turn Forward Right

   }
  if(data.x1Val<125 && data.y2Val<127){
    PWM1 = (-255./125)*(data.x1Val-125);
    PWM2 = (-255./125)*(data.x1Val-125);

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    analogWrite(ENA1 , 0);

    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(ENB1 , PWM1);

    digitalWrite(IN5,LOW);
    digitalWrite(IN6,HIGH);
    analogWrite(ENA2 , PWM2);

    digitalWrite(IN7,LOW);
    digitalWrite(IN8,LOW);
    analogWrite(ENB2 , 0);

    Serial.println("Turn Backward Left");
    //car Moves Backward Left
  }
  if(data.x1Val<125 && data.y2Val>128){
    PWM1 = (-255./125)*(data.x1Val-125);
    PWM2 = (-255./125)*(data.x1Val-125);

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    analogWrite(ENA1 , PWM1);

    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    analogWrite(ENB1 , 0);

    digitalWrite(IN5,LOW);
    digitalWrite(IN6,LOW);
    analogWrite(ENA2 , 0);

    digitalWrite(IN7,LOW);
    digitalWrite(IN8,HIGH);
    analogWrite(ENB2 , PWM2);

    Serial.println("Turn Backward Right");
    //car Moves Backward Right
  }
Serial.println(PWM1);
Serial.println(PWM2);
}

void resetData() {
  data.x1Val=125;
  data.y1Val=125;
  data.x2Val=127;
  data.y2Val=127;
  data.potVal=0;
  data.toggle1Val=0;
  data.toggle2Val=0;
  data.button1Val=1;
  data.button2Val=1;
  data.button3Val=1;
  data.button4Val=1;

  PWM1=0;
  PWM2=0;
}
