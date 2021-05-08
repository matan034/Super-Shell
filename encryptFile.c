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
	//check input to be correct
	if(argc!=4) error("Bad num of arguments");
	if(atoi(argv[2])<1||atoi(argv[2])>100)error("Given number must be between 1 to 100"); 
	int i,in,out,rbytes,wbytes;
	char buff[1024];
	if((in=open(argv[1],O_RDONLY))==-1)	error("cant open in file");//input file
	if((out=open(argv[3],O_WRONLY | O_CREAT,0664))==-1)	error("cant open out file");//output file
	if((rbytes=read( in, buff, 1024))==-1)error("read 1"); 

	while(rbytes>0)
	{
		for(i=0;i<rbytes-1;i++) buff[i]=(buff[i]+atoi(argv[2]))%256;//encrypt each char 
		if((wbytes=write(out,buff,rbytes))==-1)error("write");
		if( wbytes != rbytes )error("bad write\n");
		if((rbytes = read(in,buff,1024))==-1)error("read 2"); 
	}
	close(in);close(out);
}
