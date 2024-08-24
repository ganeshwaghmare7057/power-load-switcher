 float temp, voltage;
void setup() {
    Serial.begin(115200);
    pinMode(5,OUTPUT); 
}

void loop() {
int sensorValue = analogRead(A0);
  voltage=sensorValue*(3.3/4095.0);


  temp = analogRead(A3)*3.3/4095;
  temp = temp-0.5;
  temp = temp/0.01;
Serial.println(voltage);
Serial.println(temp);
  delay(1000);

if (voltage>1.64)
{
  if( temp>21)
  {
        printf("Relay ON");
        Serial.println("RIGHT");
        digitalWrite(5,HIGH);
        printf("\n");
  }
  else 
  {
      printf("Relay OFF"); 
      digitalWrite(5,LOW);
      printf("\n");
  }
}
else 
{
  printf("Relay OFF"); 
  Serial.println("WRONG");
      digitalWrite(5,LOW);
      printf("\n");
} 
delay (1000); 
}
