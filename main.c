#include "local.h"

#define MAC(i) char *line##i = (char *)malloc(17);\
if(line##i == NULL){\
	printf("malloc error");}\
_itoa(i,temp,10);\
		j=0;\
		while(*(temp+j)!='\0')\
		{\
			*(line##i+j)=*(temp+j);\
			j++;\
		}\
		for(;j<16;j++)\
		{\
			*(line##i+j)='a';\
		}\
		*(line##i+16)='\0';
		


int main(void)
{
	screen *menu0= makescreen(2,"******MENU******","1:TEMP  2:LIGHT ");
	screen *menu1= makescreen(2,"3:SERIAL CONFIG ","****************");
	screen *temp0=  makescreen(2,"******TEMP******","1:UNITS 2:CALIB ");
	screen *light0= makescreen(2,"******LIGHT*****","1:VOLTS 2:FC    ");
	screen *serial0=makescreen(2,"*****SERIAL*****","1:STREAM 2:STOP ");
	screen *units0=makescreen(2,"******UNITS*****","1:C     2:F     ");
	screen *units1=makescreen(2,"3:K     4:µK    ","****************");
	registered_function *regtestcalib=register_function(testcalib);
	registered_function *regtestvolts=register_function(testvolts);
	registered_function *regtestfc=register_function(testfc);
	registered_function *regteststream=register_function(teststream);
	registered_function *regteststop=register_function(teststop);
	registered_function *regtestc=register_function(testc);
	registered_function *regtestf=register_function(testf);
	registered_function *regtestk=register_function(testk);
	registered_function *regtestuk=register_function(testuk);


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
	node *units=makenode(bundle_screen(2,units0,units1),NULL,2,0,4,makenodereg(order_opts(4,"1111"),4,bundle_registered_functions(4,regtestc,regtestf,regtestk,regtestuk)));




	printf("terminal nodes\n");
	printf("%s\n",units->screen[0]->line[0]);
	printf("%s\n",units->screen[0]->line[1]);
	printf("%s\n",units->screen[1]->line[0]);
	printf("%s\n",units->screen[1]->line[1]);
	node *serial=makenode(&serial0,NULL,1,0,2,makenodereg(order_opts(2,"11"),2,bundle_registered_functions(2,regteststream,regteststop)));
	printf("%s\n",serial->screen[0]->line[0]);
	printf("%s\n",serial->screen[0]->line[1]);
	node *light=makenode(&light0,NULL,1,0,2,makenodereg(order_opts(2,"11"),2,bundle_registered_functions(2,regtestvolts,regtestfc)));
	printf("%s\n",light->screen[0]->line[0]);
	printf("%s\n",light->screen[0]->line[1]);
	printf("non terminal nodes\n");
	node *temp=makenode(&temp0,bundle_node(1,units),1,1,2,makenodereg(order_opts(2,"01"),1,bundle_registered_functions(1,regtestcalib)));
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

	units->node_registry->registered_function[0]->function();
	units->node_registry->registered_function[1]->function();
	units->node_registry->registered_function[2]->function();
	units->node_registry->registered_function[3]->function();
	serial->node_registry->registered_function[0]->function();
	serial->node_registry->registered_function[1]->function();
	light->node_registry->registered_function[0]->function();
	light->node_registry->registered_function[1]->function();
	temp->node_registry->registered_function[0]->function();

	menu->node[0]->node_registry->registered_function[0]->function(); // temp node
	menu->node[0]->node[0]->node_registry->registered_function[0]->function(); // temp -> units node
	menu->node[0]->node[0]->node_registry->registered_function[1]->function(); // temp -> units node
	menu->node[0]->node[0]->node_registry->registered_function[2]->function(); // temp -> units node
	menu->node[0]->node[0]->node_registry->registered_function[3]->function(); // temp -> units node
	menu->node[1]->node_registry->registered_function[0]->function(); // light node
	menu->node[1]->node_registry->registered_function[1]->function(); // light node
	menu->node[2]->node_registry->registered_function[0]->function(); // serial node
	menu->node[2]->node_registry->registered_function[1]->function(); // serial node

	printf("\nsem array is %c",menu->node_registry->opt_order[0]); 





	return 0;
}
