#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LcdMenu.h>
int lcdColumns = 20;
int lcdRows = 4;
int voltage = 45;
int Temp = 33;
int timeout = 6;
int a;
int b;
int c;
int mainMenuCounter;
int enterFlag = 0;
int upFlag = 0;
int downFlag = 0;
int paramCount = 0;
bool print = false;
unsigned long int enterPressedTime = 0;
#define enterTimeout 50
#define downTimeout 50
#define upTimeout 50
#define enterPin 19
#define upPin 18
#define downPin 23
LiquidCrystal_I2C lcd(0x27, 20, 4);
LcdMenu menu(4, 20);
MAIN_MENU(
  ITEM_BASIC("Cutoff Batt V"),
  ITEM_BASIC("Cutoff Temp"),
  ITEM_BASIC("Relay Timeout")
);

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  attachInterrupt(19, enterPressed, FALLING);
  attachInterrupt(18, upPressed, FALLING);
  attachInterrupt(23, downPressed, FALLING);
  lcd.setCursor(1, 1);
  lcd.print("Power Load Switcher");
  lcd.setCursor(4, 2);
  lcd.print("Press Enter");

  //menu.setupLcdWithMenu(0x27, mainMenu);
}
void loop()
{
  if (enterFlag == 1 and print)
  {
    lcd.clear();
    delay(2000);
    menu.setupLcdWithMenu(0x27, mainMenu);
    lcd.setCursor(15, 0);
    lcd.print(":");
    lcd.setCursor(16, 0);
    lcd.print(voltage);
    lcd.setCursor(19, 0);
    lcd.print("V");
    lcd.setCursor(15, 1);
    lcd.print(":");
    lcd.setCursor(16, 1);
    lcd.print(Temp);
    lcd.setCursor(19, 1);
    lcd.print("C");
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

    print = false;
  }

  if (enterFlag == 2 and print)
  {
    lcd.setCursor(16, 0);
    lcd.blink();
    delay(500);
    lcd.noBlink();
    delay(500);
    while (enterFlag == 2 and downFlag == 1)
    {
      a = voltage--;
      lcd.setCursor(16, 0);
      Serial.print(a);
      lcd.print(a);
      downFlag = 0;
      print = false;
    }
    while (enterFlag == 2 and upFlag == 1)
    {
      a = voltage++;
      lcd.setCursor(16, 0);
      Serial.print(a);
      lcd.print(a);
      upFlag = 0;
      print = false;
    }
    voltage = a;
  }
  if (enterFlag == 3 and print)
  {
    lcd.setCursor(16, 1);
    lcd.blink();
    delay(500);
    lcd.noBlink();
    delay(500);
    while (enterFlag == 3 and downFlag == 1)
    {
      lcd.setCursor(16, 1);
      b = Temp--;
      lcd.print(a);
      downFlag = 0;
      print = false;
    }
    while (enterFlag == 3 and upFlag == 1)
    {
      lcd.setCursor(16, 1);
      b = Temp++;
      lcd.print(a);
      downFlag = 0;
      print = false;
    }
  }
  if (enterFlag == 4 and print)
  {
    lcd.setCursor(16, 2);
    lcd.blink();
    delay(500);
    lcd.noBlink();
    delay(500);
    while (enterFlag == 4 and downFlag == 1)
    {
      lcd.setCursor(16, 2);
      c = timeout--;
      lcd.print(c);
      downFlag = 0;
      print = false;
    }
    while (enterFlag == 4 and upFlag == 1)
    {
      lcd.setCursor(16, 2);
      c = timeout++;
      lcd.print(c);
      downFlag = 0;
      print = false;
    }
  }
}

void enterPressed()
{
  if (millis() - enterPressedTime > enterTimeout)
  {
    enterFlag = enterFlag + 1;
    if (enterFlag > 4)
    {
      enterFlag = 1;
    }
    print = true;
  }
  enterPressedTime = millis();
}
void upPressed()
{ //Check if enterpressed is 0 or 1 or 2
  unsigned long int upPressedTime = 0;
  if (millis() - upPressedTime > upTimeout)
  {
    upFlag = upFlag + 1;
    print = true;
  }
  upPressedTime = millis();
}
void downPressed()
{ //Check if enterpressed is 0 or 1 or 2
  unsigned long int downPressedTime = 0;
  if (millis() - downPressedTime > downTimeout)
  {
    downFlag = downFlag + 1;
    print = true;
  }
  downPressedTime = millis();
}
