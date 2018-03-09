#include "local.h"
int main(void)
{
	screen *rootS=makescreen(2,"menu","1:A , 2:B");
	screen *AS=makescreen(2,"A","A1, A2");
	screen *BS=makescreen(2,"B","B1, B2");
	screen *B2S=makescreen(2,"B2","B2.1, B2.2");
	node *B2N=makenode(bundle_screen(1,B2S),NULL,1,0,2);
	node *BN=makenode(bundle_screen(1,BS),bundle_node(1,B2N),1,1,2);
	node *AN=makenode(bundle_screen(1,AS),NULL,1,0,2);
	node *rootN=makenode(bundle_screen(1,rootS),bundle_node(2,AN,BN),1,2,2);
	printf("%s\n",rootN->screen[0]->line[0]);
	printf("%s\n",rootN->screen[0]->line[1]);
	printf("%s\n",rootN->node[0]->screen[0]->line[0]);
	printf("%s\n",rootN->node[0]->screen[0]->line[1]);
	printf("%s\n",rootN->node[1]->screen[0]->line[0]);
	printf("%s\n",rootN->node[1]->screen[0]->line[1]);
	printf("%s\n",rootN->node[1]->node[0]->screen[0]->line[0]);
	printf("%s\n",rootN->node[1]->node[0]->screen[0]->line[1]);
	return 0;
}
