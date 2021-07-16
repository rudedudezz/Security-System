#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define password_length 7

//define the correct password
byte number_count = 0;
char my_password[password_length+1];
char correct_password[password_length+1] = "7355608";

//4*4 keypad
const byte ROWS = 4;
const byte COLS = 4;

//sensor pins
const int sensor_A0 = A0;
const int sensor_D0 = 11;

int Laser = 12;
int SensorReading; 

int value_sensor_A0;
bool value_sensor_D0;

bool alarmState = true;

//setup the keypad 
char allKeys[ROWS][COLS] = 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'}, 
  {'*', '0', '#', 'D'}, 
  
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};


Keypad myKeypad = Keypad(makeKeymap(allKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2); //the hex address of the lcd display is 0x27

void setup() 
{
    Serial.begin(9600);
    lcd.backlight();
    lcd.init(); 
    lcd.init(); 
    lcd.init(); 
    
   // lcd.setCursor(0, 0);
    //lcd.print("Enter Password:");
    
    pinMode(sensor_A0, INPUT);//analog signal
    pinMode(sensor_D0, INPUT); //digital signal
    
    pinMode(10, OUTPUT); //buzzer in pin 10
    pinMode(Laser, OUTPUT);//laser in pin 12
}

void loop() 
{
    digitalWrite(Laser, HIGH);
    value_sensor_A0 = analogRead(sensor_A0);
    value_sensor_D0 = digitalRead(sensor_D0);
  
    delay(100);
  
  //make alarm sound when detecting a change of light
  Serial.println(value_sensor_A0);
  
  if ((value_sensor_A0 >= 50) && (alarmState != 0))
  {
    tone(10, 1000); 
    delay(250);  //0.25s     
    noTone(10);   
  }
  
    //get keys from keypad
    char currentKey = myKeypad.getKey();
    
    //set lcd display
    lcd.setCursor(0, 0);
    lcd.print("Enter Password:");
    
    if(currentKey && (currentKey != '#') && (currentKey != '*'))
    {
      /*lcd.setCursor(0, 1);
      lcd.print(currentKey);
      Serial.println(currentKey);*/
      
      //check the entered password
      my_password[number_count] = currentKey;
      Serial.println(currentKey);
      lcd.setCursor(number_count, 1);
      lcd.print(my_password[number_count]);
      number_count++; 
      //strncpy(my_password, my_password, number_count);
      
      Serial.println(number_count);
      Serial.println(my_password);
      Serial.println(correct_password);     
      
      //make sound every time a key has been pressed
      tone(10, 1000); 
      delay(100);  //0.25s     
      noTone(10);   //buzzer in pin 10
      delay(10);       
      
    }
    
    //check the correctness of the current password
    if (number_count == password_length) 
    {
      lcd.clear();
 
      if (strcmp(my_password, correct_password) == 0) 
      {
        lcd.print("Correct");  
        
        //change the states of the alarm
       if(alarmState == 1)
       {
          lcd.setCursor(0, 1);
          lcd.print("Alarm Off"); 
          alarmState = 0;
       }
       
       else if(alarmState == 0)
       {
          lcd.setCursor(0, 1);
          lcd.print("Alarm On"); 
          alarmState = 1;
       }
        
        delay(5000);
        clearPassword();
      }
      else 
      {
        lcd.print("Incorrect");
        delay(5000);
        clearPassword();
      }
    }
    
    //'*' for clear 
    else if(currentKey && (currentKey == '*'))
    {
      lcd.setCursor(0, 0); 
      clearPassword();
    }
}

//clear the current password from display and ready for entering a new one
void clearPassword()
{
  lcd.clear();
  while(number_count != 0)
  {
    my_password[number_count-1] = 0;
    number_count--;
  }
}
