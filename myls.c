//
//  main.c
//  myls
//
//  Created by apple on 19/3/24.
//  Copyright © 2019年 apple. All rights reserved.
//




 /*
  文件信息结构体
  */
//struct stat{
//    //dev_t st_dev;/*设备id号*/
//    //ino_t st_ino;/*i节点号*/
//    mode_t st_mode;/*权限与文件类型*/
//    nlink_t st_nlink;/*硬链接数*/
//    uid_t st_uid;/*用户id*/
//    ggid_t st_gid;/*所在组id*/
//    //dev_t st_rdev;/*设备id，对于特殊文件才有*/
//    off_t st_size;/*大小，较为常用*/
//    //blksize_t st_blocks;/*blocksize for file system I/O*/
//    //blkcnt_t st_blksize;/*number of 512B blocks allocated*/
//    //time_t st_atime;/*最后的访问时间*/
//    time_t st_mtime;/*最后的修改时间，较为常用*/
//    //time_t st_ctime;/*最后的状态改变时间*/
//}


/*
 目录信息结构体
 struct dirent{
 ino_t d_ino;//子项的i节点
 off_t d_off;//节点的偏移量
 unsigned short d_reclen;//长度
 unsigned char d_type;//子项类型（常用）
 char d_name[256];//子文件名（常用）
 
 };

 */

/*
 宏函数
 S_ISREG(m)  is it a regular file?  判断是否是普通文件
 S_ISDIR(m)  directory?  判断是否是目录
 S_ISCHR(m)  character device?  判断是否是字符设备
 S_ISBLK(m)  block device?  判断是否是块设备
 S_ISFIFO(m) FIFO (named pipe)? 判断是否是管道文件
 S_ISLNK(m)  symbolic link? (Not in POSIX.1-1996.) 判断使是否是符号链接（软连接）
 S_ISSOCK(m) socket? (Not in POSIX.1-1996.) 判断是否是SOCKET文件
 */


/* -l 以长格式显示目录下的内容列表。输出的信息从左到右依次包括文件名，文件类型、权限模式、硬连接数、所有者、组、文件大小和文件的最后修改时间等*/

/* -d：仅显示目录名，而不显示目录下的内容列表。显示符号链接文件本身，而不显示其所指向的目录列表  */

/* -a：显示所有档案及目录（ls内定将档案名或目录名称为“.”的视为影藏，不会列出） */

/* -i：显示文件索引节点号（inode）。一个索引节点代表一个文件；  */

/* -R：递归处理，将指定目录下的所有文件及子目录一并处理 */




#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define ls_none 0
#define ls_l 101
#define ls_R 102
#define ls_d 103
#define ls_i 104


#define ls_a 200

void showFileInfo(char* fileName,struct stat* st_p);
void fileTypeAndPermissions(int mode,char str[]);
char* getNameFromUid(uid_t uid);
char* getNameFromGid(gid_t gid);
void ls(const char dirName[],int param);



void showFileInfo(char* fileName,struct stat* st_p){
//    char *ctime();
    char fileType[11];
    fileTypeAndPermissions(st_p->st_mode, fileType);
    printf(" %s",fileType);
    printf(" %4d",(int)st_p->st_nlink);
    printf(" %-8s",getNameFromUid(st_p->st_uid));
    printf(" %-8s",getNameFromGid(st_p->st_gid));
    printf(" %8ld",(long) st_p->st_size);
    printf(" %.12s",ctime(&st_p->st_mtime));
    printf(" %s\n",fileName);
    
}

void fileTypeAndPermissions(int mode,char str[]){
    strcpy(str, "----------");
    if (S_ISDIR(mode))
        str[0] = 'd';
    else if (S_ISCHR(mode))
        str[0] = 'c';
    else if (S_ISBLK(mode))
        str[0] = 'b';
    else if(S_ISREG(mode))
         str[0] = '-';
    else if(S_ISFIFO(mode))
        str[0] = 'p';
    else if(S_ISLNK(mode))
        str[0] = 'l';
    else if(S_ISSOCK(mode))
        str[0] = 's';
    
    if ((mode & S_IRUSR))
        str[1] = 'r';
    if ((mode & S_IWUSR))
        str[2] = 'w';
    if ((mode & S_IXUSR))
        str[3] = 'x';
    if ((mode & S_IRGRP))
        str[4] = 'r';
    if ((mode & S_IWGRP))
        str[5] = 'w';
    if ((mode & S_IXGRP))
        str[6] = 'x';
    if ((mode & S_IROTH))
        str[7] = 'r';
    if ((mode & S_IWOTH))
        str[8] = 'w';
    if ((mode & S_IXOTH))
        str[9] = 'x';

}


