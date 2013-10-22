#ifndef ERRORS_H
#define ERRORS_H

typedef struct {
  int error;
  char str[100];
} Error;

void setError(Error *error, int errorValue, char *str);

#endif // ERRORS_H
