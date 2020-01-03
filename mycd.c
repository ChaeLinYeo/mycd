#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <linux/limits.h>
#define SIZE 1024
//#ifndef _MAX_PATH
//#define _MAX_PATH 260
//#endif
//char find_directory[SIZE];

int mycd(char *av);
//void access_perm(char *perm,mode_t mode);
void listdir(char *pth);
void rwx(mode_t file_mode);

int mycd(char *av){
    DIR* dir_info = NULL; 
    dir_info=opendir(av);
    if (chdir(av)<0){
        return -1;
    }
    else if (dir_info == NULL){
        return -1;
    }
    else if (dir_info != NULL){
        return 0;
    }
}

void listdir(char *pth){
    DIR *dirp;
    //dirp = opendir(pth);
    struct dirent *dentry;
    int return_stat;
    char *file_name;
    struct stat file_info;
    struct tm *tminfo;
    mode_t file_mode;
    struct passwd *my_passwd;
    struct group *my_group;
    char buf1[80];
    struct stat statbuf;

    if((dirp = opendir(".")) == NULL)
        exit(1);

    while( dentry = readdir(dirp)) {
        if ((return_stat = stat(dentry->d_name, &file_info)) == -1){
            perror("Error : ");
            exit(0);
        }
        file_mode = file_info.st_mode;
        rwx(file_mode);//permission
        printf("%ld ", file_info.st_nlink);//number of links
        my_passwd = getpwuid(file_info.st_uid);
        my_group  = getgrgid(file_info.st_gid);
        printf("%s ", my_passwd->pw_name); //owner
        printf("%s ", my_group->gr_name); //group
        printf("%ld ", file_info.st_size); //file size
        tminfo = localtime (&(file_info.st_atime));
        strftime(buf1, 80, "%m월%d일 %H:%M", tminfo); 
        printf("%s ", buf1);   //time

        if(dentry->d_ino != 0) 
            printf("%s\n", dentry->d_name); //filename
    } 
    closedir(dirp); 
}

void rwx(mode_t file_mode){
    if (S_ISDIR(file_mode))
            printf("d");
        else
            printf("-");
        if (file_mode & S_IRUSR)
            printf("r");
        else
            printf("-");
        if (file_mode & S_IWUSR)
            printf("w");
        else
            printf("-");
        if (file_mode & S_IXUSR)
            printf("x");
        else
            printf("-"); 
        if (file_mode & S_IRGRP)
            printf("r");
        else
            printf("-");
        if (file_mode & S_IWGRP)
            printf("w");
        else
            printf("-");
        if(file_mode & S_IXGRP)
            printf("x");
        else
            printf("-"); 
        if (file_mode & S_IROTH)
            printf("r");
        else
            printf("-");
        if (file_mode & S_IWOTH)
            printf("w");
        else
            printf("-");
        if(file_mode & S_IXOTH)
            printf("x");
        else
            printf("-");
        printf(" ");
}


int main( )
{
    char strBuffer[PATH_MAX];
    char strChangeDir[PATH_MAX];
    char name[PATH_MAX];
    char add[PATH_MAX];
    DIR *dir_info;
    struct diren *dir_entry;

    while(1){
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("If you want to exit mycd, write down 'esc'\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
        printf("Current Directory:%s\n",getcwd(name,PATH_MAX));
        printf("mycd $ ");
        scanf("%s",strChangeDir);

        int nResult = mycd(strChangeDir);
        //char name[PATH_MAX];
        //dir_info = opendir("./");


        if (!strcmp(strChangeDir,"esc")){
            break;
        }

        if( nResult == 0 )
        {
            //dir_info = opendir("./");
            printf( "이동 성공\n" );
            listdir(strChangeDir);
            //printf(dir_info);
            //system("ls -al");
            //printf("Current Directory:%s\n",getcwd(name,PATH_MAX));
            //system("pwd");
        }
        else if( nResult == -1 )
        {
            perror( "이동 실패\n" );
            listdir(strChangeDir);
            //system("ls -al");
            //printf("Current Directory:%s\n",getcwd(name,PATH_MAX));
            //system("pwd");
        }
    }
    execl("/bin/bash", "/bin/bash",NULL);
    return 0;
}
