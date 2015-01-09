#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<libgen.h>
#include<sys/types.h>
#include<unistd.h>

int seconds_till_now();  // function to calculate the seconds elasped since midnight
void process_arguments(int argc , char ** argv);  //this function processes the arguments
int get_time_compute_delay(char timeString[]);  //this function validates time and computes delay
void validate_time(int hh, int mm , int ss) ;//this function validates time
validate_formation_of_time_string(char inputArr[]);  // this function validates the formation of time string
void pathfinder(); //computes the path of executable file
void At_cmd(int delay_seconds,char filePath[]) ; //At_cmd function which invokes the child process

int total_command_arguments=0;  //this stores the total number of command arguments
char arglist[10][20];  //this array stores the command arguments
char time_argument[10];  //this string stores the time string
int delay_seconds=0;
char executable_file_path[30];  //this stores the path of executable file

main(int argc , char ** argv)
{
total_command_arguments=argc-2; //because first 2 are At and the time string

 /*//PRINTING RAW INPUT  !!
printf(" arguments are \n" );
int i=0;
printf("value of argc is %d",argc);
for(i=0;i<argc;i++)
{
printf("\n argv[%d] is %s \n",i,argv[i]);
}*/


process_arguments(argc,argv);
//printf("\n\n seconds elapsed till now is %d",seconds_till_now());

delay_seconds=get_time_compute_delay(time_argument);
printf("\nDelay Time for command execution: %d",delay_seconds);
printf("\n------------------------------\n");
pathfinder();


//forking to create a new process..
pid_t At_cmd_id=fork();
if(At_cmd_id==0)  //child portion
{
pid_t new_process_group_id=setpgrp();//to make this process the head of its own process group !!

printf("\n------------------------------\n");
printf("\nChild Code: My process ID = %d",getpid() );
printf("\nChild Code: Parent process ID = %d",getppid() );
printf("\nChild Code: Inherited group ID = %d",getppid() );
printf("\nChild Code: My new group ID = %d",new_process_group_id );

At_cmd(delay_seconds,executable_file_path);  //calling the At_cmd function 

}
else  //parent process
{

printf("\n------------------------------\n");
printf("\n Parent Code: My process ID = %d",getpid());
printf("\n Parent Code: My group ID = %d",getpid());
printf("\n Parent Code: My child's process ID = %d",At_cmd_id);
printf("\n Parent Code: Old SHELL = %s",getenv("SHELL"));
putenv("SHELL=/usr/bin/sh");
printf("\n Parent Code: New SHELL = %s",getenv("SHELL"));

printf("\n------------------------------\n");
}

exit(0);

}  //end of main


void At_cmd(int delay_seconds,char executable_file_path[])
{

sleep(delay_seconds);

printf("\n Child Code:Printing  environment variable %s \n",getenv("SHELL"));
printf("\n------------------------------\n");

char *ptr[total_command_arguments];  
ptr[0]=&executable_file_path;
int i=1;
for(i=1;i<total_command_arguments;i++)
{
ptr[i]=&arglist[i];
}
ptr[i]='\0';


//char *args[] = {"usr/bin/ls", "-ltr" , '\0'};
//execv("/usr/bin/ls",args);

execv(executable_file_path,ptr);

}





void pathfinder()
{


char *p=  pathfind (getenv ("PATH"), arglist[0], "rx");

if(p != NULL)
{

//printf("\n\n the path is found for %s !!",arglist[0]);
int counter=0;
while(*p!='\0')
{
executable_file_path[counter]=*p;
p++;counter++;
}
printf("\n Executable command file =  %s \n",executable_file_path);
printf("\n------------------------------\n");
}
else{
printf("\n Path Error: %s is a non-existing command",arglist[0]);
printf("\n------------------------------\n");
exit(0);
}

}



