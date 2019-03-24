//
//  main.c
//  wc
//
//  Created by apple on 19/3/24.
//  Copyright © 2019年 apple. All rights reserved.
//

#include <stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdlib.h>
struct message{
    int words;
    int lines;
    long long sizes;
    int chars;
    int maxLineLength;
}wcRes;

void scanFile(char fileName[]){
    struct stat fileMessage ={};
    FILE *fp;
    if(stat(fileName,&fileMessage)==-1){
        printf("No Such File!");
        return;
    }
    mode_t mode = fileMessage.st_mode;
    if(S_ISDIR(mode)){
        printf("Error because file is Dir");
        return;
    }
    wcRes.sizes = fileMessage.st_size;
    fp = fopen(fileName, "r");
    char ch;
    int newWordflag = 0;
    int currentLineLength = 0;
    while((ch = fgetc(fp))!=EOF){
        wcRes.chars++;
        if(ch !='\n'){
            currentLineLength++;
        }
        if(ch == '\n'){
            wcRes.lines++;
            if(currentLineLength>wcRes.maxLineLength)
                wcRes.maxLineLength = currentLineLength;
            currentLineLength = 0;
            newWordflag = 1;
        }
        if(ch=='\t'||ch==' '){
            newWordflag = 1;
            continue;
        }
        
        if(newWordflag){
            wcRes.words++;
            newWordflag = 0;
            
        }
        
    }
    fclose(fp);
}



int main(int argc,  char * argv[]) {
    // insert code here...
    if(argc == 2){
        if(argv[1][0]!='-'){
            scanFile(argv[1]);
            printf("%d %d %llu %s\n",wcRes.lines,wcRes.words,wcRes.sizes,argv[1]);
            return 0;
        }
        else{
            printf("No File parameter!");
            return 0;
        }
    }
    else if(argc == 1){
        printf("please input insrtuctions!\n");
        return 0;
    }
    else if(argc == 3){
        scanFile(argv[2]);
    int num;
        while((num = getopt(argc, argv, "lwmcL"))!=-1){
            switch (num) {
                case 'l':
                    printf("%d\t",wcRes.lines);
                    break;
                case 'w':
                    printf("%d\t",wcRes.words);
                    break;
                case 'm':
                    printf("%d\t",wcRes.chars);
                    break;
                case 'c':
                    printf("%llu\t",wcRes.sizes);
                    break;
                case 'L':
                    printf("%d\t",wcRes.maxLineLength);
                    break;
            }
        }
        printf("\n");
        return 0;
    }

    printf("args error");
    return 0;
}
