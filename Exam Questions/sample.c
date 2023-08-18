Q1)
close(fd) returns 0 if the fd is already closed.
True
False

Q2)
Main(){
int pid1,pid2,pid3,pid4;
pid1=fork();
pid2=fork();
pid3=fork();
pid4=fork();
if(pid1==0||pid2==0||pid3==0||pid4==0){
exit(0);
}
else
{
for(;;);
} }

How many zombie processes are created?
4
16
12
None of the options
 

Q3)

//Note: Only the operational part of the program is shown below.
int fd=open("sample.txt",O_CREAT|O_RDWR);
int long n=lseek(fd,20,SEEK_END);
char buff1[15];
for(int i=0;i<15;i++)
{
  buff1[i]='A';
}
n=write(fd,buff1,15);
close(fd);
sample.txt would contain?
a. None of the options
b. 35 Characters
c. 15 consecutive 'A' characters only
d. 20 NULL characters only


Q4)
//Note: Only the operational part of the program is shown below.
int pid = fork();
if(pid==0)
{
  exit(0);
}
else if(pid<0)
{
  printf("Error");
}
else
{
while(1)
sleep(1);
}
The above code segment would lead to:
a. A zombie process
b. An orphan process
c. WIFSIGNALED(status) =TRUE
d. None of the options

Q5)
void main()
{
fork();
fork();
fork()
}

The number of processes that will not fork a child process would be 4
True
False
