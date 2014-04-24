#include <DueTimer.h>

// gyro
int ACCELERATION_X = A0;
int ACCELERATION_Y = A1;
int ACCELERATION_Z = A2;

int xValuePrev = 0;
int yValuePrev = 0;
int zValuePrev = 0;

int xValueCurr = 0;
int yValueCurr = 0;
int zValueCurr = 0;

int dx = 0;
int dy = 0;
int dz = 0;

// photo interrupter
int SENSOR_PHOTO = A3;

int photoValue = 0;
int vibrationValue = 0;
int vibrationCount = 0;

boolean isWalkingPrev = false;
boolean isWalkingCurr = false;

int WALKING_CONSTANT = 35;

int timer = 0;

void checkWalking()
{
  isWalkingPrev = isWalkingCurr;
  
  xValuePrev = xValueCurr;
  yValuePrev = yValueCurr;
  zValuePrev = zValueCurr;
  
  xValueCurr = analogRead(ACCELERATION_X);
  yValueCurr = analogRead(ACCELERATION_Y);
  zValueCurr = analogRead(ACCELERATION_Z);
  
  dx = abs(xValueCurr - xValuePrev);
  dy = abs(yValueCurr - yValuePrev);
  dz = abs(zValueCurr - zValuePrev);
  
  // Serial.println(dx + dy + dz);
  if (dx + dy + dz > WALKING_CONSTANT)
  {
    // Serial.println("true");
    isWalkingCurr = true;
  }
  else
  {
    // Serial.println("false");
    isWalkingCurr = false;
  }
}

void setup()
{
  Serial.begin(9600);
  
  // Timer3.attachInterrupt(checkWalking);
  // Timer3.start(500000);
}

void loop()
{
  timer %= 30;
  
  if (timer == 0)
  {
    checkWalking();
  }
  
  // check the photo interrupter's status
  photoValue = analogRead(SENSOR_PHOTO);
  if ( photoValue > 1000 )
  {
    // bag is open
    // Serial.println("check");
    if (isWalkingCurr == true && isWalkingPrev == false)
    {
      // open walking =)
      // signal to processing by bluetooth
      // Serial.println("****************");
      Serial.write('O');
      // Serial.println("****************");
    }
  }  
}
