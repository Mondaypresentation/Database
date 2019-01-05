#pragma once 
#include "page.cpp" 
#include "load.cpp"
#include "database.h"
#include "record.cpp"
#include<stdio.h>
#include<malloc.h>
#include <string.h>

void Project(char *s,int a[])
{   
	//printf("%s\n",s);
	Record *R;
	//printf("!1");
	R=InitRecord();
	//printf("------------------------\n");
	R=output(s);
	//printf("!3\n"); 
	//printf("%s\n",R->attribute[1]->value); 
	char c[255];
	char *d=" "; 
	(char*)memset(c,0x00,255*sizeof(char));
	for(int i=0;i<Max_Attribute_Num;i++)
	{
		//printf("%s\n",c);
	    if(a[i]==1)
		{
		strcat(c,R->attribute[i]->value); 
		strcat(c,d);	
		}	
	}
	printf("%s\n",c);
	free(d); 
	free(R);
	return;
}
int *AddAttribute(int a[],int i)
{
	a[i]=1;
	return a;
}
