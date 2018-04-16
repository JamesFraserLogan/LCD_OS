const int tp=A0; // temp probe
const int lp=A2; // light probe
const int d7=13; // d4-d7 are LCD pins
const int d6=12;
const int d5=11;
const int d4=10;
const int enable=9; // enable pin on LCD
const int rs=8; // rs pin on LCD
const int but_up=7; // Up, down, enter and back are menu navigation buttons.
const int but_down=6;
const int but_enter=5;
const int but_back=4;
const int but_menu=2; // Interrupt pin to engage menu.
volatile int light_reading=0; // Variable to store light sensor reading.
volatile int temp_reading=0; // Variable to store temperature sensor reading.
volatile bool menu_state=0; // Menu state semaphore for menu mode via an interrupt pin.
volatile int cursor_state=0; // Cursor state semaphore; the menu items should have a blinking cursor on them for human readability. Also equals which opt your are on -1.
const size_t cycle_length=500; // Cycle length in ms for refreshing a LCD so it looks nice, in ms.
volatile size_t menu_counter=0; // Counter to ensure menu times out in menu_duration ms, and is reset whenever a nav button is pressed.
const size_t menu_cycle_length=250; // Menu cycle length so it looks nice, in ms.
const size_t menu_duration=5000; // Menu duration (time until it leaves menu mode) in ms.
const size_t menu_maxcount=menu_duration/menu_cycle_length; // Menu counter max count. When the count reaches this number the menu state is flipped.
LiquidCrystal lcd(rs,enable,d4,d5,d6,d7);
volatile bool celcius=1; // Semaphore to indicate what temperature unit to display.
volatile bool farenheight=0; // Semaphore to indicate what temperature unit to display.
volatile bool kelvin=0; // Semaphore to indicate what temperature unit to display.
volatile bool millikelvin=0; // Semaphore to indicate what temperature unit to display.
volatile bool footcandle=0; // Semaphore to indicate what light unit to display.
volatile bool milivolts=1; // Semaphore to indicate what light unit to display.
volatile float offset=0.; // Offset used to calibrate temperature probe to the triple point of water.
volatile size_t node_depth=2; // RootNode->node[a1]->node->[a2]->....->node[an] is the topology this counter follows. A node depth of 0 denotes the root node.
volatile size_t max_node_depth=0; // Be careful this is set correctly, it's used to determine how much memory is alloacated to the node_indices array. This would be n in the above example.
volatile size_t screen_count=0; // This is "i" in: RootNode->node[a1]->...->node[an]->screen[i] ; that is, this is which screen you are on in a particular node.
volatile size_t line_count=0; // This is "j" in: RootNode->node[a1]->...->node[an]->screen[i]->line[j] ; that is, this is which line you are on in a particular screen in a particular node.
volatile size_t *node_indices; // This is the array of node index coefficients {a1,a2,a3,...,an} in the above example.
volatile size_t opt_counter=0; // This is which option you are on in any particular node. Registered functions can be attached to options.
void teststream(void);
void teststop(void);
void testfc(void);
void testmilivolts(void);
void testcalib(void);
void testc(void);
void testf(void);
void testk(void);
void testmk(void);
void testcalib (void)
{
  Serial.println("testcalib");
}
void testmilivolts(void)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FUNCTION PRESSED");
  lcd.setCursor(0,1);
  lcd.print("****LIGHT:mV****");
  footcandle=0;
  milivolts=1;
  delay(1500);
  lcd.clear();
}
void testfc(void)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FUNCTION PRESSED");
  lcd.setCursor(0,1);
  lcd.print("****LIGHT:FC****");
  footcandle=1;
  milivolts=0;
  delay(1500);
  lcd.clear();
}
void teststream(void)
{
  Serial.println("teststream");
}
void teststop(void)
{
  Serial.println("teststop");
}
void testc(void)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FUNCTION PRESSED");
  lcd.setCursor(0,1);
  lcd.print("****UNITS:C*****");
  celcius=1;
  farenheight=0;
  kelvin=0;
  millikelvin=0;
  delay(1500);
  lcd.clear();
}
void testf(void)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FUNCTION PRESSED");
  lcd.setCursor(0,1);
  lcd.print("****UNITS:F*****");
  celcius=0;
  farenheight=1;
  kelvin=0;
  millikelvin=0;
  delay(1500);
  lcd.clear();
}
void testk(void)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FUNCTION PRESSED");
  lcd.setCursor(0,1);
  lcd.print("****UNITS:K*****");
  celcius=0;
  farenheight=0;
  kelvin=1;
  millikelvin=0;
  delay(1500);
  lcd.clear();
}
void testmk(void)
{  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FUNCTION PRESSED");
  lcd.setCursor(0,1);
  lcd.print("****UNITS:mK****");
  celcius=0;
  farenheight=0;
  kelvin=0;
  millikelvin=01;
  delay(1500);
  lcd.clear();
}
screen *menu0= makescreen(2,"******MENU******","1:TEMP  2:LIGHT ");
screen *menu1= makescreen(2,"3:SERIAL CONFIG ","****************");
screen *temp0=  makescreen(2,"******TEMP******","1:UNITS 2:CALIB ");
screen *light0= makescreen(2,"******LIGHT*****","1:mV    2:FC    ");
screen *serial0=makescreen(2,"**SERIAL STREAM*","1:START 2:STOP  ");
screen *units0=makescreen(2,"******UNITS*****","1:C     2:F     ");
screen *units1=makescreen(2,"3:K     4:mK    ","****************");
registered_function *regtestcalib=register_function(testcalib);
registered_function *regtestmilivolts=register_function(testmilivolts);
registered_function *regtestfc=register_function(testfc);
registered_function *regteststream=register_function(teststream);
registered_function *regteststop=register_function(teststop);
registered_function *regtestc=register_function(testc);
registered_function *regtestf=register_function(testf);
registered_function *regtestk=register_function(testk);
registered_function *regtestmk=register_function(testmk);
node *units=makenode(bundle_screen(2,units0,units1),NULL,2,0,4,makenodereg(order_opts(4,"1111"),4,bundle_registered_functions(4,regtestc,regtestf,regtestk,regtestmk)));
node *serial=makenode(&serial0,NULL,1,0,2,makenodereg(order_opts(2,"11"),2,bundle_registered_functions(2,regteststream,regteststop)));
node *light=makenode(&light0,NULL,1,0,2,makenodereg(order_opts(2,"11"),2,bundle_registered_functions(2,regtestmilivolts,regtestfc)));
node *temp=makenode(&temp0,bundle_node(1,units),1,1,2,makenodereg(order_opts(2,"01"),1,bundle_registered_functions(1,regtestcalib)));
node *menu=makenode(bundle_screen(2,menu0,menu1),bundle_node(3,temp,light,serial),2,3,3,makenodereg(order_opts(3,NULL),0,NULL));
node *root=menu;
