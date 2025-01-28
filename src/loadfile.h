int loadFile(char *input, char **bufptr, size_t *size){
  FILE *_File;
  int retval;
  int ret;
  size_t num;
  size_t pos;
  char *buf;
  
  if (input == (char *)NULL) {
    ret = -1;
  }
  else {
    buf = (void *)0;
    pos = 0;
    num = 0;
    _File = fopen(input,"rb");
    if (_File != (FILE *)NULL) {
      fseek(_File,0,2);
      pos = ftell(_File);
      fseek(_File,0,0);
      buf = malloc(pos);
      if (buf != (void *)NULL) {
        num = fread(buf,1,pos,_File);
      }
      fclose(_File);
    }
    if (((buf == (void *)0) || (pos == 0)) || (pos != num)) {
      if (buf != (void *)0) {
        free(buf);
      }
      buf = (void *)0;
      pos = 0;
    }
    if (bufptr != 0) {
      *bufptr = buf;
    }
    if (size != (size_t *)NULL) {
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

