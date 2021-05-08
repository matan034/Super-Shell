#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

void error(char* str)
{
		perror(str);
		exit(1);
}

void main(int argc,char* argv[])
{
	if(argc!=4) error("Bad num of arguments");//check if we got correct number of arguments
	int i,in,out,rbytes,wbytes,index,cnt=0;
	char buff[1024];
	index=atoi(argv[3]);
	if(index==0 && strcmp(argv[3],"0")!=0) error("Given index must be an integer");//check that the given index is valid
	if((in=open(argv[1],O_RDONLY))==-1)	error("cant open in file");
	if((out=open(argv[2],O_WRONLY | O_CREAT,0664))==-1)	error("cant open out file");
	if((rbytes=read( in, buff, 1024))==-1) error("read 1"); 
	while(rbytes>0)//run as long we have input from file
	{
		for(i=0;i<rbytes;)
		{
			while(buff[i]!=' ' &&buff[i]!='\0')//as long the currunt char is not space or null
			{
				if(cnt==index)//if the current index in the current word is equal to the given index
				{
					if(buff[i]>='A'&&buff[i]<='Z')//if the current char is a uppercase letter we lower it
					{
						 buff[i]=tolower(buff[i]);
					}
				}				
				cnt++;//move one char in the current word
				i++;//move one char on the string
			}	
		i++;
		cnt=0;//count a new word
		}
		if((wbytes=write(out,buff,rbytes))==-1)error("write");
		if( wbytes != rbytes )error("bad write\n");
		if((rbytes = read(in,buff,1024))==-1)error("read 2"); 	
	}
	close(in); close(out);
}
