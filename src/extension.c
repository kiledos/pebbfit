#include <pebble.h>
#include "extension.h"
#include <math.h>
#include <stdlib.h>


static int counter=0;
static int mode=1;
static int zerocount=0;
static int onecount=0;
static int consecutivehi =0;
static int consecutivelo = 0;
static int last=1; //change to start of workout after

void printsomething(int list[], int n){
  int i;
  for (i=0;i<n;i++){
    //printf("%d",list[i]);
  }
}

int isforce(double x, double y, double z){
  int force;
  if (((x&&y)>200)||((x&&z)>200)||((y&&z)>200)){
    force =1;
    //printf("%d",force);
  }
  else{
    force =0;
  }
  
  return force;
}
double averagex(int16_t **list){
  int accum =0;
  int diff;
  int i;
  double avg;
  for(i=1;i<20-1;i++){
    diff = list[i+1][0]-list[i][0];
    accum = accum +diff;
  }
  avg = (double)accum/20; 
  
  return avg;
  
  
}
double averagey(int16_t **list){
  int accum =0;
  int diff;
  int i;
  double avg;
  for(i=1;i<20-1;i++){
    diff = list[i+1][1]-list[i][1];
    accum = accum +diff;
  }
  avg = (double)accum/20; 
  
  return avg;
  
  
}
double averagez(int16_t **list){
  int accum =0;
  int diff;
  int i;
  double avg;
  for(i=1;i<20-1;i++){
    diff = abs(list[i+1][2]-list[i][2]);
    accum = accum +diff;
  }
  avg = (double)accum/20; 
  
  return avg;
  
  
}
int isavhigh(int16_t **list){
  double n=0;
  double avgx = averagex(list);
  double avgy = averagey(list);
  double avgz = averagez(list);
  
  if (avgx>n||avgy>n||avgz>n){
    return 1;
  }
  return 0;
}
int Pqualifier(int16_t **list){
  int qualify=0;
  int x = isavhigh(list);
  int y = isforce(averagex(list),averagey(list),averagez(list));
  if ((x|y)==1){
    qualify = 1;
    
  }
  
  /*if (qualify==1){
    period[counter]=1;
    
    counter++;
  }
  if (qualify==0){
    period[counter]=0;
    counter++;
  }*/
  return qualify;
}
int highsession(int16_t **list){
  
  mode = Pqualifier(list);
  
 
  onecount = onecount + Pqualifier(list);
  zerocount = zerocount + (~Pqualifier(list));
  
  if((last==mode)&&(mode==1)){
    consecutivehi = consecutivehi + Pqualifier(list);
  }
  if ((last==mode)&&(mode==0)){
    consecutivelo = consecutivelo + ~Pqualifier(list);
  }
  if ((last!=mode)&&(mode==1)){
    last = mode;
    consecutivehi =0;
    
  }
  if ((last!=mode)&&(mode==0)){
    last =mode;
    consecutivelo = 0;
    
  }
  if (consecutivelo>=5){
    onecount =0;
    return 0;
    
  }
  if (consecutivehi>=7){
    onecount = 0;
    zerocount =0;
    return 1;
  }
  if ((onecount==7)&&(consecutivelo<6)){
    onecount =0;
    
    return 1;
  }
  if ((zerocount==7)&&(consecutivehi)<6){
    onecount = 0;
    zerocount = 0;
    
    
    return 0;
    
  }
  else{
    return 0;
  }
      
}



  



