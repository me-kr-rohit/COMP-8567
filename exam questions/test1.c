//Questions 1
The number of forked processes from the main process with immediate descendants is 3 (three)
main()
{
  fork();
  fork();
  fork();
}
TRUE
FALSE

//Question 2
The address of the first element of an array of 10 integers is a constant pointer.
TRUE
FALSE

//Question 3
int (*f1)(int,int);
int (*f2)(int,int);

int a1(int a,int b) 
{
  if(a<=b)return a;
  else return b;
}

int a2(int a,int b) 
{
  if(a>=b)return a;
  else return b;
}

int calculate(int (*f1)(int,int), int (*f2)(int,int), int p1,int p2,int p3,int p4) 
{
  int k=f1(p1,p2);
  int j=f2(p3,p4);
  if(k>=j)return k;
  else return j;
  return k;
}

Which of the following function calls would return maximum( minimum(20,30) , maximum(25,15) ) ?
int ret=calculate(f1,f2,20,30,25,15)
int ret=calculate(&a1,&a2,20,30,25,15)
int ret=calculate(a1,a2,20,30,25,15)
None of the above


//Question 4
int func1(int x)
{
  return(x*x + x - 6);
}
int func2(int x)
{
  return(x*x - 3*x +1);
}
int main()
{
  int (*pt)(int)=func2;
  printf("%d\n", pt(3));
  pt=func1;
  int ret=pt(2);
  printf("%d\n", ret));
  exit(0);
}
What would be the value of ret?
· 0
· 1
· 0
· None of the above
