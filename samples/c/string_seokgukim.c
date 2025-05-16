// string helper functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int len;
} string_t;

string_t *string_create(const char *str) {
    string_t *s = (string_t *)malloc(sizeof(string_t));
    if (!s) {
        perror("Failed to allocate memory for string");
        exit(EXIT_FAILURE);
    }
    s->len = strlen(str);
    s->str = (char *)malloc((s->len + 1) * sizeof(char));
    if (!s->str) {
        perror("Failed to allocate memory for string data");
        free(s);
        exit(EXIT_FAILURE);
    }
    strcpy(s->str, str);
    return s;
}

void string_free(string_t *s) {
    if (s) {
        free(s->str);
        free(s);
    }
}

int concatenate_strings(string_t *dest, const string_t *src) {
    if (!dest || !src) return -1;
    dest->str = (char *)realloc(dest->str, (dest->len + src->len + 1) * sizeof(char));
    if (!dest->str) {
        perror("Failed to reallocate memory for concatenated string");
        return -1;
    }
    strcat(dest->str, src->str);
    dest->len += src->len;
    return 0;
}

int concatenate_strings(string_t *dest, const char *src) {
    if (!dest || !src) return -1;
    int src_len = strlen(src);
    dest->str = (char *)realloc(dest->str, (dest->len + src_len + 1) * sizeof(char));
    if (!dest->str) {
        perror("Failed to reallocate memory for concatenated string");
        return -1;
    }
    strcat(dest->str, src);
    dest->len += src_len;
    return 0;
}

int compare_strings(const string_t *a, const string_t *b) {
    if (!a || !b) return -1;
    return strcmp(a->str, b->str);
}

int string_length(const string_t *s) {
    if (!s) return -1;
    return s->len;
}