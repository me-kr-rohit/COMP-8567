//Question 1
Any thread can kill any other thread within the program
 TRUE  //correct answer
 FALSE
 
//Question 2
main()
{
  int num1,num2;
  int fd1=open("input.txt",O_RDONLY);
  int fd2=open("output.txt",O_RDWR);
  printf("\nSUNDAY");
  printf("\nMONDAY");
  int c1=dup(0);
  dup2(fd1,0);
  scanf("%d",&num1);
  scanf("%d",&num2);
  printf("\nTUESDAY");
  int c2=dup(1);
  dup2(fd2,1);
  printf("\nThe sum of two numbers is %d\n",num1+num2);
  dup2(1,c2);
  dup2(0,c1);
  printf("\nWEDNESDAY");
  printf("\nTHURSDAY");
}

What is the output of the above program?
 SUNDAY  // correct answer
  MONDAY
  TUESDAY
 TUESDAY
 WEDNESDAY
  THURSDAY
 SUNDAY
  MONDAY

 
//Question 3 
void handler()
{
  exit(0);
}

int main(int argc, char *argv[])
{
int i, pid;
signal(SIGINT, handler);
for(int i=1;i<4;i++)
fork();
if((pid=fork()) == 0)
{
  setpgid(0, getpid());
  for(;;){
  sleep(1);
  }
}
else
{
  for(;;)
  {
    sleep(1);
  }
}
}
How many processes will not be killed when you run the program and you press ctrl-c ?
 2
 8
 4
 None of the options

 
//Question 4
main()
{
  int a, b;
  long int count;
  char *buffer;
  f1=open("sample.txt", O_RDONLY);
  dup2(f1, STDIN_FILENO);
  c=lseek(STDIN_FILENO, 0, SEEK_END);
  lseek(STDIN_FILENO, 0, SEEK_SET);
  buffer=malloc(count);
  read(0, buffer, count);
  write(STDOUT_FILENO, buffer, count);
}
What does the above program do?
 Copies contents from keyboard to a file
 Displays the contents of a file on the monitor/screen.  //correct answer
 None of the options
 Copies files

 //Question 5
main()
{
int fd3=open("sample.txt",O_RDONLY);
char buffer[95];
long int n;
for(int i=0;i<15;i++)
{
  n+=read(fd3,buffer,15);
}

for(int i=0;i<5;i++)
{
  printf("\n%d", (int) lseek(fd3, -100, SEEK_END));
}
close(fd3);
}
Assuming that the size of sample.txt is 50 bytes, what would be the output of the final printf statement in this program?
 -100
 -450
 -50
 None of the options  //correct answer
