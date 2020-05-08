#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int iter = 0;

char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
//char* rename(char* text) {
//   char blank[100];
//   int c = 0, d = 0;
//
//   while (text[c] != '\0')
//   {
//      if (!(text[c] == ' ' && text[c+1] == ' ')) {
//        blank[d] = text[c];
//        d++;
//      }
//      c++;
//   }
//
//   blank[d] = '\0';
//
//   printf("Text after removing blanks\n%s\n", blank);
//
//   return black;
//}
void ls(char *path)
{
  iter++;
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    iter--;
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    iter--;
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      iter--;
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if(strcmp(fmtname(buf), ".") !=32 && strcmp(fmtname(buf), ".") !=46){
        for(int y=1;y<iter;y++){
          printf(1, "  ");
        }
        printf(1, "|");
        printf(1, "__%s\n",fmtname(buf)/*, st.type, st.ino, st.size*/);
        if(st.type==1){
          ls(buf);
        }
      }
    }
    break;
  }
  iter--;
  close(fd);
}

int main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit();
}
