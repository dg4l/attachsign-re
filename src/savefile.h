int saveFile(char *out,char *buf,size_t size){
  FILE *outfile;
  int retval;
  int ret;
  size_t num_bytes_written;
  
  if (!out) {
    ret = -1;
  }
  else if (!buf) {
    ret = -1;
  }
  else if (!size) {
    ret = -1;
  }
  else {
    num_bytes_written = 0;
    outfile = fopen(out,"wb");
    if (outfile) {
      if (buf) {
        num_bytes_written = fwrite(buf,1,size,outfile);
      }
      fclose(outfile);
    }
    if (size != num_bytes_written) {
      size = 0;
    }
    if (size == 0) {
      retval = -1;
    }
    else {
      retval = 0;
    }
    ret = retval;
  }
  return ret;
}

