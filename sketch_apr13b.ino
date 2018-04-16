#include <LiquidCrystal.h>
#include "os_class.h"
#include "local.h"
void call_menu(void); // Interrupt function to engage menu mode.
void cursor_pos(int i); // Function to properly allign the blinking cursor in this specific 16x2 implementation.
void display_screen(void); // Function to display data when the menu mode is engaged.
void display_temp_light(void); // Function to display data when the menu mode is not engaged.
void setup()
{
  node_indices=(size_t *)calloc(max_node_depth,sizeof(size_t));
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
      menu_counter=0;
      if(root->node_registry->opt_order[cursor_state]=='1')
      {
        size_t counter=0;
        for(size_t i=0;i<=cursor_state;i++)
        {
          if(root->node_registry->opt_order[i]=='1')
          {
            counter ++;
          }
        }
        root->node_registry->registered_function[counter-1]->function();
      }
      else if(root->node_registry->opt_order[cursor_state]=='0')
      {
        node_depth_up(&root,cursor_state);
        node_depth++;
        cursor_state=0;
        screen_count=0;
        *(node_indices+node_depth)=cursor_state;
      }
      delay(70);
    }
    else if(digitalRead(but_back)==HIGH)
    {
      menu_counter=0;
      if(node_depth>0)
      {
        
        
        cursor_state=0;
        screen_count=0;
        node_depth--;
        root=menu;
        for(size_t i=0;i<node_depth;i++)
        {
          node_depth_up(&root,i);
        }        
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
  root=menu;
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
  temp_reading=analogRead(tp)+offset;
  lcd.setCursor(0,0);
  lcd.print("   Temp:");
  if(celcius)
  {
    temp_reading=((temp_reading*3.3/1023.0)-0.33)*100.0;
    lcd.print(temp_reading);
    lcd.print("C");
  }
  else if(farenheight)
  {
    temp_reading=((temp_reading*3.3/1023.0)-0.33)*180.0+32.0;
    lcd.print(temp_reading);
    lcd.print("F");
  }
  else if(kelvin)
  {
    temp_reading=((temp_reading*3.3/1023.0)-0.33)*100.0+273.15;
    lcd.print(temp_reading);
    lcd.print("K");
  }
  else if(millikelvin)
  {
    temp_reading=(((temp_reading*3.3/1023.0)-0.33)*100.0+273.15)*1000;
    lcd.print(temp_reading);
    lcd.print("mk");
  }  
  lcd.setCursor(0,1);
  lcd.print("   Light:");
  if(footcandle)
  {
    light_reading=(light_reading/1023.0)*3.3;
    lcd.print(light_reading);
    lcd.print("FC");
  }
  else if(milivolts)
  {
    light_reading=(light_reading/1023.0)*3300;
    lcd.print(light_reading);
    lcd.print("mV");
  }
}
