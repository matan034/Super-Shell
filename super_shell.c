#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

char* locked_commands[256];// a global array to save the names of our locked commands
double time_of_lock[256];//a global array to save how long each command needs to be lockes
struct timeval times[256];//a global array that saves each command time of lock
int last;//global integer to point to the last command that was added to the arrays

/*the func gets command name and time and locks is*/
void lockCmdForTime(char* x,int lock_time)
{
	int i,flag=1;
	i= searchCmd(x);//search if the command is locked already
	if(i!=-1)//if it is locked already
	{
		gettimeofday(&times[i], NULL);//update its lock time to the current time
		time_of_lock[i]=(double)lock_time;//update new lock time
		flag=0;
	}
	if(flag)//if we got a new command we add it to last place in the arrays
	{
		locked_commands[last]=x;
		gettimeofday(&times[last], NULL);
		time_of_lock[last]=(double)lock_time;
		last++;
	}
}
/*this func gets a command name and search if it is already got locked,return the index if true,-1 else*/
int searchCmd(char* command)
{
	int i;
	for(i=0;i<10;i++)
		if(strcmp(locked_commands[i],command)==0)return i;
	return -1;
}
void main(int argc, char* argv[])
{
	int cnt,j=0,i,status,flag=1;
	char *command[5];//array to get input, first cell command name, other cells are args
	char temp[256];
	last=0;
	struct timeval t1;
	pid_t pid;
	for(i=0;i<10;i++) locked_commands[i]=" ";//reset the array
	do
	{
		printf("SuperShell>");
		fgets(temp,256,stdin);//get command from user
		for(i=0;i<strlen(temp);i++)//move on the given string from user to break it to words
		{
			cnt++;//count the chars of word
			if(temp[i]==' '||temp[i]=='\n')//if got to space or null, that meand end of word
			{
				command[j]=(char*)malloc(sizeof(char)*cnt);//memory allocation for the word
				if(command[j]==NULL)
				{
						printf("Memory allocation error\n");
						exit(1);
				}
				strncpy(command[j],temp+i-cnt+1,cnt-1);//move to the the array the current word
				cnt=0;//reset the count
				j++;
			}
		}
		for(j;j<5;j++)//set the remainder of cells to null
			command[j]=NULL;
		j=0;

			if(strcmp(command[0],"lockCmdForTime")==0)//if the given comment is lockCmd 
				 lockCmdForTime(command[1],atoi(command[2]));
			else
			{	
				if((pid=fork())==0)//in case this is the son
				{
					if((i=searchCmd(command[0]))!=-1)//check if the given command could be locked
					{
						gettimeofday(&t1, NULL);//get time stamp 
						if((t1.tv_sec - times[i].tv_sec)>=time_of_lock[i])//check that lock time for the command has passed
						{
							execvp(command[0],command);
							execv(command[0],command);
						}
						else//the command is still locked
						{
							flag=0;
							printf("The command %s is now locked!\n",command[0]);
							exit(1);//return from son
						}
					}
					else//the command is not locked
					{
						execvp(command[0],command);
						execv(command[0],command);
					}
			
					if(strcmp(command[0],"byebye")!=0&&flag)//check if we got an invalid command
					{
						printf("Not Supported\n");
						exit(1);
					}
		 		}
			}
		wait(&status);
		
	}while(strcmp(command[0],"byebye")!=0);
	for(i=0;i<5;i++)//free the memory used for the commands array
		free(command[i]);
}

