int SENSOR_VIBRATION = A0;
int SENSOR_PHOTO = A3;

String message = "";
String prev_message = "";

int photoValue = 0;
int sensorValue = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  photoValue = analogRead(SENSOR_PHOTO);
  if ( photoValue > 1000 )
  {
    // bag is open
    message="";
    sensorValue = analogRead(SENSOR_VIBRATION);
    // serialRotationValue = constrain(serialRotationValue,0,100);
  
    message = String(sensorValue);
    Serial.println("vibration : " + message);
  }
  delay(1000);  
}
