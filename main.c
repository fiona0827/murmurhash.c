
/**
 * `main.c' - murmurhash
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef UNIX
#include <unistd.h>
#include <inttypes.h>
#endif
#ifdef WIN32
#include <io.h> // for _isatty()
#endif
#include "murmurhash.h"

#ifdef WIN32
#define PRIu32 "u"
#endif

static void
usage () {
  fprintf(stderr, "usage: murmur [-hV] [options]\n");
}

static void
help () {
  fprintf(stderr, "\noptions:\n");
  fprintf(stderr, "\n  --seed=[seed]  hash seed (optional)");
  fprintf(stderr, "\n");
}

static char *
read_stdin () {
  #ifdef WIN32
  #define BSIZE 1024
  #else
  const size_t BSIZE = 1024;
  #endif
  size_t size = 1;
  char buf[BSIZE];
  char *res = (char *) malloc(sizeof(char) * BSIZE);
  char *tmp = NULL;

  // memory issue
  if (NULL == res) { return NULL; }

  // cap
  res[0] = '\0';

  // read
  if (NULL != fgets(buf, BSIZE, stdin)) {
    // store
    tmp = res;
    // resize
    size += (size_t) strlen(buf);
    // realloc
    res = (char *) realloc(res, size);

    // memory issues
    if (NULL == res) {
      free(tmp);
      return NULL;
    }

    // yield
#ifdef WIN32
    strcat_s(res, size, buf);
#else
    strcat(res, buf);
#endif

    return res;
  }

  free(res);

  return NULL;
}

#define isopt(opt, str) (0 == strncmp(opt, str, strlen(str)))

#define setopt(opt, key, var) do {                  \
    size_t i;                                       \
    char tmp = 0;                                   \
    size_t len = strlen(key) + 1;                   \
    for (i = 0; i < len; ++i) { tmp = *opt++; }     \
    var = opt;                                      \
  } while (0);

int
main (int argc, char **argv) {
  char *buf = NULL;
  char *key = NULL;
  char *seed = NULL;
  uint32_t h = 0;

  // parse opts
  {
    char *opt = NULL;
    char tmp = 0;

    opt = *argv++; // unused

    while ((opt = *argv++)) {

      // flags
      if ('-' == *opt++) {
        switch (*opt++) {
          case 'h':
            return usage(), help(), 0;

          case 'V':
            fprintf(stderr, "%s\n", MURMURHASH_VERSION);
            return 0;

          case '-':
            if (isopt(opt, "seed")) {
              setopt(opt, "seed", seed);
            }
            break;

          default:
            tmp = *opt--;
            // error
            fprintf(stderr, "unknown option: `%s'\n", opt);
            usage();
            return 1;
        }
      }
    }
  }

  if (NULL == seed) {
    seed = "0";
  }

#define hash(key) murmurhash(key, (uint32_t) strlen(key), (uint32_t) atoi(seed));

  if (1 == _isatty(0)) { return 1; }
  else if (ferror(stdin)) { return 1; }
  else {
    buf = read_stdin();
    if (NULL == buf) { return 1; }
    else if (0 == strlen(buf)) { buf = ""; }
    h = hash(buf);
    printf("%" PRIu32 "\n", h);
    do {
      key = read_stdin();
      if (NULL == key) { break; }
      else if (0 == strlen(buf)) { buf = ""; }
      h = hash(buf);
      printf("%d" PRIu32 "\n", h);
    } while (NULL != key);
  }

#undef hash

  return 0;
}
