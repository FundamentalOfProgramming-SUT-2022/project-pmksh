#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif


//end of inclusions

//start of global variables
#define N 256//input size limit

//end of global variables

//start of declaring functions




// end of declaring functions

//start of functions

char *fix_name(char name[]){
    char buf[N];
    snprintf(buf, sizeof(buf), ".%s", name);
    strcpy(name, buf);
    return name;
}

int checkexdir(char address[]){
    DIR* dir = opendir(address);
    if (dir) {
        return 1;
        closedir(dir);
    }
    if (ENOENT == errno) {
        return 0;
    }
}

int checkexfile(char address[]){
    FILE *file;
    if((file = fopen(address,"r"))!=NULL){
        fclose(file);
        return 1;
    }
    else {
        return 0;
    }
}

int *pathmaker(char path[], int pathdash[]){
    int count=0;
    for(int i=0; i<N; i++){
        if(path[i]=='/'){
            count+=1;
            pathdash[count]=i;
        }
    }
    int pathlen=count;
    return pathdash;
}

//int makedir(char name)

int createfile(char name[]){
    if(checkexfile(name)==1){
        printf("file already exists\n");
        return 0;
    }
    int pathdash[N+1]={0};
    pathmaker(name, pathdash);
    int i=1;
    while(pathdash[i]!=0){
        char cur_path[N];
        strncpy(cur_path,name, pathdash[i]);
        if(checkexdir(cur_path)==0){
            mkdir(cur_path);
        }
        i++;
    }
    FILE *f = fopen(name, "w");
    fclose(f);
    return 1;
}

int insertstr(char name[], char text[], int line, int dep){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    int endcheck=0;
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    FILE *part2w=fopen("part2.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    while(1){
        c=fgetc(file);
        if(c==EOF){
            endcheck=1;
            break;
        }
        if(c=='\n'){
            newline_counter++;
        }
        if(newline_counter==line-1){
            lastline++;
        }
        if((lastline==dep+1)){
            break;
        }
        putc(c, part1w);
    }
    if(endcheck==1){
        while(newline_counter<line-1){
            putc('\n', part1w);
            newline_counter++;
        }
        while(lastline<dep-1){
            putc(' ', part1w);
            lastline++;
        }
    }
    while(c!=EOF){
        putc(c, part2w);
        c=fgetc(file);
    }
    fclose(part1w);
    fclose(part2w);
    fclose(file);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    FILE *part2r=fopen("part2.txt","r");
    c=fgetc(part1r);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(part1r);
    }
    for(int i=0; i<strlen(text); i++){
        putc(text[i],filew);
    }
    c=fgetc(part2r);
    while(c!=EOF){
        putc(c, filew);

        c=fgetc(part2r);
    }
    fclose(part1r);
    fclose(part2r);
    fclose(filew);
}

int cat(char name[]){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    FILE *file=fopen(name,"r");
    char c=fgetc(file);
    while(c!=EOF){
        printf("%c", c);
        c=fgetc(file);
    }
    fclose(file);
    return 1;
}

// end of functions


int main(){
    printf("Powered by <<Bario>>\n");
    printf("This program is made for the windows operating system(linux batel)\nplease test on windows\n\n");
    char a[100];
    //scanf("%s", &a);
    //createfile(a);
    char b[]="./root/amo.txt";
    //cat(b);
}
