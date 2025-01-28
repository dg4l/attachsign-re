int loadFile(char *input, char **bufptr, size_t *size){
  FILE *infile;
  int retval;
  int ret;
  size_t num;
  size_t pos;
  char *buf;
  
  if (!input) {
    ret = -1;
  }
  else {
    buf = 0;
    pos = 0;
    num = 0;
    infile = fopen(input,"rb");
    if (infile) {
      fseek(infile,0,2);
      pos = ftell(infile);
      fseek(infile,0,0);
      buf = malloc(pos);
      if (buf) {
        num = fread(buf,1,pos,infile);
      }
      fclose(infile);
    }
    if ((!buf) || (!pos) || (pos != num)) {
      if (buf) {
        free(buf);
      }
      buf = 0;
      pos = 0;
    }
    if (bufptr) {
      *bufptr = buf;
    }
    if (size) {
      *size = pos;
    }
    if (!pos) {
      retval = -1;
    }
    else {
      retval = 0;
    }
    ret = retval;
  }
  return ret;
}

