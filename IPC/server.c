#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

/*Тверской А.(211);IPC_SERVER;Игра "Быки и Коровы"*/

void generator(int *arr){    //генерация числа для отгадывания
  int x;
  srand(time(NULL));
  arr[0]=rand()%6+1;
  while ((x=rand()%6+1)==arr[0]);
  arr[1]=x;
  while (((x=rand()%6+1)==arr[0])||(x==arr[1]));
  arr[2]=x;
  while (((x=rand()%6+1)==arr[0])||(x==arr[1])||(x==arr[2]));
  arr[3]=x;
  return;
}

void print4arr(int *arr){           //печать массива из 4-х элементов
  for (int i=0;i<4;i++) printf("[%d] ",arr[i]);
  printf("\n");
  return;
}

int int4toarr(int *rez,int data){
    int x;
    for (int i=0;i<4;i++){
      if (data==0) return -1;
      x=data%10;
      rez[3-i]=x;
      data=data/10;
    }
    if (data!=0) return -1;
    for (int i=0;i<4;i++)
      for (int j=0;j<4;j++){
        if ((rez[i]==rez[j])&&(i!=j)) return -1;
        if ((rez[j]>6)||(rez[j]<1)) return -1;
      }
    return 0;
  }

int cowBULL(int *arr1,int *arr2,int *arr3){    //механика игры
  int k=0,win=0,bulls=0;
  for (int j=0;j<4;j++)
    for (int i=0;i<4;i++){
      if ((arr1[j]==arr2[i])&&(i==j)) {arr3[i]=arr1[j];++win;bulls=1;}
      else if (arr1[j]==arr2[i]) k++;
    }
  if (bulls) {print4arr(arr3);} else if (k) {printf("bulls:0\n");}
  else printf("-\n");
  for (int i=0;i<4;i++) arr3[i]=0;
  if (win==4) return 1;
  if (k) {printf("Cows:%d\n",k);} else if (bulls) printf("Cows:0\n");
  return 0;
}


int main (int argc,char **argv){
    key_t key;
    char *shmaddr;
    int semid,shmid,x,attempt=0;
    struct sembuf sops;
    int Serv[4]; //число,загадонное сервером
    int Usr[4];  //предположение клиента
    int Rez[4]={0,0,0,0};  //результат
    char str[sizeof(int)];  //передача в общую память

    generator(Serv); //генерируем число
    key=ftok("/tmp",'S');
    semid=semget(key,1,0666|IPC_CREAT); //создаем семафор
    shmid=shmget(key,sizeof(int),0666|IPC_CREAT); //создаем область разделяемой памяти
    shmaddr=shmat(shmid,NULL,0); //присоединяем полученную р.п. к адресному пр-ву процесса
    semctl(semid,0,SETVAL,(int) 0); //инициализируем семафор = 0
    sops.sem_num=0;
    sops.sem_flg=0;

    while ((attempt<6)&&(str[0]!='*')){
      printf("waiting..\n");
      sops.sem_op=-2;
      semop(semid,&sops,1);//ждем  User
      strcpy(str,shmaddr);
      x=atoi(str);
      attempt++;
      if (int4toarr(Usr,x)==-1){printf("Error:data\n");return -1;}
      if (cowBULL(Usr,Serv,Rez)){
        strcpy(str,"*");
        strcpy(shmaddr,str);
      } else{
        strcpy(str,"&");
        strcpy(shmaddr,str);
      }
      sops.sem_op=-1;
      semop(semid,&sops,1);//пускаем User
    }
  printf("ANSWER:");
  print4arr(Serv);
  return 0;
}
