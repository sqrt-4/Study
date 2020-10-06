#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sys/dir.h>
#include <unistd.h>

#define MAX_PATH 1024


char hname[MAX_PATH];
char drname[MAX_PATH];
char dname[MAX_PATH];


int fa=0,fl=0,fr=0,flag=0,k=0;

void dirwalk(char *dname,void (*fcn)(char *,int,int,int)){
char name[MAX_PATH];

  struct dirent *dp;
  struct stat buf;
  DIR *dfd;
  stat(dname,&buf);
   if ((dfd=opendir(dname))==NULL){
    fprintf(stderr,"dirwalk: can't open %s\n",dname);
    return;
   }

  if (fr) printf("%s:\n",dname);
  while ((dp=readdir(dfd))!=NULL){
        sprintf(name,"%s/%s%c",dname,dp->d_name,'\0');
        stat(name,&buf);
        if ((fa)&&(fl)){
          printf((S_ISDIR(buf.st_mode))  ? "d" : "-"); //directory
          printf((buf.st_mode & S_IRUSR) ? "r" : "-"); //read,owner
          printf((buf.st_mode & S_IWUSR) ? "w" : "-"); //write,owner
          printf((buf.st_mode & S_IXUSR) ? "x" : "-"); //execute/search,owner
          printf((buf.st_mode & S_IRGRP) ? "r" : "-"); //read,group
          printf((buf.st_mode & S_IWGRP) ? "w" : "-"); //write,group
          printf((buf.st_mode & S_IXGRP) ? "x" : "-"); //execute/search,group
          printf((buf.st_mode & S_IROTH) ? "r" : "-"); //read,others
          printf((buf.st_mode & S_IWOTH) ? "w" : "-"); //write,others
          printf((buf.st_mode & S_IXOTH) ? "x " : "- "); //execure/search,others
          printf("%lu ",buf.st_nlink);//links
          printf("%u ",buf.st_gid);//user
          printf("%lu ",buf.st_rdev);//group
          printf("%ld ",buf.st_size);
          printf("%lu ",buf.st_ctime);
          printf("%s\n",dp->d_name);
        }
        else if (fa) {printf("%s\n",dp->d_name);}
        else if ((fl)&&((dp->d_name)[0]!='.')){
          printf((S_ISDIR(buf.st_mode))  ? "d" : "-"); //directory
          printf((buf.st_mode & S_IRUSR) ? "r" : "-"); //read,owner
          printf((buf.st_mode & S_IWUSR) ? "w" : "-"); //write,owner
          printf((buf.st_mode & S_IXUSR) ? "x" : "-"); //execute/search,owner
          printf((buf.st_mode & S_IRGRP) ? "r" : "-"); //read,group
          printf((buf.st_mode & S_IWGRP) ? "w" : "-"); //write,group
          printf((buf.st_mode & S_IXGRP) ? "x" : "-"); //execute/search,group
          printf((buf.st_mode & S_IROTH) ? "r" : "-"); //read,others
          printf((buf.st_mode & S_IWOTH) ? "w" : "-"); //write,others
          printf((buf.st_mode & S_IXOTH) ? "x " : "- "); //execure/search,others
          printf("%lu ",buf.st_nlink);//links
          printf("%u ",buf.st_gid);//user
          printf("%lu ",buf.st_rdev);//group
          printf("%ld ",buf.st_size);
          printf("%lu ",buf.st_ctime);
          printf("%s\n",dp->d_name);
        }
        else if ((dp->d_name)[0]!='.') printf("%s\n",dp->d_name);
}
  closedir(dfd);
  if (fr){
  if ((dfd=opendir(dname))==NULL){
    fprintf(stderr,"dirwalk: can't open %s\n",dname);
    return;
  }
  while ((dp=readdir(dfd))!=NULL){
    struct stat buf;
    sprintf(name,"%s/%s%c",dname,dp->d_name,'\0');
    stat(name,&buf);
    if ((fa)&&((buf.st_mode&S_IFMT)==S_IFDIR)&&(strcmp(dp->d_name,"..")!=0)&&(strcmp(dp->d_name,".")!=0)&&(strcmp(drname,name)!=0)&&(strcmp(hname,drname)!=0)) {
      sprintf(drname,"%s%c",name,'\0');
      (*fcn)(drname,fa,fl,fr);} else
    if (((buf.st_mode&S_IFMT)==S_IFDIR)&&((dp->d_name)[0]!='.')&&(strcmp(hname,drname)!=0)) {
      sprintf(drname,"%s%c",name,'\0');
      (*fcn)(drname,fa,fl,fr);}
  }
  }

    return;
} //dirwalk


