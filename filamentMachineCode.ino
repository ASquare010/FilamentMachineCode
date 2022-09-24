//voltage 3.3=brown ,5.0=red,grownd = Black

#include <AccelStepper.h>

//RPM

int nema16Rpm =2400;
int nema17Rpm = 1;
int nema23Rpm = 225;


//Nema 16 pins White
#define clk16 6
#define dir16 7
#define constTime  3000

//thinessGage pins
#define CLOCK_PIN 12 //Green
#define DATA_PIN  11 //~Blue

//stepper steps
#define STEPS 200

//Nema 17 pins Grey
#define clk17 3
#define dir17 2
bool changDir = false;


//Nema 23 pins Purple
#define clk23 5 
#define dir23 4


//Global Data varibles
unsigned long tmpTime , checkTime,oldTime;
int sign; 
long value; 
float result,upper =1.77, lower=1.73;


AccelStepper nema16(AccelStepper::DRIVER, clk16,dir16);
AccelStepper nema17(AccelStepper::DRIVER, clk17,dir17);
AccelStepper nema23(AccelStepper::DRIVER, clk23,dir23);




void setup() {
  Serial.begin(9600);
  //gage setup
   pinMode(CLOCK_PIN, INPUT);
   pinMode(DATA_PIN, INPUT);
   oldTime = millis();
  
  // Configure each stepper
  nema16.setMaxSpeed(nema16Rpm);
  nema16.setAcceleration(300.0);
  nema16.moveTo(1000000);

  nema17.setMaxSpeed(nema17Rpm);
  nema17.setAcceleration(300.0);
  
  nema23.setMaxSpeed(nema23Rpm);
  nema23.setAcceleration(300.0);
  nema23.moveTo(1000000);
}

void loop() {
 //readData();
//   nema16.run();
//   nema16.moveTo(1000000);
//   
//   invertDirNema17();

   nema23.run();
   nema23.moveTo(1000000);
}

void readData()
{
  checkTime = millis();
  if((checkTime - oldTime) >= constTime)
  {
    getThikness();
    compaiorData();
    oldTime = checkTime;
  }//else{}
}
void compaiorData()
{
  if(result>=0){
      //1.73 < 1.77
  if (result < upper )
  {
    nema16Rpm = nema16Rpm - 2;
    nema16.setMaxSpeed(nema16Rpm);
    nema16.moveTo(1000000);
  }
    //1.82 > 1.62
  else if(result > lower )
  {
    nema16Rpm = nema16Rpm + 2;
    nema16.setMaxSpeed(nema16Rpm);
    nema16.moveTo(1000000);
 
  }
  else{}
      }
}
void getThikness()
{
  while(digitalRead(CLOCK_PIN)==LOW) {}
  tmpTime=micros();
  while(digitalRead(CLOCK_PIN)==HIGH) {}
  if((micros()-tmpTime)<500) return;
  readCaliper(); 
}
void readCaliper()
{
  sign=1;
  value=0;
  for(int i=0;i<24;i++) {
    while(digitalRead(CLOCK_PIN)==LOW) {}
    while(digitalRead(CLOCK_PIN)==HIGH) {}
    if(digitalRead(DATA_PIN)==HIGH) {
      if(i<20) value|=(1<<i);
      if(i==20) sign=-1;
     }
  }
    result=(value*sign)/100.0;
  
  Serial.print(result); Serial.print(" mm\n");
  Serial.print(nema16Rpm); Serial.print(" Rpm\n");
}
void invertDirNema17()
{
  if(!nema17.isRunning())
  {
    if(changDir)
   {
     nema17.run();
     nema17.moveTo(60);
     changDir = !changDir;
   }
   else
    {
    nema17.moveTo(-60);
    nema17.run();
    changDir = !changDir;
    }
  }
} 
