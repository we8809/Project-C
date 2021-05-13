#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERNAME_LENGTH         (51U)
#define MAX_EMAIL_LENGTH            (51U)
#define MAX_PASSWORD_LENGTH         (51U)

typedef struct user {
    size_t id;
    char username[MAX_USERNAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} user_t;

#endif /* USER_H */
