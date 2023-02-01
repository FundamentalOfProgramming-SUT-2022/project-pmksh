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
#define N 1000000//input size limit

//end of global variables

//start of declaring functions



// end of declaring functions

//start of structs
struct linkedlist{
    char *c;
    long long len;
    long long pl;
    int mode;
    struct linkedlist *next;
    struct linkedlist *prev;
};

struct linkedlist* create_linkedlist(){
    struct linkedlist *list=(struct linkedlist *)malloc(sizeof(struct linkedlist));
    list->len=0;
    list->next= NULL;
    list->prev= NULL;
    list->mode=0;
    list->pl=-1;
    return list;
};

void add(struct linkedlist* list, char text[], int len, int pl, int mode){
    while(list->next!=NULL){
        list=list->next;
    }
    list->next=create_linkedlist();
    list->next->c=calloc(len,sizeof(char));
    for(int i=0; i<len; i++){
        *(list->next->c+i)=text[i];
    }
    list->next->prev=list;
    list->next->len=len;
    list->next->pl=pl;
    list->next->mode=mode;
}

void movelist(struct linkedlist *list, int n){
    if(n>0){
        for(int i=0; i<n; i++){
            list=list->next;
        }
    }
    if(n<0){
        for(int i=0; i<-n; i++){
            list=list->prev;
        }
    }
    return;
}
//end of structs


//start of functions

void fill_file(char name1[],char name2[]){
    FILE *file1=fopen(name1, "r");
    FILE *file2=fopen(name2, "w");
    char c=fgetc(file1);
    while(c!=EOF){
        putc(c, file2);
        c=fgetc(file1);
    }
    fclose(file1);
    fclose(file2);
}


void mem_wordtochar(char name[]){
    FILE *file=fopen(name,"r");
    FILE *fmem=fopen("fmem.txt","r");
    FILE *mem2=fopen("mem2.txt","w");
    long long chartype=0, wcount=0, i=0, word;
    int ex=fscanf(fmem,"%lld", &word);
    char c=fgetc(file);
    while(ex>0){
        i++;
        if((chartype==0)&&(c!='\n')&&(c!=' ')){
            chartype=1;
            wcount+=1;
        }
        else if((chartype==1)&&((c=='\n')||(c==' '))){
            chartype=0;
        }
        if (wcount==word){
            fprintf(mem2,"%lld ", i);
            ex=fscanf(fmem,"%lld", &word);
        }
        c=getc(file);
    }
    fclose(file);
    fclose(fmem);
    fclose(mem2);
    fill_file("mem2.txt","fmem.txt");
    return;
}

long long wordtochar(char name[], int word){
    FILE *file=fopen(name,"r");
    int chartype=0, wcount=0, i=0;
    char c=fgetc(file);
    while(c!=EOF){
        i++;
        if((chartype==0)&&(c!='\n')&&(c!=' ')){
            chartype=1;
            wcount+=1;
        }
        else if((chartype==1)&&((c=='\n')||(c==' '))){
            chartype=0;
        }
        if (wcount==word){
            fclose(file);
            return i;
        }
        c=getc(file);
    }
    fclose(file);
    return -1;
}


