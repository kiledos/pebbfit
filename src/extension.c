#include <pebble.h>
#include "extension.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static int counter=0;
static int mode = -1;
static int array[3000];
static int ind = 0;

void printA(int * a,int l){
  int i;
  for(i=0;i<l;i++)
  {
    printf("%d ",a[i]);
  }
  printf("\n");
}


int caverage(int list[][3],int m,int index){
  
  int accum =0, diff =0, i,cavg;  
  for(i=1;i<m;i++){    
    diff = list[i][index]-list[i-1][index];
    accum += diff;    
  }  
  cavg = accum*100/(m-1); 
  return cavg; 
}

int average(int list[][3],int m,int index){
  int accum =0, i, avg;  
  for(i=0;i<m;i++){     
    accum += list[i][index];
  }
  avg = accum*100/m;  
  return avg; 
}

int isavhigh(int list[][3],int m){
  
  int n=10*100; //Acceleration threshold
  int avgx = abs(caverage(list,m,0));  
  int avgy = abs(caverage(list,m,1));  
  int avgz = abs(caverage(list,m,2)); 
  
  printf("\navgx:%d,avgy:%d,avgz:%d",avgx,avgy,avgz);
  
  if ((avgx>n&&avgy>n)||(avgy>n&&avgz>n)||(avgx>n&&avgz>n)){   
    return 1;
  }
  return 0;
}

int isforce(int list[][3],int m){  
  
  int t = 100*450; //Force threshold
  
  int fx = abs(average(list,m,0))-10000;  
  int fy = abs(average(list,m,1))-10000;  
  int fz = abs(average(list,m,2))-10000;   
  
  //printf("fx:%d,fy:%d,fz:%d",fx,fy,fz);
  
  if ((fx>t&&fy>t)||(fy>t&&fz>t)||(fx>t&&fz>t)){
    return 1;   
  }
  return 0;
}

int Pqualifier(int list[][3],int m){ 
  int qualify=0;
  int x = isavhigh(list,m);
  int y = isforce(list,m);
  
  if ((x|y)==1){
    qualify = 1;    
  }
  printf("Qualify -> x:%d,y%d\n",x,y);
  return qualify;
}
  /*if (qualify==1){
    period[counter]=1;
    
    counter++;
  }
  if (qualify==0){
    period[counter]=0;
    counter++;
  }*/
  
void appendArray(int list[][3],int m)
{
  array[counter] = Pqualifier(list,m);
  if (mode==-1)
    mode = array[counter];
  counter++;
}

int* decimalArray(int l)
{
  int i,current, previous = array[0],count = 0;
  int *d = (int*)calloc(l,sizeof(int*));
  
  for (i=1;i<l;i++)
  {
    current = array[i];
    if(current==previous)
    {
      count++;  
    }
    else if(i==1)
    {
      d[ind] = 1;
      previous = current;
      ind++;
      count = 1;
    }
    else
    {
     d[ind] = count;
     previous = current;
     ind++;
     count = 1;
    }
    if(i==l-1)
    {
      d[ind] = count;
      ind++;
    }
  }
  printA(d,ind);
  return d;
}

int* dataArray(int* decarray, int length)
{
  int i,current = 0,previous = mode,ncounter = 0;
  int lt = 5, ht = 7;
  int *f = (int*)calloc(length,sizeof(int*));
  f[0] = mode;
  
  for (i=1;i<=length;i++)
  {
    current = decarray[i-1];
    if(i==1)
    {
       f[1] = current;       
       ncounter++;
    }   
    if (previous == 1 && i%2 == 0) //If previous element is high and next is low
    {
      if(current>lt) {
        f[ncounter] = current;
        ncounter++;
        previous = 0;
      }
      else //current<lt 
        f[ncounter-1] += current;
    }
    else if (previous == 1 && i%2 == 1) //If previous and next element are both high
    {
      f[ncounter-1] += current;      
    }
    else if (previous == 0 && i%2 == 1){  //If previous element was low and next is high
      if(current>ht){
        f[ncounter] = current;
        previous = 1;
        ncounter++;
      }
      else //current<ht
        f[ncounter-1] += current;           
    }
    else { //(previous == 0 && i%2 == 0)
      f[ncounter-1] += current;
    }
      
  }
  printA(f,ncounter);
  return f; 
}

int* session(){  
  printA(array,counter);
  int *darray = decimalArray(counter);  
  int *sessiondata = dataArray(darray,ind); 
  return sessiondata;
}
 




  



