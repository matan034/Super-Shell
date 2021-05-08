#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void error(char* str)
{
		perror(str);
		exit(EXIT_FAILURE);
}

void main(int argc,char* argv[])
{
	if(argc!=3) error("Bad num of arguments");//check that we got correct number of arguments
	srand(time(NULL));  //reset time for random
	int len,i,out,wbytes,cnt=0;
	len=atoi(argv[1]);
	char* buff=(char*)malloc(sizeof(char)*len);//create a buffer according to the given index
	if(buff==NULL) error("cant allocate memory");
	if((out=open(argv[2],O_WRONLY | O_CREAT,0664))==-1)	error("cant open out file");
	for(i=0;i<len;i++)
		buff[i]= (char)(rand() % ('z'+1-'a')+'a');//set each char in the string as a random letter
	if((wbytes=write(out,buff,len))==-1)error("write");
		if( wbytes != len )error("bad write\n");
	close(out);
	free(buff);//free the memory of buffer;
}	