long long standardize(char name[], int line, int dep){
    FILE *file=fopen(name,"r");
    int newline_counter=0, lastline=-1;
    char c;
    long long i=0;
    while(1){
        i++;
        c=fgetc(file);
        if(c==EOF){
            fclose(file);
            return -1;;
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
    }
    if (line>1){
        i++;
    }
    fclose(file);
    return i-2;
}

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

int insertstr(char name[], char text[], int place){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place==-1){
        printf("no such position");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    FILE *part2w=fopen("part2.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    for(int i=0; i<place; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    while(c!=EOF){
        c=fgetc(file);
        putc(c, part2w);
    }
    fclose(part1w);
    fclose(part2w);
    fclose(file);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    FILE *part2r=fopen("part2.txt","r");
    FILE *mem=fopen("mem.txt","w");
    fprintf(mem,"%d\n%d",3,place);
    fclose(mem);
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
    fill_file("mem.txt","permem.txt");
    return 1;
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

//mode ghable vooroodi baresi mishavad va voroodi agar -b bood place-size+1 dadeh mishavad
//baraye removestr copystr cutstr pastestr hanooz anjam nashode
//khar yadet nare
//gav
int removestr(char name[], int place, int size){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place<0){
        printf("no such position");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    FILE *mem=fopen("mem.txt","w");
    fprintf(mem,"%d\n%d %d\n",4,place,size);
    for(int i=0; i<place-1; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    for(int i=0; i<size; i++){
        c=fgetc(file);
        if(c==EOF){
            printf("no such position");
            fclose(part1w);
            fclose(file);
            fclose(mem);
            return 0;
        }
        putc(c, mem);
    }
    while((c!=EOF)){
        c=fgetc(file);
        if(c==EOF){
            break;
        }
        putc(c, part1w);
    }
    fclose(part1w);
    fclose(file);
    fclose(mem);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    c='a';
    while((c!=EOF)){
        c=fgetc(part1r);
        if(c==EOF){
            break;
        }
        putc(c,filew);
    }
    fclose(filew);
    fclose(part1r);
    fill_file("mem.txt","permem.txt");
    return 1;
}

int copystr(char name[], int place, int size){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place<0){
        printf("no such position");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    FILE *mem=fopen("mem.txt","w");
    FILE *tempboard=fopen("tempboard.txt","w");
    fprintf(mem,"%d\n%d %d\n",5,place,size);
    for(int i=0; i<place-1; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    for(int i=0; i<size; i++){
        c=fgetc(file);
        if(c==EOF){
            printf("no such position");
            fclose(mem);
            fclose(tempboard);
            fclose(file);
            return 0;
        }
        putc(c, mem);
        putc(c, tempboard);
    }
    while((c!=EOF)){
        c=fgetc(file);
        if(c==EOF){
            break;
        }
        putc(c, part1w);
    }
    fclose(part1w);
    fclose(file);
    fclose(mem);
    fclose(tempboard);
    fill_file("tempboard.txt","clipboard.txt");
    fill_file("mem.txt","permem.txt");
    return 1;
}

int cutstr(char name[], int place, int size){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place<0){
        printf("no such position");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    FILE *mem=fopen("mem.txt","w");
    FILE *tempboard=fopen("tempboard.txt","w");
    fprintf(mem,"%d\n%d %d\n",6,place,size);
    for(int i=0; i<place-1; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    for(int i=0; i<size; i++){
        c=fgetc(file);
        if(c==EOF){
            printf("no such position");
            fclose(mem);
            fclose(tempboard);
            fclose(part1w);
            fclose(file);
            return 0;
        }
        putc(c, mem);
        putc(c, tempboard);
    }
    while((c!=EOF)){
        c=fgetc(file);
        if(c==EOF){
            break;
        }
        putc(c, part1w);
    }
    fclose(part1w);
    fclose(file);
    fclose(mem);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    c='a';
    while((c!=EOF)){
        c=fgetc(part1r);
        if(c==EOF){
            break;
        }
        putc(c,filew);
    }
    fclose(filew);
    fclose(part1r);
    fclose(tempboard);
    fill_file("tempboard.txt","clipboard.txt");
    fill_file("mem.txt","permem.txt");
    return 1;
}

int pastestr(char name[], int place){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place==-1){
        printf("no such position");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *pst=fopen("clipboard.txt","r");
    FILE *part1w=fopen("part1.txt","w");
    FILE *part2w=fopen("part2.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    for(int i=0; i<place; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    while(c!=EOF){
        c=fgetc(file);
        putc(c, part2w);
    }
    fclose(part1w);
    fclose(part2w);
    fclose(file);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    FILE *part2r=fopen("part2.txt","r");
    FILE *mem=fopen("mem.txt","w");
    fprintf(mem,"%d\n%d",3,place);
    fclose(mem);
    c=fgetc(part1r);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(part1r);
    }
    c=fgetc(pst);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(pst);
    }
    c=fgetc(part2r);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(part2r);
    }
    fclose(part1r);
    fclose(part2r);
    fclose(filew);
    fclose(pst);
    fill_file("mem.txt","permem.txt");
    return 1;
}


int moder(char str[], int len){
    if(str[0]=='*'){
        return 1;
    }
    if((str[len-1]=='*')&&((len==1)||(str[len-2]!='\\'))){
        return 2;
    }
    return 0;
}


char *cleaner(char str[], int len, int mode){
    int n=len-!(mode==0);
    int m=(mode==1);
    char *a=calloc(n,sizeof(char));
    int count=0;
    for(int i=0; i<n; i++){
        if(str[i+m]=='\\'){
            if((i+m<n-1)&&(str[i+m+1]=='*')){
                *(a+count)='*';
                i++;
            }
            else{
                *(a+count)='\\';
            }
            count++;
        }
        else{
            *(a+count)=str[i+m];
            count++;
        }
    }
    return a;
}

//mode: 0=a, 1=*a, 2=a*
int comparator(char a[], char b[], int m){
    int la=strlen(a), lb=strlen(b);
    if (m==0){
        if (strcmp(a,b)==0){
            return 1;
        }
        return 0;
    }
    if(la>lb){
        return 0;
    }
    for(int i=0;i<la;i++){
        int ra=(m==2)*i+(m==1)*(la-1-i);
        int rb=(m==2)*i+(m==1)*(lb-1-i);
        if(a[ra]!=b[rb]){
            return 0;
        }
    }
    return 1;
}

int grepper(char a[],char b[]){
    long long la=strlen(a), lb=strlen(b);
    int c=0;
    for(int i=0;i<=lb-la; i++){
        c=0;
        for(int j=0; j<la;j++){
            if(a[j]!=b[j+i]){
                c=1;
                break;
            }
        }
        if(c==0){
            return 1;
        }
    }
    return 0;

}

void find_init(char name[], char text[]){
    int len=strlen(text);
    FILE *file=fopen(name,"r");
    FILE *help=fopen("part1.txt","w");
    for(int i=0; i<len; i++){
        putc(text[i], help);
    }
    struct linkedlist *check=create_linkedlist(), *src=create_linkedlist();
    fclose(help);
    FILE *part1r=fopen("part1.txt","r");
    int fcount=0, ccount=0;
    char a[N];
    int ex=fscanf(part1r, "%s", a);
    while(ex>0){
        ccount++;
        int mm=moder(a,strlen(a));
        strcpy(a,cleaner(a,strlen(a),mm));
        add(check,a,strlen(a),-1,mm);
        ex=fscanf(part1r, "%s", a);
    }
    char b[N];
    ex=fscanf(file, "%s", b);
    while(ex>0){
        fcount++;
        strcpy(b,cleaner(b,strlen(b),0));
        add(src,b,strlen(b), fcount, 0);
        ex=fscanf(file, "%s", b);
    }
    fclose(file);
    fclose(help);
    FILE *fmem=fopen("fmem.txt","w");
    src=src->next;
    check=check->next;
    for(int i=0; i<=fcount-ccount; i++){
        int amogus=0;
        struct linkedlist *aaa=check, *bbb=src;
        int khar=0;
        for(int j=0; j<ccount; j++){
            if(comparator(aaa->c,bbb->c,aaa->mode)==0){
                amogus=1;
                break;
            }
            aaa=aaa->next;
            bbb=bbb->next;
        }
        if(amogus==0){
            fprintf(fmem,"%lld ",src->pl);
        }
        src=src->next;
    }
    fclose(fmem);
    return;
}

//mode: 0=aadi, 1=count, 2=at, 3=all
//outtype: 0=char, 1=word
// agar count nabashad, ifcount=0 voroodi migirim
void find_exe(char name[], int mode, int outtype, int ifat){
    FILE *fmem=fopen("fmem.txt","r");
    long long n, count=0, ex=0;
    if(outtype==0){
        mem_wordtochar(name);
    }
    ex=fscanf(fmem,"%lld", &n);
    if(ex<=0){
        printf("no such expression\n");
        fclose(fmem);
        return;
    }
    while(ex>0){
        count++;
        if(mode==0){
            printf("%lld\n", n);
            return;
        }
        else if(mode==2){
            if(count==ifat){
                printf("%lld\n", n);
                fclose(fmem);
                return;
            }
        }
        else if(mode==3){
            printf("%lld\n", n);
        }
        ex=fscanf(fmem,"%lld", &n);
    }
    fclose(fmem);
    if(mode==2){
        printf("expression appears fewer times");
        return;
    }
    if(mode==1){
        printf("%lld", count);
        return;
    }
    return;

}


//mode: 0=at, 1=all;
//text1=ooni ke bar midaarim
//text2=ooni ke jash mizarim
void replace_exe(char name[], int mode, int ifat, char text1[], char text2[]){
    mem_wordtochar(name);
    FILE *fmem=fopen("fmem.txt","r");
    long long n, count=0, shift=0, slide=strlen(text2)-strlen(text1), l1=strlen(text1), l2=strlen(text2);
    printf("%d\n", l1);
    int ex=fscanf(fmem,"%d", &n);
    if(ex<=0){
        printf("no such expression\n");
        return;
    }
    while(ex>0){
        printf("aaa%d", n);
        count++;
        printf("%d\n", l1);
        if(mode==0){
            if(count==ifat){
                removestr(name,shift+n, l1);
                insertstr(name,text2,shift+n-1);
                fclose(fmem);
                return;
            }
        }
        else if(mode==1){
            removestr(name,n,l1);
            insertstr(name,text2,n-1);
            shift+=slide;
        }
        ex=fscanf(fmem,"%d", &n);
        n+=shift;
    }
    if(mode==0){
        printf("expression appears fewer times");
    }
    fclose(fmem);
    return;
}



// dar voroodi gereftan file ha, esm tamam file ha ro too grepfiles.txt mmizarim
//ba in tabeh:
void grep_init(){}


//mode: 0=print all lines, 1=count, 2=print all files
void grep_exe(char pattern[], int mode){
    FILE *names=fopen("grepfiles.txt","r");
    char name[N], line[N];
    int ex=fscanf(names,"%[^\n]s", name);
    fgetc(names);
    int ex2=1;
    long long count=0;
    while(ex>0){
        int filecheck=0;
        FILE *now=fopen(name,"r");
        ex2=fscanf(now,"%[^\n]s", line);
        fgetc(now);
        while(ex2>0){
            if (grepper(pattern, line)){
                count++;
                if(mode==0){
                    printf("%s: %s\n", name, line);
                }
                if((mode==2)&&(filecheck==0)){
                    printf("%s\n", name);
                    filecheck=1;
                }
            }
            ex2=fscanf(now,"%[^\n]s", line);
            fgetc(now);
        }
        ex=fscanf(names,"%[^\n]s", name);
        fgetc(names);
    }
    if(mode==1){
        printf("%d", count);
    }
    return;
}



// end of functions


int main(){
    printf("Powered by <<Bario>>\n");
    printf("This program is made for the windows operating system\nplease test on windows\n\n");
    char a[100];
    char b[]="./root/amo.txt";
    char s[]="crazy";
    char t[]="boe jiden";
    grep_exe("j",2);
    //find_init(b,s);
    //replace_exe(b,0,2,s,t);
}
