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
int a;
int b;
int c;
int enterFlag = 0;
int upFlag = 0;
int downFlag = 0;
int mainMenuCounter = 0;
bool print = false;
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  attachInterrupt(19, enterPressed, FALLING);
  attachInterrupt(18, upPressed, FALLING);
  attachInterrupt(23, downPressed, FALLING);
  while (!enterFlag == 1)
  {
    lcd.setCursor(1, 1);
    lcd.print("Power Load Switcher");
    lcd.setCursor(4, 2);
    lcd.print("Press Enter");
  }
  lcd.clear();
      updateMenu();

}

void loop() {
  if (enterFlag == 1 and print) {
    menu++;
    updateMenu();
    delay(100);
    print = false;
  }
  if (enterFlag == 2 and print)
  {
    menu++;
    delay(100);
    while (enterFlag == 2 and upFlag == 1 and print)
    {
      a = voltage++;
      lcd.setCursor(16, 0);
      Serial.print(a);
      lcd.print(a);
      upFlag = 0;
    }
    while (enterFlag == 2 and downFlag == 1 and print)
    {
      a = voltage--;
      lcd.setCursor(16, 0);
      Serial.print(a);
      lcd.print(a);
      downFlag = 0;
    }
    print = false;
  }
  if (enterFlag == 3 and print)
  {
    menu++;
    delay(100);
    while (enterFlag == 3 and downFlag == 1 and print)
    {
      lcd.setCursor(16, 1);
      b = Temp--;
      lcd.print(b);
      downFlag = 0;
    }
    while (enterFlag == 3 and upFlag == 1 and print)
    {
      lcd.setCursor(16, 1);
      b = Temp++;
      lcd.print(b);
      downFlag = 0;
    }
    print = false;
  }
  if (enterFlag == 4 and print)
  {
    menu++;
    delay(100);
    while (enterFlag == 4 and downFlag == 1 and print)
    {
      lcd.setCursor(16, 2);
      c = timeout--;
      lcd.print(c);
      downFlag = 0;
    }
    while (enterFlag == 4 and upFlag == 1 and print)
    {
      lcd.setCursor(16, 2);
      c = timeout++;
      lcd.print(c);
      downFlag = 0;
    }
    print = false;
  }
  if (enterFlag == 5 and print)
  {
    menu++;
    delay(100);
    print = false;
  }
}

void updateMenu() {
  switch (menu) {
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
void enterPressed()
{
  unsigned long int enterPressedTime = 0;
  if (millis() - enterPressedTime > enterTimeout)
  {
    enterFlag = enterFlag + 1;
    if (enterFlag > 5)
    {
      enterFlag = 1;
    }
    print = true;
  }
  enterPressedTime = millis();
}
void upPressed()
{
  unsigned long int upPressedTime = 0;
  if (millis() - upPressedTime > upTimeout)
  {
    upFlag = upFlag + 1;
  }
  upPressedTime=millis();
  print = true;
}
void downPressed()
{
  unsigned long int downPressedTime = 0;
  if (millis() - downPressedTime > downTimeout)
  {
    downFlag = downFlag + 1;
  }
  downPressedTime = millis();
  print = true;
}
