/* A program to display temperature and light data with options to modify outputs, calibrate temperature, and stream serial data
*  Input voltage is assumed 3.3V
*/
#include <LiquidCrystal.h> // Must be included first as os_class.h requires it.
#include "os_class.h" // Must be included before local.h; this is the operative system of structures, and functions to build them.
#include "local.h" // All implementation specific variables, semaphores and functions.
void call_menu(void); // Interrupt function to engage menu mode.
void display_screen(void); // Function to display data when the menu mode is engaged.
void display_temp_light(void); // Function to display data when the menu mode is not engaged.
void setup()
{
  node_indices=(size_t *)calloc(max_node_depth,sizeof(size_t)); // Allocates and zeros a sempahore array for node navigation.
  attachInterrupt(digitalPinToInterrupt(but_menu),call_menu,RISING); // Sets the menu button as a system interrupt: but_menu must be on an interupt pin!
  Serial.begin(9600); // Initializes serial connectivity.
  lcd.begin(16,2); // Initializes the LCD: set to a 16x2 LCD.
  pinMode(but_up,INPUT); // pinMode intitializes the navigation buttons.
  pinMode(but_down,INPUT);
  pinMode(but_enter,INPUT);
  pinMode(but_menu,INPUT);
  lcd.print("Setup complete.");
  delay(1000);
  lcd.clear();
}
void loop()
{  
  display_temp_light(); // Displays temperature and light sensor readings.
  while(menu_state) // Menu system logic.
  {
    lcd.clear();
    display_screen();
    cursor_pos(cursor_state);
    lcd.cursor();
    lcd.blink();
    if(digitalRead(but_up)==HIGH)
    {
      menu_counter=0;
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
  lcd.noBlink(); // Ensures the menu's blinking cursor isn't displayed while sensor data is displayed.
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
  Serial.println(offset);
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
  else if(millivolts_temp)
  {
    lcd.print(temp_reading*3.3);
    lcd.print("mV");
  }  
  lcd.setCursor(0,1);
  lcd.print("   Light:");
  if(footcandle)
  {
    light_reading=(light_reading/1023.0)*3.3;
    lcd.print(light_reading);
    lcd.print("FC");
  }
  else if(millivolts_light)
  {
    light_reading=(light_reading/1023.0)*3300;
    lcd.print(light_reading);
    lcd.print("mV");
  }
  if(stream)
  {
    Serial.print("Temp:");
    Serial.println(temp_reading);
    Serial.print("Light:");
    Serial.println(light_reading);
  }
}
