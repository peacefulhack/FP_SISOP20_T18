#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
  int i;
  int j;
  //int len;
  int enter = 0;
  int amount = 0;
  //char pesan[512];

  if(argc < 2){
    printf(2, "argument kurang\n" );
    exit();
  }

  if (argv[1][0] == '-' && argv[1][1] == 'w'){
    char *aw = argv[1] + 2;
    amount = atoi(aw);
  }
  if(amount == 0){
    printf(1,"%d\n", amount);
    for(i = 1; i < argc; i++){
      printf(1, "%s%s", argv[i], i+1 < argc ? " " : "\n");
    }
    exit();
  }
  else{
    for(i = 2; i<argc;i++){
      if((strlen(argv[i]) + enter) <= amount){
        printf(1, "%s ", argv[i]);
        enter += strlen(argv[i]);
      }
      else{
        for(j = 0; j < (amount-enter);j++){
          printf(1,"%s", argv[i][j]);
        }
        printf(1,"%s\n",argv[i] + (amount-enter));
        enter += strlen(argv[i]) - amount;
      }
    }
    exit();
  }


}
