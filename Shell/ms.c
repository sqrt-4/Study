#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXstr_size 128
#define MAXopt 8
#define MAXcmd 16
#define MAXelems 32


struct cmdCONV{
    char **argv;
    char *infile; //NULL если файла нет
    char *outfile; // куда вывод из конвейера
    int type_of_out; // 0 = > ; 1 = >>
    char * razdelitel; // что стоит после конвейра && или || или ничего
    struct cmdCONV *pipeline; 
    struct cmdCONV *next; // ссылка на следующую команду после && или ||
};

struct cmdCOND{
    struct cmdCONV *link;
    char *parametr; // то, что стоит после нашей команды с условным выполнением ; & или NULL в случае если ничего
};

char str[MAXstr_size];
char **parsing=NULL;
struct cmdCOND *main_table=NULL; // ссылка массив структур cmdCOND, то есть ссылка на таблицу команд с условным выполнением
int pos=1; // какой элемент нашего вектора parsing мы сейчас анализируем
int shell_cmd=0;//размер таблицы команд, то есть количество команд с условным выполнением в нашей строке
struct cmdCONV *cur_elem=NULL;
struct cmdCONV *new_elem;
char **argv;
pid_t pidLAST;

int check(void){
    char *elements[]={"&&","||",";","&","|",">",">>","<"};
    if (parsing[pos]==NULL)
        return 0;
    for (int j=0;j<8;j++){
        if (strcmp(parsing[pos],elements[j])==0)
            return 0;
    }
    return 1;
}

void conveer(void){
    argv=malloc(sizeof(char*)*MAXopt);
    int i=0;
    while (check()){ //проверка 
        argv[i]=parsing[pos];
        pos++;
        i++;
    }
    cur_elem->argv=argv;
    while (parsing[pos]!=NULL && strcmp(parsing[pos],"|")==0 && pos++){
        argv=malloc(sizeof(char*)*MAXopt);
        i=0;
        cur_elem->pipeline=malloc(sizeof(struct cmdCONV));
        cur_elem=cur_elem->pipeline;
        while (check()){
            argv[i]=parsing[pos];
            pos++;
            i++;
        }
        argv[i]=NULL;
        cur_elem->argv=argv;
    }
    cur_elem->pipeline=NULL;
}


void pipeline(void){
    new_elem=malloc(sizeof(struct cmdCONV)); // знак < file стоит перед конвейером, > otherfile после
    new_elem->next=NULL;
    if (strcmp(parsing[pos],"<")==0){
        pos++;
        new_elem->infile=parsing[pos];
        pos++;
    }
    cur_elem=new_elem;
    conveer(); // следить за cur_elem, массив параметров, NULL в pipe у последнего
    if (parsing[pos]==NULL)
        cur_elem->outfile=NULL;
    else if (strcmp(parsing[pos],">")==0){
        pos++;
        cur_elem->outfile=parsing[pos];
        cur_elem->type_of_out=0;
        pos++;
    }
    else if (strcmp(parsing[pos],">>")==0){
        pos++;
        cur_elem->outfile=parsing[pos];
        cur_elem->type_of_out=1;
        pos++;
    }
    else {
        cur_elem->outfile=NULL;
    }
}


void comand_cond(void){
    struct cmdCONV *buf;
    while (1){
        pipeline();
        if (main_table[shell_cmd-1].link==NULL)
            main_table[shell_cmd-1].link=new_elem;
        else {
            buf=main_table[shell_cmd-1].link;
            while (buf->next!=NULL)
                buf=buf->next;
            buf->next=new_elem;
        }
        if (parsing[pos]==NULL || strcmp(parsing[pos],"&")==0 || strcmp(parsing[pos],";")==0){
            new_elem->razdelitel=NULL;
            break;
        }
        else {
            new_elem->razdelitel=parsing[pos];
            pos++;
        }
    }
}


void buildLST(void){
    main_table=malloc(sizeof(struct cmdCOND)*MAXcmd);
    while (parsing[pos]!=NULL){
        shell_cmd++;
        main_table[shell_cmd-1].link=NULL;
        comand_cond();
        main_table[shell_cmd-1].parametr=parsing[pos];
        if (parsing[pos]!=NULL)
            pos++;
    }
}


