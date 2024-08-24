#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <otadrive_esp.h>
#define tempSensor 13
#define enterPin 25
#define upPin 23
#define downPin 26
#define enterTimeout 120
#define downTimeout 120
#define upTimeout 120
#define mainsPin 2
#define battPin 19
#define relayPin 27
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define FILESYS SPIFFS
#define APIKEY "c3d5dc50-8007-499f-a4fe-89ff966a3df7"

float mainVoltage;
float mainsVolt;
int menu = 0;
int voltage = 45;
int Temp = 33;
int timeout = 6;
int a;
int b;
int c;
int add = 0;
int sub = 0;
int mainMenuCounter = 0;
float tempreture;
float batt;
float mainsArray[100];
float mainsBig = 0;
unsigned long int enterPressedTime = 0;
unsigned long int upPressedTime = 0;
unsigned long int downPressedTime = 0;
unsigned long int screenTime = 0;
unsigned long screenTimeout = 5000;
unsigned long otaTimeout = 5000;
unsigned long int previousTime = 0;
bool print = true;
int y = 0;
bool mainsCheck = false;
int x = 0;
String ssid_update = "Icon_PLS";
String password_update = "PowerLoadSwitcher";
unsigned long time_now;
unsigned long time_prev;

void OTA_init() {
  lcd.clear();
  lcd.setCursor(6, 1);
  lcd.print("UPDATING");
  lcd.setCursor(6, 2);
  lcd.print("FIRMWARE");

  // Connect to WiFi network
  WiFi.begin(ssid_update, password_update);
  Serial.println("");
  // Wait for connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("WiFi not connected");
    // Serial.setCursor(0, 1);
    Serial.print("Reconnecting...");
    for (int i = 0; i < 3; i++) {
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid_update, password_update);
      delay(5000);
      if (WiFi.status() == WL_CONNECTED) {
        lcd.clear();
        lcd.setCursor(8, 1);
        lcd.print("WIFI");
        lcd.setCursor(6, 2);
        lcd.print("CONNECTED");
        Serial.print("WiFi Connected");
        break;
      }
    }
    if (WiFi.status() != WL_CONNECTED) {
      lcd.clear();
      lcd.setCursor(8, 1);
      lcd.print("WIFI");
      lcd.setCursor(2, 2);
      lcd.print("NOT CONNECTED");
      Serial.print("WiFi Connected");
      Serial.print("WiFI Not Connnecting");
      Serial.print("Try connecting to WiFi");
    }
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid_update);

  // initialize FileSystem
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  Serial.println("File system Mounted");
  OTADRIVE.setInfo(APIKEY, "0.0");
  OTADRIVE.onUpdateFirmwareProgress(onUpdateProgress);

  if (WiFi.status() == WL_CONNECTED) {
    // retrive firmware info from OTAdrive server
    updateInfo inf = OTADRIVE.updateFirmwareInfo();
    Serial.printf("\nfirmware info: %s ,%dBytes\n%s\n",
                  inf.version.c_str(), inf.size, inf.available ? "New version available" : "No newer version");
    // update firmware if newer available
    if (inf.available) {
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("New Version");
      lcd.setCursor(5, 2);
      lcd.print("Available");
      // Serial.setCursor(0, 2);
      Serial.print("New version available");
      OTADRIVE.updateFirmware();
    } else {
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print("No newer Version");
      lcd.setCursor(5, 2);
      lcd.print("Available");
      // Serial.setCursor(0, 2);
      Serial.print("No newer version");
    }
    // sync local files with OTAdrive server
    OTADRIVE.syncResources();
  }
}

