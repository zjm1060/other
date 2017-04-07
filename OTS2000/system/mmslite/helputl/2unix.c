
#include <stdio.h>



main(argc, argv)

int  argc;
char *argv[];



{

  register int           c;

           unsigned char buf[80];
           FILE          *fil1;
           FILE          *fil2;



  if (argc < 2) {
    printf("2unix - ERROR: No file to translate\n");
    printf("2unix - Syntax: 2dufi <file name>\n");
    exit(1);
  }
  if ((fil1 = fopen(argv[1], "r")) == (FILE *)NULL) {
    printf("2unix - ERROR: Can't open file %s\n", argv[1]);
    exit(1);
  }
  system("rm 2unix.tmp >/dev/null 2>/dev/null");
  if ((fil2 = fopen("2unix.tmp", "w")) == (FILE *)NULL) {
    printf("2unix - ERROR: Can't open file %s\n", argv[1]);
    exit(1);
  }

  while ((c = getc(fil1)) != EOF) {
    if (c != 0x0d) fprintf(fil2, "%c", c);
  }
  fclose(fil2);
  fclose(fil1);
  sprintf(buf, "mv 2unix.tmp %s", argv[1]);
  system(buf);

}

