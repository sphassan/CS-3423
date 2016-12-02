#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSZ 1024

int fill_buffer(void); 
int print_line(void);
void write_newline(void);

struct buffer {
  int fd;
  int size;
  char *pos;
  char *end;
  char buf[1024];
} b;


int main (int argc, char * argv[])
  {
  
     if ( argc != 2 ) {
       fprintf(stderr,"Usage: prog1 <file>\n");
       exit(-1);
     }
  
    if((b.fd = open(argv[1], O_RDONLY)) < 0 ) {
      perror(argv[1]);
      exit(-1);
    }
  
    if ( fill_buffer() == 0 )
      exit(0);

    while ( print_line() );
      
    exit(0);
  }

int fill_buffer(void) {
  int n;

  if ( (n = read(b.fd,b.buf,BUFSZ)) == -1 ) {
    perror("Read error");
    exit(-1);
  }

  b.size = n;
  b.pos = b.buf;
  b.end = b.buf + n;
  return(n);
}
 
int print_line(void) {
  int line_printed, n, length;
  char *cur;

  cur = b.pos;
  line_printed = 0;
  while ( !line_printed ) {

    // Advance cur to either the end of the buffer or to the
    // next newline.
    while ( cur < b.end && *cur != '\n' ) {
      cur++;
    }

    // Compute length of data to be printed
    if ( cur == b.end ) {
      length = b.end-b.pos;
    } else {
      length = cur + 1 - b.pos;
    }

    // Write data and check for errors (real or just don't want to code)
    n = write(STDOUT_FILENO,b.pos,length);
    if ( n == -1 ) {
      perror("Write Error");
      exit(-1);
    } 
    if ( n != length ) {
      fprintf(stderr,"Partial Write - not an Error - just lazy\n");
      exit(-1);
    }
    
    // Adjust buffer structure 
    if ( cur == b.end ) {
      fill_buffer();
      cur = b.pos;
      if ( b.size == 0 )
        return(0);
    } else {
      if ( cur == b.end-1 ) {
        fill_buffer();
        cur = b.pos;
      } else {
        b.pos = cur+1;
      }
    write_newline();
    line_printed = 1;
    }
  }
  return(1);
}

void write_newline(void) {
  int n;

  n = write(STDOUT_FILENO,"\n",1);
  if ( n != 1 ) {
    perror("Newline Write Error");
    exit(-1);
  }
}
