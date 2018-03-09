#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node
{
	struct screen *screen;
	struct node **node;
	size_t n;
}node;
typedef struct screen
{
	char **line;
	size_t n;
}screen;
typedef struct node **spine;
struct node **bundle(size_t n,struct node *node,...);
struct node *makenode(struct screen *screen,struct node **node,size_t n);
struct screen *makescreen(size_t n,char *line,...);
struct node **makespine(size_t n,struct node *node,...);
struct node **bundle(size_t n,struct node *node,...)
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
struct node *makenode(struct screen *screen,struct node **node,size_t n)
{
	if(screen==NULL)
	{
		return NULL;
	}
	if(node==NULL&&n!=0)
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
	ret->n=n;
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
struct node **makespine(size_t n,struct node *node,...)
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
