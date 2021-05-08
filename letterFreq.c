#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void error(char* str)
{
		perror(str);
		exit(EXIT_FAILURE);
}

void main(int argc,char* argv[])
{
	if(argc!=2) error("Bad num of arguments");
	int rbytes,i,i_max,i_secMax,i_thirdMax,in,cnt=0;
	int letter[26];
	char temp[1024];
	double max=0,secMax=0,thirdMax=0;
	for(i=0;i<26;i++) letter[i]=0;
	if((in=open(argv[1],O_RDONLY))==-1)	error("cant open in file");
	if((rbytes=read( in, temp, 1024))==-1)error("read 1"); 
	while(rbytes>0)
	{
		for(i=0;i<rbytes-1;i++)
		{
			if(temp[i]>='a'&&temp[i]<='z')// if letter is between a and z we increase our monim array in the correct position and our cnt to know how many letter there are in general
			{
				letter[temp[i]-'a']++;
				cnt++;
			} 
			if(temp[i]>='A'&&temp[i]<='Z')//same for A and Z
			{
				letter[temp[i]-'A']++;
				cnt++;
			}	
		}
		if((rbytes=read(in, temp, 1024))==-1)error("read 2");
	}
	for(i=0;i<26;i++)//for to find our 3 max letters
	{
		if(letter[i]>max)//here we find our max
		{
			thirdMax=secMax;/*if we found a new max we must update our third max and second max accordingly*/
			secMax=max;
 			max=letter[i];
			i_thirdMax=i_secMax;//update indexes accordingly
			i_secMax=i_max;
			i_max=i;
		}
		else if(letter[i]>secMax)//finds our second max
		{ 
			thirdMax=secMax;
			secMax=letter[i];
			i_thirdMax=i_secMax;
		    i_secMax=i;
		}
		else if(letter[i]>thirdMax)//finds our third max
		{
			thirdMax=letter[i];
		    i_thirdMax=i;
		}
	}
	printf("%c - %.2lf%\n",(char)(i_max+'a'),(max/cnt)*100);
	printf("%c - %.2lf%\n",(char)(i_secMax+'a'),secMax/cnt*100);
	printf("%c - %.2lf%\n",(char)(i_thirdMax+'a'),thirdMax/cnt*100);
	if(i_max==4&&i_secMax==0&&(i_thirdMax==8||i_thirdMax==14))
	printf("Good Letter Frequency\n");
	
	close(in);
	
}
