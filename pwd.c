#include "types.h"
#include "fcntl.h"
#include "fs.h"
#include "stat.h"
#include "user.h"

#define PATH_SEPARATOR   "/"

static int    dir_look(int fd, int ino, char* p){
  struct dirent de;
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    if (de.inum == ino) {
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = '\0';
      return 1;
    }
  }
  return 0;
}
static char*  dir_fnd(int ino, char* dir_up, char* directory){
  strcpy(dir_up + strlen(dir_up), PATH_SEPARATOR "..");
  struct stat st;
  if(stat(dir_up, &st) <0) return ((void*)0);
  if(st.ino == ino) return directory;
  char* getdir = ((void*)0);
  int fd = open(dir_up, O_RDONLY);
  if(fd >= 0){
    char* p = dir_fnd(st.ino, dir_up, directory);
    if(p!=((void*)0)){
      strcpy(p, PATH_SEPARATOR);
      p += sizeof(PATH_SEPARATOR) - 1;

      if (dir_look(fd, ino, p)) getdir = p+strlen(p);
    }
    close(fd);
  }
  return getdir;
}
static int    dir_cwd(char* directory){
  directory[0] = '\0';
  char dir_up[512];
  strcpy(dir_up, ".");
  struct stat st;
  if(stat(dir_up, &st) < 0) return 0;
  char* p = dir_fnd(st.ino, dir_up, directory);
  if(p==((void*)0)) return 0;
  if(directory[0] == '\0') strcpy(directory,PATH_SEPARATOR);
  return 1;
}
//find cd .. sampe ngga ada lagi (udah paling root)


int main(int argc, char *argv[]){

  char directory[512];
  if(dir_cwd(directory)) printf(1, "%s\n", directory);
  else printf(2, "pwd gagal" );
  exit();
}
