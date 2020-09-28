#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

/*Тверской А.(211);IPC_USER;Игра "Быки и Коровы"*/

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

int main (int argc,char **argv){
    key_t key;
    char *shmaddr;
    int semid,shmid,x,attempt=0;
    struct sembuf sops;
    int Usr[4];
    char str[sizeof(int)];

    key=ftok("/tmp",'S');
    semid=semget(key,1,0666|IPC_CREAT); //семафор
    shmid=shmget(key,sizeof(int),0666|IPC_CREAT); //разделяемая память
    shmaddr=shmat(shmid,NULL,0);
    semctl(semid,0,SETVAL,(int) 0);
    sops.sem_num=0;
    sops.sem_flg=0;
    while ((attempt<6)&&(str[0]!='*')){
      L1:printf("Your guess: ");
      scanf("%d",&x);
      if (int4toarr(Usr,x)==-1){printf("Invalid Value. Try again..\n"); goto L1;}
      strcpy(str,shmaddr);
      sprintf(str,"%d",x);
      strcpy(shmaddr,str);
      attempt++;
      sops.sem_op=3; //пускаем server
      semop(semid,&sops,1);
      sops.sem_op=0;
      semop(semid,&sops,1);//ждем server
      strcpy(str,shmaddr);
      if (str[0]=='*') {printf("Y O U  W I N\n");break;}
    }
    if (attempt>=6) printf("G A M E  O V E R\n");
    shmdt(shmaddr);
    shmctl(shmid,IPC_RMID,NULL);
    semctl(semid,0,IPC_RMID,(int) 0);
    return 0;
}
