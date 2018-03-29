#include "local.h"
int main(void)
{
	screen *menu0= makescreen(2,"******MENU******","1:TEMP  2:LIGHT ");
	screen *menu1= makescreen(2,"3:SERIAL CONFIG ","****************");
	screen *temp0=  makescreen(2,"******TEMP******","1:UNITS 2:CALIB ");
	screen *light0= makescreen(2,"******LIGHT*****","1:VOLTS 2:FC    ");
	screen *serial0=makescreen(2,"*****SERIAL*****","1:STREAM 2:STOP ");
	screen *units0=makescreen(2,"******UNITS*****","1:C     2:F     ");
	screen *units1=makescreen(2,"3:K     4:µK    ","****************");
	printf("%s\n",menu0->line[0]);
	printf("%s\n",menu0->line[1]);
	printf("%s\n",menu1->line[0]);
	printf("%s\n",menu1->line[1]);
	printf("%s\n",temp0->line[0]);
	printf("%s\n",temp0->line[1]);
	printf("%s\n",light0->line[0]);
	printf("%s\n",light0->line[1]);
	printf("%s\n",serial0->line[0]);
	printf("%s\n",serial0->line[1]);
	printf("%s\n",units0->line[0]);
	printf("%s\n",units0->line[1]);
	printf("%s\n",units1->line[0]);
	printf("%s\n",units1->line[1]);
	node *units=makenode(bundle_screen(2,units0,units1),NULL,2,0,4,makenodereg(order_opts(4,NULL),0,NULL));
	printf("terminal nodes\n");
	printf("%s\n",units->screen[0]->line[0]);
	printf("%s\n",units->screen[0]->line[1]);
	printf("%s\n",units->screen[1]->line[0]);
	printf("%s\n",units->screen[1]->line[1]);
	node *serial=makenode(&serial0,NULL,1,0,2,makenodereg(order_opts(2,NULL),0,NULL));
	printf("%s\n",serial->screen[0]->line[0]);
	printf("%s\n",serial->screen[0]->line[1]);
	node *light=makenode(&light0,NULL,1,0,2,makenodereg(order_opts(2,NULL),0,NULL));
	printf("%s\n",light->screen[0]->line[0]);
	printf("%s\n",light->screen[0]->line[1]);
	printf("non terminal nodes\n");
	node *temp=makenode(&temp0,bundle_node(1,units),1,1,2,makenodereg(order_opts(2,NULL),0,NULL));
	printf("%s\n",temp->screen[0]->line[0]);
	printf("%s\n",temp->screen[0]->line[1]);
	printf("%s\n",temp->node[0]->screen[0]->line[0]);
	printf("%s\n",temp->node[0]->screen[0]->line[1]);
	printf("%s\n",temp->node[0]->screen[1]->line[0]);
	printf("%s\n",temp->node[0]->screen[1]->line[1]);
	node *menu=makenode(bundle_screen(2,menu0,menu1),bundle_node(3,temp,light,serial),2,3,3,makenodereg(order_opts(3,NULL),0,NULL));
	printf("%s\n",menu->screen[0]->line[0]);
	printf("%s\n",menu->screen[0]->line[1]);
	printf("%s\n",menu->screen[1]->line[0]);
	printf("%s\n",menu->screen[1]->line[1]);
	printf("%s\n",menu->node[0]->screen[0]->line[0]);
	printf("%s\n",menu->node[0]->screen[0]->line[1]);
	printf("%s\n",menu->node[0]->node[0]->screen[0]->line[0]);
	printf("%s\n",menu->node[0]->node[0]->screen[0]->line[1]);
	printf("%s\n",menu->node[0]->node[0]->screen[1]->line[0]);
	printf("%s\n",menu->node[0]->node[0]->screen[1]->line[1]);
	printf("%s\n",menu->node[1]->screen[0]->line[0]);
	printf("%s\n",menu->node[1]->screen[0]->line[1]);
	printf("%s\n",menu->node[2]->screen[0]->line[0]);
	printf("%s\n",menu->node[2]->screen[0]->line[1]);


	return 0;
}
