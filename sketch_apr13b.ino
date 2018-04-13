#include <LiquidCrystal.h>
#include "local.h"
#include "init.h"

const int tp=A0; // temp probe
const int lp=A2; // light probe
const int d7=13; // d4-d7 are LCD pins
const int d6=12;
const int d5=11;
const int d4=10;
const int enable=9; // enable pin on LCD
const int rs=8; // rs pin on LCD
const int but_up=7; // up down enter back are nav buttons
const int but_down=6;
const int but_enter=5;
const int but_back=4;
const int but_menu=2; // interrupt pin to engage menu
volatile int light_reading=0; // variable to store light sensor reading
volatile int temp_reading=0; // variable to store temperature sensor reading
volatile bool menu_state=0; // menu state semaphore for menu mode via an interrupt pin
volatile int cursor_state=0; // cursor state semaphore; the menu items should have a blinking cursor on them for human readability. Also equals which opt your are on -1
const size_t cycle_length=300; // cycle length in ms for refreshing a LCD so it looks nice, in ms
volatile size_t menu_counter=0; // counter to ensure menu times out in menu_duration ms, and is reset whenever a nav button is pressed
const size_t menu_cycle_length=100; // menu cycle length so it looks nice, in ms
const size_t menu_duration=5000; // menu duration (time until it leaves menu mode) in ms
const size_t menu_maxcount=menu_duration/menu_cycle_length; // trickery to invoker a timer in interrupt mode
void call_menu(void);
void cursor_pos(int i);
void display_screen(void);
void display_temp_light(void);
LiquidCrystal lcd(rs,enable,d4,d5,d6,d7);
void setup()
{
  attachInterrupt(digitalPinToInterrupt(but_menu),call_menu,RISING);
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(but_up,INPUT);
  pinMode(but_down,INPUT);
  pinMode(but_enter,INPUT);
  pinMode(but_menu,INPUT);
  lcd.print("Setup complete.");
  delay(1000);
  lcd.clear();

}
void loop()
{  
  display_temp_light();
  while(menu_state)
  {
    lcd.clear();
    display_screen();
    cursor_pos(cursor_state);
    lcd.cursor();
    lcd.blink();

    if(digitalRead(but_up)==HIGH)
    {
      menu_counter=0;
      Serial.println(cursor_state);
      if(cursor_state<((root->nopts)-1))
      {
        cursor_state++;
        if(cursor_state==2||cursor_state>2&&(cursor_state%4)==2)
        {
          screen_count++;
        }
      }      
      delay(70);
    }
    else if(digitalRead(but_down)==HIGH)
    {
      menu_counter=0;
      Serial.println(cursor_state);
      if(cursor_state>0)
      {
        cursor_state--;
        if(cursor_state==1||cursor_state>2&&(cursor_state%4)==2)
        {
          screen_count--;
        }
      }      
      delay(70);
    }
    else if(digitalRead(but_enter)==HIGH)
    {
      Serial.println(root->node_registry->opt_order[0]);
      if(root->node_registry->opt_order[cursor_state+1]=='1')
      {
        root->node_registry->registered_function[cursor_state+1]->function();
        Serial.println("schlip schloop");
      }
      else if(root->node_registry->opt_order[cursor_state+1]=='0')
      {
        node_depth_up(&root,cursor_state+1);
        node_depth++;
        Serial.println("snip snopp");
      }
      delay(70);
    }

    
    delay(menu_cycle_length);



    
    if(menu_counter>=menu_maxcount)
    {
      menu_counter=0;
      menu_state=!menu_state;
      break;
    }
    menu_counter++;    
  }
  /*if(digitalRead(but_up)==HIGH) // for testing buttons
  {
    Serial.println("but_up");
  }
  if(digitalRead(but_down)==HIGH)
  {
    Serial.println("but_down");
  }
    if(digitalRead(but_enter)==HIGH)
  {
    Serial.println("but_enter");
  }
    if(digitalRead(but_menu)==HIGH)
  {
    Serial.println("but_menu");
  }
    if(digitalRead(but_back)==HIGH)
  {
    Serial.println("but_back");
  }*/
  lcd.noBlink();
  lcd.noCursor();
  delay(cycle_length);
  lcd.clear();
}
void call_menu(void)
{
  menu_state=!menu_state;
  node_depth=0;
  screen_count=0;
  cursor_state=0;
}
void cursor_pos(int i)
{
  i%=4;
  switch(i)
  {
    case 0:
    {
      lcd.setCursor(0,1);
      break;
    }
    case 1:
    {
      lcd.setCursor(8,1);
      break;
    }
    case 2:
    {
      lcd.setCursor(0,0);
      break;
    }
    case 3:
    {
      lcd.setCursor(8,0);
      break;
    }
    default:
    {
      break;
    }
  }
}
void display_screen(void)
{
  lcd.setCursor(0,0);
  lcd.print(root->screen[screen_count]->line[0]);
  lcd.setCursor(0,1);
  lcd.print(root->screen[screen_count]->line[1]);
}
void display_temp_light(void)
{
  light_reading=analogRead(lp);
  temp_reading=analogRead(tp);
  lcd.setCursor(0,0);
  lcd.print("   Temp:");
  lcd.print(temp_reading);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("   Light:");
  lcd.print(light_reading);
  lcd.print("FC");
}


