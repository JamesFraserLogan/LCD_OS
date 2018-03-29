#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
volatile size_t node_depth=0; // RootNode->node[a1]->node->[a2]->....->node[an] is the topology this counter follows. A node depth of 0 denotes the root node.
volatile size_t max_node_depth=0; // Be careful this is set correctly, it's used to determine how much memory is alloacated to the node_indices array. This would be n in the above example.
volatile size_t screen_count=0; // This is "i" in: RootNode->node[a1]->...->node[an]->screen[i] ; that is, this is which screen you are on in a particular node.
volatile size_t line_count=0; // This is "j" in: RootNode->node[a1]->...->node[an]->screen[i]->line[j] ; that is, this is which line you are on in a particular screen in a particular node.
volatile size_t *node_indices; // This is the array of node index coefficients {a1,a2,a3,...,an} in the above example.
volatile size_t opt_counter=0; // This is which option you are on in any particular node. Registered functions can be attached to options.
typedef struct node // Links to i other nodes, j screens, and k functional options. Functional options can invoke a function via the node registry, or link to another node and/or screen.
{
	struct node **node;
	struct node_registry *node_registry;
	struct screen **screen;	
	size_t nnodes;
	size_t nopts;
	size_t nscreens;
}node;
typedef struct node_registry // Links to n registered functions to be invoked in a specific node. The order array is 0 for node navigation, and 1 for function invocation, thus functions must be properly ordered.
{
	struct registered_function **registered_funciton;
	size_t n;
	unsigned char *opt_order;
}node_registry;
typedef struct registered_function // Prepares void functions with void arguments to be inserted into a node registry.
{
	void (*function)(void);
}registered_function;
typedef struct screen // Each screen contains n lines of (possibly) variable length.
{
	char **line;
	size_t n;
}screen;
struct node **bundle_node(size_t n,struct node *node,...); // Bundles n nodes into a single **node. Returns NULL on error.
struct registered_function **bundle_registered_functions(size_t n,struct registered_function *function,...); // Bundles n registered functions into a single **registered_function. Returns NULL on error.
struct screen **bundle_screen(size_t n,struct screen *screen,...); //Bundles n screens into a single **screen. Returns Null on error.
struct node *makenode(struct screen **screen,struct node **node,size_t nscreens,size_t nnodes,size_t nopts,struct node_registry *reg); // Returns a *node. Feel free to use NULL as an input to any **node, but do not use NULL as an input to **screen. Each node must have at least one screen. Returns NULL on error.
struct node_registry *makenodereg(unsigned char *opt_order,size_t n,registered_function **registered_function); // Returns a *node_registry. Returns NULL on error.
struct screen *makescreen(size_t n,char *line,...); // Returns a *screen. Empty screens are not allowed. Returns NULL on error.
void node_depth_up(struct node **node,size_t index); // Sequential function that increases node depth. Only use this starting from the root node and building up to n node depth. Use "&mynode" as input, not a bundle, as C funcitons pass by value thus a **node input allows this function to modify the target *node. Simple function with no error checking, BE CAREFUL!
unsigned char *order_opts(size_t nopts,char *bin_sem); // Returns a binary semaphore arrary with '0's for node navigation and '1's for functions, thus ordering the options in each node structure.
struct registered_function *register_function(void (*function)(void)); // Returns a registered_function * to a void function with void arguments.
struct node **bundle_node(size_t n,struct node *node,...)
{
	if(n<=0||node==NULL)
	{
		return NULL;
	}
	struct node **ret=(struct node **)malloc(sizeof(struct node *));
	if(ret==NULL)
	{
		return NULL;
	}
	*ret=node;
	va_list ap;
	va_start(ap,node);
	for(size_t i=1;i<n;i++)
	{
		*(ret+i)=va_arg(ap,struct node *);
		if(*(ret+i)==NULL)
		{
			return NULL;
		}
	}
	va_end(ap);
	return ret;
}
struct registered_function **bundle_registered_functions(size_t n,struct registered_function *function,...)
{
	if(n<=0||function==NULL)
	{
		return NULL;
	}
	struct registered_function **ret=(struct registered_function **)malloc(sizeof(struct registered_function *));
	if(ret==NULL)
	{
		return NULL;
	}
	va_list ap;
	va_start(ap,function);
	for(size_t i=0;i<n;i++)
	{
		*(ret+i)=va_arg(ap,struct registered_function *);
		if(*(ret+i)==NULL)
		{
			return NULL;
		}
	}
	va_end(ap);
	return ret;
}
struct screen **bundle_screen(size_t n,struct screen *screen,...)
{
	if(n<=0||screen==NULL)
	{
		return NULL;
	}
	struct screen **ret=(struct screen **)malloc(sizeof(struct screen *));
	if(ret==NULL)
	{
		return NULL;
	}
	*ret=screen;
	va_list ap;
	va_start(ap,screen);
	for(size_t i=1;i<n;i++)
	{
		*(ret+i)=va_arg(ap,struct screen *);
		if(*(ret+i)==NULL)
		{
			return NULL;
		}
	}
	va_end(ap);
	return ret;
} 
struct node *makenode(struct screen **screen,struct node **node,size_t nscreens,size_t nnodes,size_t nopts,struct node_registry *reg)
{
	if(screen==NULL)
	{
		return NULL;
	}
	if(node==NULL&&nnodes!=0)
	{
		return NULL;
	}
	if(reg==NULL)
	{
		return NULL;
	}
	struct node *ret=(struct node *)malloc(sizeof(struct node));
	if(ret==NULL)
	{
		return NULL;
	}
	ret->screen=screen;
	ret->node=node;
	ret->nscreens=nscreens;
	ret->nnodes=nnodes;
	ret->nopts=nopts;
	ret->node_registry=reg;
	return ret;
}
struct node_registry *makenodereg(unsigned char *opt_order,size_t n,registered_function **registered_function)
{
	if(n<0) // incase one wants to swap out all size_t variables with ints or other signed variables
	{
		return NULL;
	}
	struct node_registry *ret=(struct node_registry *)malloc(sizeof(struct node_registry));
	if(ret==NULL)
	{
		return NULL;
	}
	ret->opt_order=opt_order;
	ret->n=n;
	ret->registered_funciton=registered_function;
	return ret;
}
struct screen *makescreen(size_t n,char *line,...)
{
	if(line==NULL)
	{
		return NULL;
	}
	struct screen *ret=(struct screen *)malloc(sizeof(struct screen));
	if(ret==NULL)
	{
		return NULL;
	}
	char **arr=(char **)malloc(sizeof(char *));
	if(arr==NULL)
	{
		return NULL;
	}
	char *line1=(char *)malloc((strlen(line)+1)*sizeof(char));
	if(line1==NULL)
	{
		return NULL;
	}
	strcpy(line1,line);
	*arr=line1;	
	va_list ap;
	va_start(ap,line);
	for(size_t i=1;i<n;i++)
	{
		char *p=va_arg(ap,char *);
		char *temp=(char *)malloc((1+strlen(p))*sizeof(char));
		if(temp==NULL)
		{
			return NULL;
		}
		strcpy(temp,p);
		*(arr+i)=temp;
	}
	va_end(ap);
	ret->line=arr;
	ret->n=n;
	return ret;
}
void node_depth_up(struct node **node,size_t index)
{
	*node=(*node)->node[index];
}
unsigned char *order_opts(size_t nopts,char *bin_sem)
{
	if(nopts==0)
	{
		return NULL;
	}
	unsigned char *ret=(unsigned char *)malloc((1+nopts)*sizeof(unsigned char));
	if(ret==NULL)
	{
		return NULL;
	}
	for(size_t i=0;i<nopts;i++)
	{
		*(ret+i)='0';
	}
	*(ret+nopts)='\0';
	if(bin_sem==NULL)
	{
		return ret;
	}
	if(strlen(bin_sem)!=nopts)
	{
		free(ret);
		return NULL;
	}
	for(size_t i=0;i<nopts;i++)
	{
		if(*(bin_sem+i)=='1')
		{
			*(ret+i)='1';
		}
		else if(*(bin_sem+i)!='0')
		{
			free(ret);
			return NULL;
		}
	}
	return ret;
}
struct registered_function *register_function(void (*function)(void))
{
	if(function==NULL)
	{
			return NULL;
	}
	registered_function *ret=(struct registered_function *)malloc(sizeof(struct registered_function));
	if(ret==NULL)
	{
		return NULL;
	}
	ret->function=function;
	return ret;
}
void testvoid(void)
{
	printf("testvoid\n");
}