char* getNameFromUid(uid_t uid){
    struct passwd* getpwuid(),* pw_ptr;
    static char numstr[10];
    if((pw_ptr = getpwuid(uid))==NULL){
        sprintf(numstr,"%d",uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;

}

char* getNameFromGid(gid_t gid){
    struct group* getgrgid(),* grp_ptr;
    static char numstr[10];
    if((grp_ptr=getgrgid(gid))==NULL){
        sprintf(numstr,"%d",gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;

    
}

//ls

/*
 
DIR * opendir(const char * name);打开一个目录
struct dirent * readdir(DIR *);读目录，依次返回目录的子项（一次返回一个，读一个目录指针后移一次），到目录尾或出错时返回NULL，出错会设
 errno
 
 
 #include<sys/stat.h>
 int stat(const char * path,struct stat * buf); 将path参数（文件或目录）的文件信息写到buf中，buf为传出参数
 
 */
void ls(const char dirName[],int param){
    DIR* dir_ptr;
    struct dirent* dirent_ptr;
    dir_ptr = opendir(dirName);
    if(dir_ptr==NULL)
        fprintf(stderr, "cannot open %s \n", dirName);
    else{
        if(param==ls_d)
            printf("%s\n",dirName);
        else{
            char dirs[50][100];
            int dir_count = 0;
        
            while((dirent_ptr=readdir(dir_ptr))!=NULL){
                //-l -i -R -d 隐藏项不读
                if(param<200&& dirent_ptr->d_name[0]=='.')
                    continue;
                
                char completeFileLocation[200];
                strcpy(completeFileLocation, dirName);
                strcat(completeFileLocation, "/");
                strcat(completeFileLocation, dirent_ptr->d_name);
                
                struct stat info;
                stat(completeFileLocation, &info);
                if(param==ls_l)
                    showFileInfo(dirent_ptr->d_name, &info);
                else if(param == ls_a ||param ==ls_none)
                    printf("%s\n",dirent_ptr->d_name);
                else if(param == ls_i){
                    printf("%llu",dirent_ptr->d_ino);
                    printf("%s\n",dirent_ptr->d_name);
                }
                else if(param==ls_R){
                    if(S_ISDIR(info.st_mode)){
                        printf("%s\n",dirent_ptr->d_name);
                        strcpy(dirs[dir_count], completeFileLocation);
                        dir_count++;
                    }
                    else
                        printf("%s\n",dirent_ptr->d_name);
                    
                }
                
                
            }
            
            if(param == ls_R){
                printf("\n");
                for(int index = 0;index<dir_count;index++){
                    printf("%s:\n",dirs[index]);
                    ls(dirs[index],ls_R);
                    printf("\n");
                }
            }
            closedir(dir_ptr);
            
        }
    }
    
}


//解析参数
int analyzeParam(const char* param){
    if(strlen(param)==2){
        if(param[0]!='-') return -1;
        
        if(param[1]=='l') return ls_l;
        if(param[1]=='a') return ls_a;
        if(param[1]=='d') return ls_d;
        if(param[1]=='R') return ls_R;
        if(param[1]=='i') return ls_i;
    }
    
    return -1;
}


int main(int argc, const char * argv[]) {
//hello.exe Shiqi Yu argc的值是 3，argv[0]是"hello.exe"，argv[1]是"Shiqi"，argv[2]是"Yu"
/*
 
./myls  执行ls(".",ls_none);
./myls -l  执行ls(".",param),无文件名,则默认./目录
./myls -l fileName 执行ls(fileName,param)
 
 */
    if(argc == 1)
        ls(".",ls_none);
    else{
        int param = ls_none;
        int exist_file_param = 0;
        while(argc>1){
            argc--;
            argv++;
            
            int paramRes = analyzeParam(*argv);
            if(paramRes!=-1)
                param = param+paramRes;
            else
            {
                exist_file_param = 1;
                do{
                    printf("%s:\n",*argv);
                    ls(*argv,param);
                    printf("\n");
                    
                    argc--;
                    argc++;
                }while(argc>=1);
                
            }
            
            if(!exist_file_param)
                ls(".",param);
        }
        
    }
    
    return 0;
}
