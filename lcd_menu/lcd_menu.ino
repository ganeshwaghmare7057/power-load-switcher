#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define enterPin 19
#define upPin 23
#define downPin 18
#define enterTimeout 120
#define downTimeout 120
#define upTimeout 120
int menu = 0;
int voltage = 45;
int Temp = 33;
int timeout = 6;
int add = 0;
int sub = 0;
int mainMenuCounter = 0;
bool print = true;
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  lcd.setCursor(1, 1);
  lcd.print("Power Load Switcher");
  lcd.setCursor(4, 2);
  lcd.print("Press Enter");
  while (digitalRead(enterPin) == HIGH);
  lcd.clear();
  updateMenu();

}

void loop() {
  if (digitalRead(enterPin) == LOW) {
    delay(100);
    if (digitalRead(enterPin) == LOW) {
      menu++;
      updateMenu();
      delay(500);
      while (!digitalRead(enterPin));
    }
  }
}

void updateMenu() {
  switch (menu) {
    Serial.print("Menu value : ");
    Serial.println(menu);
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Cutoff Voltage");
      lcd.setCursor(15, 0);
      lcd.print(":");
      lcd.setCursor(16, 0);
      lcd.print(voltage);
      lcd.setCursor(19, 0);
      lcd.print("V");
      lcd.setCursor(0, 1);
      lcd.print("Cutoff Temp");
      lcd.setCursor(15, 1);
      lcd.print(":");
      lcd.setCursor(16, 1);
      lcd.print(Temp);
      lcd.setCursor(19, 1);
      lcd.print("C");
      lcd.setCursor(0, 2);
      lcd.print("Relay Timeout");
      lcd.setCursor(15, 2);
      lcd.print(":");
      lcd.setCursor(16, 2);
      lcd.print(timeout);
      lcd.setCursor(18, 2);
      lcd.print("Hr");
      lcd.setCursor(4, 3);
      lcd.print("Save Changes");
      lcd.setCursor(18, 3);
      lcd.print("  ");
      print = true;
    case 2:
      lcd.setCursor(16, 0);
      lcd.blink();
      delay(500);
      lcd.noBlink();
      delay(500);
      print = true;
    case 3:
      lcd.setCursor(16, 1);
      lcd.blink();
      delay(500);
      lcd.noBlink();
      delay(500);
      print = true;
    case 4:
      lcd.setCursor(16, 2);
      lcd.blink();
      delay(500);
      lcd.noBlink();
      delay(500);
      print = true;
    case 5:
      lcd.setCursor(3, 3);
      lcd.blink();
      delay(500);
      lcd.noBlink();
      delay(500);
      print = true;
  }
}
void executeAddition() {
  switch (add)
  {
    case 1:
      action1();
      break;
    case 2:
      action3();
      break;
    case 3:
      action5();
      break;
  }
}
void executeSubtraction()
{
  switch (sub)
  {
    case 1:
      action2();
      break;
    case 2:
      action4();
      break;
    case 3:
      action6();
      break;
  }
}
void action1()
{
  voltage++;
  lcd.setCursor(16, 0);
  lcd.print(voltage);
}
void action2()
{
  voltage--;
  lcd.setCursor(16, 0);
  lcd.print(voltage);
}
void action3()
{
  Temp++;
  lcd.setCursor(16, 1);
  lcd.print(Temp);
}
void action4()
{
  Temp--;
  lcd.setCursor(16, 1);
  lcd.print(Temp);
}
void action5()
{
  timeout++;
  lcd.setCursor(16, 1);
  lcd.print(timeout);
}
void action6()
{
  timeout--;
  lcd.setCursor(16, 1);
  lcd.print(timeout);
}