void ls(char *sname,int fa,int fl,int fr){
  DIR *dfd;
  struct stat buf;
  stat(sname,&buf);
  if ((!k)&&((buf.st_mode&S_IFMT)==S_IFDIR)) {k=1;dirwalk(sname,ls);}
  else{
  if (!k){
      if ((fa)&&(fl)){
        printf((S_ISDIR(buf.st_mode))  ? "d" : "-"); //directory
        printf((buf.st_mode & S_IRUSR) ? "r" : "-"); //read,owner
        printf((buf.st_mode & S_IWUSR) ? "w" : "-"); //write,owner
        printf((buf.st_mode & S_IXUSR) ? "x" : "-"); //execute/search,owner
        printf((buf.st_mode & S_IRGRP) ? "r" : "-"); //read,group
        printf((buf.st_mode & S_IWGRP) ? "w" : "-"); //write,group
        printf((buf.st_mode & S_IXGRP) ? "x" : "-"); //execute/search,group
        printf((buf.st_mode & S_IROTH) ? "r" : "-"); //read,others
        printf((buf.st_mode & S_IWOTH) ? "w" : "-"); //write,others
        printf((buf.st_mode & S_IXOTH) ? "x " : "- "); //execure/search,others
        printf("%lu ",buf.st_nlink);//links
        printf("%u ",buf.st_gid);//user
        printf("%lu ",buf.st_rdev);//group
        printf("%ld ",buf.st_size);
        printf("%lu ",buf.st_ctime);
        printf("%s\n",sname);
      }
      else if (fa) {printf("%s\n",sname);}
      else if ((fl)&&(sname[0]!='.')){
        printf((S_ISDIR(buf.st_mode))  ? "d" : "-"); //directory
        printf((buf.st_mode & S_IRUSR) ? "r" : "-"); //read,owner
        printf((buf.st_mode & S_IWUSR) ? "w" : "-"); //write,owner
        printf((buf.st_mode & S_IXUSR) ? "x" : "-"); //execute/search,owner
        printf((buf.st_mode & S_IRGRP) ? "r" : "-"); //read,group
        printf((buf.st_mode & S_IWGRP) ? "w" : "-"); //write,group
        printf((buf.st_mode & S_IXGRP) ? "x" : "-"); //execute/search,group
        printf((buf.st_mode & S_IROTH) ? "r" : "-"); //read,others
        printf((buf.st_mode & S_IWOTH) ? "w" : "-"); //write,others
        printf((buf.st_mode & S_IXOTH) ? "x " : "- "); //execure/search,others
        printf("%lu ",buf.st_nlink);//links
        printf("%u ",buf.st_gid);//user
        printf("%lu ",buf.st_rdev);//group
        printf("%ld ",buf.st_size);
        printf("%lu ",buf.st_ctime);
        printf("%s\n",sname);
      }
      else if (sname[0]!='.') printf("%s\n",sname);
      return;
    }
  }
  if ((fa)&&(fr)&&((buf.st_mode&S_IFMT)==S_IFDIR)&&(strcmp(hname,sname)!=0)) {
    dirwalk(sname,ls);
  } else
  if ((fr)&&((buf.st_mode&S_IFMT)==S_IFDIR)&&(strcmp(hname,sname)!=0)){
    dirwalk(sname,ls);
  }
  return;
 }


int main(int argc,char *argv[]){

    for (int i=1;i<argc;i++){
    if ((strlen(argv[i])==2)&&(argv[i][0]=='-')){
        switch(argv[i][1]){
            case 'a':{fa=1;break;}
            case 'l':{fl=1;break;}
            case 'R':{fr=1;break;}
            deafult:{break;}
        }
       }
    }
     if ((argv[argc-1][0]!='-')&&(argc>1)){sprintf(hname,"%s",argv[argc-1]);sprintf(dname,"%s",hname);ls(dname,fa,fl,fr);}
     else {sprintf(hname,"%s",".");sprintf(dname,"%s",hname);ls(dname,fa,fl,fr);}
   return 0;
}
