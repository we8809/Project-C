#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "data_store.h"

#include <assert.h>

user_t* get_user_by_id_or_null(user_t** users_or_null, size_t id)
{
    if (users_or_null == NULL) {
        return NULL;
    }

    while (true) {

        if (*users_or_null == NULL) {
            break;
        }

        if ((*users_or_null)->id == id) {
            return *users_or_null;
        }

        ++users_or_null;
    }

    return NULL;
}

user_t* get_user_by_username_or_null(user_t** users_or_null, const char* username)
{
    if (users_or_null == NULL) {
        return NULL;
    }

    if (username == NULL) {
        return NULL;
    }

    while (true) {

        if (*users_or_null == NULL) {
            break;
        }

        if (strcmp((*users_or_null)->username, username) == 0) {
            return *users_or_null;
        }

        ++users_or_null;
    }

    return NULL;
}

bool update_email(user_t** users_or_null, size_t id, const char* email)
{
    FILE* log_fp;

    char log_text[512] = { 0, };
    char prev_email[MAX_EMAIL_LENGTH] = { 0, };

    size_t index;
    size_t prev_email_length;
    size_t prev_email_id_length = 0;
    size_t email_length;
    size_t email_id_length = 0;

    if (users_or_null == NULL) {
        return NULL;
    }

    if (email == NULL) {
        return NULL;
    }

    log_fp = fopen("log.txt", "a+");

    while (true) {

        if (*users_or_null == NULL) {
            break;
        }

        if ((*users_or_null)->id == id) {
            snprintf(prev_email, MAX_EMAIL_LENGTH, "%s", (*users_or_null)->email);
            prev_email[MAX_EMAIL_LENGTH - 1] = '\0';

            snprintf((*users_or_null)->email, MAX_EMAIL_LENGTH, "%s", email);
            (*users_or_null)->email[MAX_EMAIL_LENGTH - 1] = '\0';

            prev_email_length = strlen(prev_email);
            email_length = strlen(email);

            for (index = 0; index < prev_email_length; index++) {
                if (prev_email[index] == '@') {
                    break;
                }

                ++prev_email_id_length;
            }

            for (index = 0; index < email_length; index++) {
                if (email[index] == '@') {
                    break;
                }

                ++email_id_length;
            }

#if !defined(RELEASE)
            sprintf(log_text, "TRACE: User %d updated email from \"%s\" to \"%s\"\n", id, prev_email, email);
            fprintf(log_fp, "%s", log_text);
#else
            sprintf(log_text, "TRACE: User %d updated email from \"", id);
            fprintf(log_fp, "%s", log_text);

            for (index = 0; index < prev_email_length; index++) {
                if (index < prev_email_id_length) {
                    if ((index == 0) && (index == prev_email_id_length - 1)) {
                        fputc('*', log_fp);
                    } else if (index == 0) {
                        fputc(prev_email[index], log_fp);
                    } else if ((index == 1) && (index == prev_email_id_length - 1)) {
                        fputc('*', log_fp);
                    } else if (index == (prev_email_id_length - 1)) {
                        fputc(prev_email[index], log_fp);
                    } else {
                        fputc('*', log_fp);
                    }
                } else {

                    fputc(prev_email[index], log_fp);
                }
            }

            sprintf(log_text, "\" to \"");
            fprintf(log_fp, "%s", log_text);

            for (index = 0; index < email_length; index++) {
                if (index < email_id_length) {
                    if ((index == 0) && (index == email_id_length - 1)) {
                        fputc('*', log_fp);
                    } else if (index == 0) {
                        fputc(email[index], log_fp);
                    } else if ((index == 1) && (index == email_id_length - 1)) {
                        fputc('*', log_fp);
                    } else if (index == (email_id_length - 1)) {
                        fputc(email[index], log_fp);
                    } else {
                        fputc('*', log_fp);
                    }

                } else {

                    fputc(email[index], log_fp);
                }
            }

            fputc('\"', log_fp);
            fputc('\n', log_fp);
#endif

            fclose(log_fp);
            return true;
        }

        ++users_or_null;
    }

    fclose(log_fp);
    return false;
}

bool update_password(user_t** users_or_null, size_t id, const char* password)
{
    FILE* log_fp;

    char log_text[512] = { 0, };
    char prev_password[MAX_PASSWORD_LENGTH] = { 0, };

    size_t password_length;
    size_t prev_password_length;

    if (users_or_null == NULL) {
        return false;
    }

    log_fp = fopen("log.txt", "a+");

    while (true) {

        if (*users_or_null == NULL) {
            break;
        }

        if ((*users_or_null)->id == id) {
            snprintf(prev_password, MAX_PASSWORD_LENGTH, "%s", (*users_or_null)->password);
            prev_password[MAX_PASSWORD_LENGTH - 1] = '\0';

            snprintf((*users_or_null)->password, MAX_PASSWORD_LENGTH, "%s", password);
            (*users_or_null)->password[MAX_PASSWORD_LENGTH - 1] = '\0';

            prev_password_length = strlen(prev_password);
            password_length = strlen(password);

#if !defined(RELEASE)
            sprintf(log_text, "TRACE: User %d updated password from \"%s\" to \"%s\"\n", id, prev_password, password);
            fprintf(log_fp, "%s", log_text);
#else
            size_t index;

            sprintf(log_text, "TRACE: User %d updated password from \"", id);
            fprintf(log_fp, "%s", log_text);

            for (index = 0; index < prev_password_length; index++) {
                if ((index == 0) && (index == (prev_password_length - 1))) {
                    fputc('*', log_fp);
                } else if (index == 0) {
                    fputc(*(prev_password + index), log_fp);
                } else if ((index == 1) && (index == (prev_password_length - 1))) {
                    fputc('*', log_fp);
                } else if (index == (prev_password_length - 1)) {
                    fputc(*(prev_password + index), log_fp);
                } else {
                    fputc('*', log_fp);
                }
            }

            sprintf(log_text, "\" to \"");
            fprintf(log_fp, "%s", log_text);

            for (index = 0; index < password_length; index++) {
                if ((index == 0) && (index == (password_length - 1))) {
                    fputc('*', log_fp);
                } else if (index == 0) {
                    fputc(*(password + index), log_fp);
                } else if ((index == 1) && (index == (password_length - 1))) {
                    fputc('*', log_fp);
                } else if (index == (password_length - 1)) {
                    fputc(*(password + index), log_fp);
                } else {
                    fputc('*', log_fp);
                }
            }

            fputc('\"', log_fp);
            fputc('\n', log_fp);
#endif

            fclose(log_fp);
            return true;
        }

        ++users_or_null;
    }

    fclose(log_fp);
    return false;
}
