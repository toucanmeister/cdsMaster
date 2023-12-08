#include <stdio.h>

char *next_token(FILE *stream)
{
  static char buffer[256];
  int pos = 0;
  int c;
  state_0: switch (c = getc(stream)) {
    case 'a': buffer[pos++] = c;
              goto state_1;
    default : goto state_err; }
  state_1: switch (c = getc(stream)) {
    case 'a': buffer[pos++] = c;
              goto state_2;
    case 'c': buffer[pos++] = c;
              goto state_1;
    case EOF: goto state_succ;
    default : goto state_err; }
  state_2: switch (c = getc(stream)) {
    case 'b': buffer[pos++] = c;
              goto state_1;
    default : goto state_err; }
  state_succ: return buffer;
  state_err: return NULL;
}

int main(void) {
  FILE *f = fopen("mytest.txt", "r");
  char *t = next_token(f);
  if (t) {
    printf("Found token: %s\n", t);
  } else {
    printf("Token not found.\n");
  }
}
