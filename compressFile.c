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
/*the func gets a string and the index of the start and the end of a streak in the string to correct, and correct is according to instruction*/
void correctStr(char* str,int start, int last,int len)
{
	int cnt,i=start+1;
	cnt=last-start+1;
	str[i++]=cnt+'0';//set in the string the lengts of streak
	for(i;i<len-cnt+2;i++)//move back all the letters after streak
	{
		str[i]=str[i+cnt-2];
	}
	str[i]='\0';//set null to end our new string
}

void main(int argc,char* argv[])
{
	if(argc!=3) error("Bad num of arguments");//check that we got the correct number of arguments
	int maxLen,wbytes,rbytes,in,out,i,j,k,cnt=0;
	char buff[1024],temp;
	if((in=open(argv[1],O_RDONLY))==-1)	error("cant open in file");
	if((out=open(argv[2],O_WRONLY | O_CREAT,0664))==-1)	error("cant open out file");
	if((rbytes=read( in, buff, 1024))==-1) error("read 1"); 
	while(rbytes>0)//as long we have input from file
	{
		temp=buff[0];//set temp for the first letter
		cnt=1;
		for(i=1;i<rbytes;i++)//move on the string
		{
			if(buff[i]==temp)cnt++;//if the current char is equal to the last char update count
			else//we got a new char
			{
				if(cnt>4)//if the streak was of at least 4 letters
				{
					correctStr(buff,i-cnt,i-1,rbytes);//correct the string according to the instructions
					rbytes=rbytes-cnt+2;//update the len of string
					i=i-cnt+2;//move to the correct index according to new string
				}
				temp=buff[i];//save the current letter
				cnt=1;//reset counter
			}
		}
		if((wbytes=write(out,buff,rbytes))==-1)error("write");
		if((rbytes=read( in, buff, 1024))==-1) error("read 2"); 
	}
	close(in); close(out);
}