void onUpdateProgress(int progress, int totalt) {
  static int last = 0;
  int progressPercent = (100 * progress) / totalt;
  Serial.print("*");
  if (last != progressPercent && progressPercent % 10 == 0) {
    //print every 10%
    Serial.printf("%d", progressPercent);
  }
  last = progressPercent;
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname, "r");
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(downPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(enterPin, INPUT_PULLUP);
  pinMode(tempSensor, INPUT);
  pinMode(mainsPin, INPUT);
  pinMode(battPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  lcd.setCursor(1, 1);
  lcd.print("Power Load Switcher");
  lcd.setCursor(4, 2);
  lcd.print("Press Enter");
  while (digitalRead(enterPin) == HIGH)
    ;
  lcd.clear();
  //updateMenu();
}

void loop() {
  //OTA
  time_now = millis();
  while (digitalRead(downPin) == 0 && digitalRead(upPin) == 0 && (time_now - time_prev) >= otaTimeout) {
    OTA_init();
    // time_prev = time_now;
  }
  if (digitalRead(enterPin) == LOW) {
    delay(100);
    if (digitalRead(enterPin) == LOW) {
      if (millis() - enterPressedTime > enterTimeout) {
        menu++;
        updateMenu();
        mainMenuCounter = mainMenuCounter + 1;
        delay(100);
        //      while (!digitalRead(enterPin));
      }
      enterPressedTime = millis();
    }
  }
  if (digitalRead(downPin) == LOW) {
    delay(100);
    if (digitalRead(downPin) == LOW) {
      if (mainMenuCounter == 2 and print) {
        sub = 1;
        executeSubtraction();
      }
      if (mainMenuCounter == 3 and print) {
        sub = 2;
        executeSubtraction();
      }
      if (mainMenuCounter == 4 and print) {
        sub = 3;
        executeSubtraction();
        Serial.print(timeout);
      }
    }
  }
  if (digitalRead(upPin) == LOW) {
    delay(100);
    if (digitalRead(upPin) == LOW) {
      if (mainMenuCounter == 2 and print) {
        add = 1;
        executeAddition();
      }
      if (mainMenuCounter == 3 and print) {
        add = 2;
        executeAddition();
      }
      if (mainMenuCounter == 4 and print) {
        add = 3;
        executeAddition();
        Serial.print(timeout);
      }
    }
  }
  if (mainMenuCounter == 6 and print) {
    x++;
    if (x == 1) {
      saveChange();
    }
    unsigned long currentTime = millis();
    if (currentTime - screenTime >= screenTimeout) {
      // Serial.print("Hello");
      mains();
      battery();
      tempValue();
      thirdScreen();
      if (mainsCheck == 1) {
        mainsCheck = 0;
        digitalWrite(relayPin, HIGH);
        //        Serial.println("Mains Still available");
      }
      else {
        Serial.println("Mains not available");
        if (batt <= voltage or Temp >= tempreture) {
          unsigned long int Time = millis();
          int interval;
          interval = 3600000 * timeout;
          Serial.print("interval:");
          Serial.println(interval);
          digitalWrite(relayPin, LOW);
          Serial.println("Relay ON");
          unsigned long int timer = millis();
          int prevRelaytime = 0;
//          Serial.print("Waiting for 5 Mins");
//          while (timer - prevRelaytime >= 300000);
//          Serial.println("5 mins over");
          Serial.print("Time");
          Serial.println(Time);
          Serial.print("previousTime:");
          Serial.println(previousTime);
          if (Time - previousTime >= interval) {
            digitalWrite(relayPin, HIGH);
            Serial.println("Timeout over Switchoff Relay");
            previousTime = Time;
            Serial.print("previousTime inside loop:");
            Serial.println(previousTime);
            delay(2000);
          }
        }
      }
      screenTime = currentTime;
    }
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
      break;
    case 2:
      lcd.setCursor(16, 0);
      lcd.blink();
      delay(500);
      print = true;
      break;
    case 3:
      lcd.setCursor(16, 1);
      lcd.blink();
      delay(500);
      print = true;
      break;
    case 4:
      lcd.setCursor(16, 2);
      lcd.blink();
      delay(500);
      print = true;
      break;
    case 5:
      lcd.setCursor(3, 3);
      lcd.blink();
      delay(500);
      print = true;
      break;
  }
}

void executeAddition() {
  switch (add) {
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

void executeSubtraction() {
  switch (sub) {
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

void action1() {
  voltage++;
  a = voltage;
  lcd.setCursor(16, 0);
  lcd.print(voltage);
}

void action2() {
  voltage--;
  a = voltage;
  lcd.setCursor(16, 0);
  lcd.print(voltage);
}

void action3() {
  Temp++;
  b = Temp;
  lcd.setCursor(16, 1);
  lcd.print(Temp);
}

void action4() {
  Temp--;
  b = Temp;
  lcd.setCursor(16, 1);
  lcd.print(Temp);
}

void action5() {
  timeout++;
  c = timeout;
  lcd.setCursor(16, 2);
  lcd.print(timeout);
}
void action6() {
  timeout--;
  c = timeout;
  lcd.setCursor(16, 2);
  lcd.print(timeout);
}

void saveChange() {
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("Changes");
  lcd.setCursor(8, 2);
  lcd.print("Saved");
  delay(2000);
  print = false;
}

void thirdScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Main Voltage");
  lcd.setCursor(13, 0);
  lcd.print(":");
  delay(500);
  lcd.setCursor(14, 0);
  lcd.print(mainsBig, 1);
  lcd.setCursor(19, 0);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Batt Voltage");
  lcd.noBlink();
  delay(500);
  lcd.setCursor(13, 1);
  lcd.print(":");
  lcd.setCursor(14, 1);
  lcd.print(batt, 2);
  lcd.setCursor(19, 1);
  lcd.print("V");
  lcd.setCursor(0, 2);
  lcd.print("Current Temp");
  delay(500);
  lcd.setCursor(13, 2);
  lcd.print(":");
  lcd.setCursor(14, 2);
  lcd.print(tempreture, 1);
  lcd.setCursor(18, 2);
  lcd.print(char(223));
  lcd.setCursor(19, 2);
  lcd.print("C");
  print = true;
}

void tempValue() {
  int sensorValue = analogRead(tempSensor);
  float voltage = sensorValue * 3.3;
  tempreture = (voltage - 0.5) / 100;
  delay(1000);
}

void mains() {
  // int i = 0;
  for (int i = 0; i < 100; i++) {
    //    Serial.print("Analog Value");
    //    Serial.println(analogRead(mainsPin));
    mainsArray[i] = (analogRead(mainsPin) * 3.3) / 4095.0;
    mainsArray[i] = mainsArray[i] * 100;
    //    Serial.print("Array");
    //    Serial.println(mainsArray[i]);
  }
  // int voltageMain;
  //    mainVoltage = (analogRead(mainsPin) * 3.3) / 4095;
  //    mainsVolt = mainVoltage * 100;
  float max = 200;
  for (int i = 0; i < 100; i++)
  {
    if (mainsArray[i] >= max)
    {
      //      Serial.println("Inside MAX");
      max = mainsArray[i];
      mainsBig = max;
      //      Serial.print("MAX value:");
      //      Serial.println(max);
    }
  }

  for (int i = 0; i < 100; i++) {
    if (mainsArray[i] > 200) {
      mainsCheck = 1;
      break;
    }
  }
  if (mainsCheck == 0)
  {
    mainsBig = 0;
    //  Serial.println("No mains so value is 0");
  }
  delay(1000);
}
void battery() {
  int battVoltage = analogRead(battPin);
  float battVolt = (battVoltage * 3.3) / 4095;
  batt = (battVolt * 60) / 3.3;
  delay(1000);
}