void startCONV(struct cmdCONV *link){
    int in=dup(0);
    int out=dup(1);
    int fd[2];
    int vv_vd;
    if (link->infile!=NULL){
        vv_vd=open(link->infile,O_RDONLY);
        dup2(vv_vd,0);
        close(vv_vd);
    }
    while (link->pipeline!=NULL){
        pipe(fd);
        if (fork()==0){
            dup2(fd[1],1);
            close(fd[1]);
            close(fd[0]);
            execvp(link->argv[0],link->argv);
            printf("Can't execute %s\n",link->argv[0]);
            exit(-1); // если execvp не сработает
        }
        close(fd[1]);
        dup2(fd[0],0);
        close(fd[0]);
        link=link->pipeline;
    }
    if (link->outfile!=NULL){
        if (link->type_of_out==0) //значит >
            vv_vd=open(link->outfile,O_WRONLY | O_CREAT | O_TRUNC, 0666);
        else if (link->type_of_out==1)
            vv_vd=open(link->outfile,O_WRONLY | O_CREAT | O_APPEND, 0666);
        else {
            printf("Error with out.\n");
            exit(-1);
        }
        dup2(vv_vd,1);
        close(vv_vd);
    }
    if ((pidLAST=fork())==0){ //запоминаем pid посл-него эл-та конвеера, для определения что делать дальше если || или &&
        execvp(link->argv[0],link->argv);
        printf("Can't execute %s\n",link->argv[0]);
        exit(-1);
    }
    dup2(in,0); // восстанавливаем содержимое стандартный дескрипторов
    dup2(out,1);
    close(out);
    close(in);
}


void start_common(int number){
    int status;
    struct cmdCONV *next_one=main_table[number].link;
    while (next_one!=NULL){
        startCONV(next_one);
        waitpid(pidLAST,&status,0);
        while (wait(NULL)!=-1); // ждем, пока все команды конвеера не завершатся и обрабатываем их, чтобы не было процессов зомби
        if (next_one->razdelitel==NULL)
            break;
        if (strcmp(next_one->razdelitel,"&&")==0){
            if (status==0)
                next_one=next_one->next;
            else
                next_one=next_one->next->next;
        }
        else if (strcmp(next_one->razdelitel,"||")==0){
            if (status!=0)
                next_one=next_one->next;
            else
                next_one=next_one->next->next;
        }
    }
}

void start(void){
    int fd;
    for (int j=0;j<shell_cmd;j++){
        if (main_table[j].parametr==NULL || strcmp(main_table[j].parametr,";")==0)
            start_common(j);
        else if (strcmp(main_table[j].parametr,"&")==0){ //фоновый режим
            if (fork()==0){
                fd=open("/dev/null",O_RDONLY);
                dup2(fd,0);
                close(fd);
                setpgid(getpid(),0); //меняем номер группы,чтобы не приходили сигналы
                if (fork()==0){ //порождаем внука
                    start_common(j);
                    exit(0);//завершаем отца внука, теперь отец Init
                }
                else
                    exit(0);
            }
        }
    }
}

void renew(void){
    struct cmdCONV *link,*buf,*helper;
    for (int j=0;j<shell_cmd;j++){
        link=main_table[j].link;
        while (link!=NULL){
            buf=link->pipeline;
            while (buf!=NULL){ //очистка конвеера
                helper=buf;
                buf=buf->pipeline;
                free(helper);
            }
            buf=link;
            link=link->next;
            free(buf);
        }
    }
    free(parsing);
    free(main_table);
    parsing=NULL;
    main_table=NULL;
    pos=1;
    shell_cmd=0;
    cur_elem=NULL;
}

int main(){
    printf(">");
    while (fgets(str,MAXstr_size,stdin)!=NULL){
        str[strlen(str)-1]='\0';
        int i=1;
        char *newstr=strtok(str," ");
        parsing=malloc(sizeof(char *)*MAXelems);
        while (newstr!=NULL){
            parsing[i]=newstr;
            newstr=strtok(NULL," ");
            i++;
        }
        parsing[i]=NULL;
        buildLST(); //создание структуры для выполнения
        start();  //выполнение
        renew(); //освобождение памяти
        printf(">");
    }
    return 0;
}