int get_time_compute_delay(char inputArr[])
{
validate_formation_of_time_string(inputArr);

//printf("in the function get time compute delay with the input %s",inputArr);
char hours_string[2];char minutes_string[2];char seconds_string[2];

char * token=&inputArr[0];
token=token+1;

int i=0;
for(i=0;i<2;i++)
{
hours_string[i]=*token;
token=token+1;
}
hours_string[i]='\0';
int hours_int=atoi(hours_string);
token=token+1;
for(i=0;i<2;i++)
{
minutes_string[i]=*token;
token=token+1;
}
minutes_string[i]='\0';
int minutes_int=atoi(minutes_string);
token=token+1;
for(i=0;i<2;i++)
{
seconds_string[i]=*token;
token=token+1;
}
seconds_string[i]='\0';
int seconds_int=atoi(seconds_string);

//printf("time breaks is - %d %d %d",hours_int,minutes_int,seconds_int);

validate_time(hours_int,minutes_int,seconds_int);

// Computing delay starts

int delay_sec=0;
if(inputArr[0] == '+')
{
delay_sec=(hours_int*60*60)  +  (minutes_int*60) + seconds_int;
}

int actual_seconds_till_now=seconds_till_now();

int entered_time_in_seconds=(hours_int*60*60)  +  (minutes_int*60) + seconds_int;

if(inputArr[0]=='@')
{
if(entered_time_in_seconds > actual_seconds_till_now)
{
delay_sec=entered_time_in_seconds-actual_seconds_till_now;
}
else
{
delay_sec=entered_time_in_seconds+(24*60*60) - actual_seconds_till_now;
}
}

//Printf("\n delay isss - %d",delay_sec);
return delay_sec;   //returning the delay
}

void validate_time(int hh,int mm,int ss)
{
if(hh>23)
{
printf(" \n\n Hours exceeded 24 !!!!");
printf("\n------------------------------\n");
exit(0);
}
if(mm>59)
{
printf(" \n\n Minutes exceeded 60 !!!!");
printf("\n------------------------------\n");
exit(0);
}
if(ss>60)
{
printf(" \n\n Seconds exceeded 60 !!!!");
printf("\n------------------------------\n");
exit(0);
}
}


void process_arguments(int argc , char ** argv)
{
//printf("\n inside the function process_arguments !!");
//printf("\n time string copying starts !!");
int i=0;
char *q;
q=&argv[1][0];
int counter=0;
while(*q != '\0')
{
time_argument[counter]=*q;
q=q+1;counter=counter+1;
}
time_argument[counter]='\0';
printf("\n The time entered  is %s",time_argument);
printf("\n------------------------------\n");


//printf("\n copying command list arguments starts !!");
for(i=0;i<total_command_arguments;i++)
{
//printf("\n in the loop the %d(th) time to copy the command list in arglist",i+1);
char *p;
p=&argv[i+2][0];
counter=0;
while(*p != '\0')
{
arglist[i][counter]=*p;
p=p+1;counter=counter+1;
}
arglist[i][counter]='\0';
//printf("\n copied the %d(th) command parameter successfully into agrlist",i+1);
}
arglist[i][0]='\0';  //null pointer to last element of arglist
//printf("\n\n printing the list of command arguments !!");
printf("\n Command-string =  ");
for(i=0;i<total_command_arguments;i++)
{
printf(" %s " , arglist[i]);
}
printf("\n------------------------------\n");

}  // end of function process_arguments


int seconds_till_now()
      {
	   
               struct tm *T;
               time_t ltime=1;
               time(&ltime);
               T = localtime(&ltime);
             // printf("\n\n Inside the function seconds_till_now(). The current time is: \n time is sec=%d, min=%d, hour=%d\n", T->tm_sec, T->tm_min, T->tm_hour);
              return (  ((T->tm_hour)*60*60)+((T->tm_min)*60)+((T->tm_sec))  );
	   
	  
	   }


 validate_formation_of_time_string(char inputArr[])
{
char * token=&inputArr[0];
if(!(*token=='+'  || *token=='@' ))
{
printf("\n the first character is not a '+' or '@'");
printf("\n------------------------------\n");
exit(0);
}
token++;
//pointing to hh
if(! isdigit(*token))
{
printf("\n Hours entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
if(! isdigit(*token))
{
printf("\n Hours entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
if(*token !=':')
{
printf("\n Hours entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
//pointing to mm
if(! isdigit(*token))
{
printf("\n Minutes entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
if(! isdigit(*token))
{
printf("\n Minutes entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
if(*token !=':')
{
printf("\n Minutes entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
//pointing to ss
if(! isdigit(*token))
{
printf("\n Seconds entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
if(! isdigit(*token))
{
printf("\n Seconds entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
token++;
if(*token != '\0')
{
printf("\n Seconds entered is not correct");
printf("\n------------------------------\n");
exit(0);
}
}


