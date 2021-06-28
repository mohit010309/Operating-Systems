//19ucc023
//Mohit Akhouri
//OS Lab ( Wednesday Batch ( 2-4pm) )
//OS_Lab_3_Task 1
//February 6, 2021
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void read_ProcessorType();
void read_KernelVersion();
void read_TotalMemory();
void read_Uptime();
int find(char src[],char ch,int l);
int num_equivalent(char ch);
void main()
{
	//calling the functions
	read_ProcessorType();
	read_KernelVersion();
	read_TotalMemory();
	read_Uptime();
}
void read_Uptime() //to calculate the time since the system was last booted using /proc/uptime
{
	//using the concept of File handling to implement the function
	FILE *fp;
	fp=fopen("/proc/uptime","r");
	char line[100];
	float time=0.0;
	int i=0;
	while(fgets(line,100,fp)!=NULL)
	{
		while(isdigit(line[i]))
		{
			time=(time*10)+(float)num_equivalent(line[i]);
			i++;
		}
	}
	int d,h,m,s;
	d=0;
	h=0;
	m=0;
	s=0;
	//calculating different components of time like days,hours,minutes and seconds
	int time1=(int)time;
	if(time1/86400!=0)
	{
		d=time1/86400;
		time1=time1-(86400*(time1/86400));
	}
	if(time1/3600!=0)
	{
		h=time1/3600;
		time1=time1-(3600*(time1/3600));
	}
	if(time1/60!=0)
	{
		m=time1/60;
		time1=time1-(60*(time1/60));
	}
	s=time1;
	//printing time since system was last booted
	printf("Amount of time since the system was last booted : %d days, %d hours, %d minutes, %d seconds\n",d,h,m,s);
	
	
	fclose(fp);
}
void read_TotalMemory() //to find the Total memory available in /proc/meminfo and print it
{
	//using the concept of File handling to implement the function
	FILE *fp;
	fp=fopen("/proc/meminfo","r");
	char line[100];
	char search[100]="MemTotal";
	char *ret;
	int flag=0;
	int i;
	while(fgets(line,100,fp)!=NULL)
	{
		ret=strstr(line,search);
		if(ret!=NULL)
		{
			flag=1;
			break;
		}
	}
	i=0;
	while(!isdigit(line[i]))
		i++;
	printf("Amount of memory configured into this computer : ");
	while(i<strlen(line))
	{
		printf("%c",line[i]);
		i++;
	}
	printf("\n");
	fclose(fp);
		
}
void read_KernelVersion() //to find the Kernel version in /proc/version and print it
{
	//using the concept of File handling to implement the function
	FILE *fp;
	fp=fopen("/proc/version","r");
	char line[500];
	int i;
	int x;
	printf("Kernel version : ");
	while(fgets(line,500,fp)!=NULL)
	{
		i=find(line,'(',strlen(line))-2;
		x=0;
		while(x<i)
		{
			printf("%c",line[x]);
			x++;
		}
	}
	printf("\n\n");
	fclose(fp);
	
}
void read_ProcessorType() //to find the processor type in /proc/cpuinfo and print it
{
	//using the concept of File handling to implement the function
	FILE *fp;
	fp=fopen("/proc/cpuinfo","r");
	char *ret;
	char line[100];
	char search[]="model name";
	int flag=0;
	char *arr[100];
	int i;
	char ch;
	while(fgets(line,100,fp)!=NULL)
	{
		ret=strstr(line,search); //strstr to search for <search> string in <line> string
		if(ret!=NULL)
		{
			
			flag=1;
			break;
		}
	}
	if(flag==1)
	{
		printf("Processor Type : ");
		i=find(line,':',strlen(line));
		while(i<strlen(line))
		{
			printf("%c",line[i]);
			i++;
		}
				
	}
	printf("\n");
	fclose(fp);
}
int find(char src[],char ch,int l) //to find and return the index of 'ch' in string 'src' of length 'l'
{
	int i;
	for(i=0;i<l;i++)
	{
		if(src[i]==ch)
			return i+1;
	}
}
int num_equivalent(char ch) //to return the equivalent digit for a character digit
{
	switch(ch)
	{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default:
			return -1;
	}
}
			
		
