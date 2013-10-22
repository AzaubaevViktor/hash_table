#include "errors.h"

void setError(Error *error, int errorValue, char *str) {
  error->error = errorValue;
  strcpy(error->str,str);
}
