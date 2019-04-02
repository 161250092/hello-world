//
//  main.c
//  dir
//
//  Created by apple on 19/4/2.
//  Copyright © 2019年 apple. All rights reserved.
//

#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

void showFileInfo(char* fileName,struct stat* st_p){
    char filetype[11];
    if(S_ISDIR(st_p->st_mode))
    {
        char temp[] = "文件夹";
        strcpy(filetype,temp);
    }
    
    else if(S_ISREG(st_p->st_mode)){
        char temp[] = "文件";
        strcpy(filetype, temp);
    }
    
    printf(" %s",fileName);
    printf(" %s  ",filetype);
    
}
void ls(const char dirName[]){
    DIR* dir_ptr;
    struct dirent* dirent_ptr;
    dir_ptr = opendir(dirName);
    while((dirent_ptr = readdir(dir_ptr))!=NULL){
        char fullName[200];
        strcpy(fullName,dirName);
        strcat(fullName,"/");
        strcat(fullName,dirent_ptr->d_name);
        
        struct stat info;
        stat(fullName,&info);
        showFileInfo(dirent_ptr->d_name,&info);
    
    }
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    ls(".");
    return 0;
}
