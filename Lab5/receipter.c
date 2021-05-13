#include "receipter.h"

#include <assert.h>
#include <ctype.h>

#define MAX_LENGTH              (4096U)
#define MAX_ITEM_COUNT          (10U)
#define MAX_MESSAGE_LENGTH      (76U)
#define MAX_ITEM_NAME_LENGTH    (26U)

#define LINE_LENGTH             (50U)

static size_t s_receipt_count = 0;

static size_t s_item_count = 0;
static char s_item_name_array[MAX_ITEM_COUNT][MAX_ITEM_NAME_LENGTH];
static double s_item_price_array[MAX_ITEM_COUNT] = { 0.0, };

static char s_message[MAX_MESSAGE_LENGTH] = { 0, };

static double s_tip = 0.0;

static size_t s_flag_item_setted = FALSE;
static size_t s_flag_message_setted = FALSE;
static size_t s_flag_tip_setted = FALSE;

int add_item(const char* name, double price)
{
    if (name == NULL) {
        return FALSE;
    }

    if ((price < 0.0) || (price > 999.99)) {
        return FALSE;
    }

    if (s_item_count >= MAX_ITEM_COUNT) {
        return FALSE;
    }

    strncpy(s_item_name_array[s_item_count], name, (MAX_ITEM_NAME_LENGTH - 1));
    s_item_name_array[s_item_count][MAX_ITEM_NAME_LENGTH - 1] = '\0';

    s_item_price_array[s_item_count] = price;

    s_item_count++;
    s_flag_item_setted = TRUE;

    return TRUE;
}

void set_tip(double tip)
{
    if (tip <= 0.0) {
        return;
    }

    if (s_flag_item_setted == FALSE) {
        return;
    }

    assert(tip <= 999.99);

    s_tip = tip;
    s_flag_tip_setted = TRUE;
}

void set_message(const char* message)
{
    size_t msg_length = 0;
    size_t msg_length_remain = 0;

    if (message == NULL) {
        return;
    }

    if (s_flag_item_setted == FALSE) {
        return;
    }

    if (strlen(s_message) >= MAX_MESSAGE_LENGTH - 1) {
        return;
    }
    
    if (s_flag_message_setted == FALSE) {
        strncpy(s_message, message, (MAX_MESSAGE_LENGTH - 1));
        s_message[MAX_MESSAGE_LENGTH - 1] = '\0';

        /* MSG DEBUG */
        fprintf(stdout, "set_message() -> msg = %s\n", s_message);
        fprintf(stdout, "set_message() -> msg_length = %d\n", strlen(s_message));
        */

    } else {
        msg_length = strlen(s_message);
        msg_length_remain = MAX_MESSAGE_LENGTH - msg_length - 1;

        if (msg_length_remain == 0) {
            return;
        }

        strncat(s_message, message, msg_length_remain);
        s_message[MAX_MESSAGE_LENGTH - 1] = '\0';

        /* ADD MSG DEBUG */
        fprintf(stdout, "set_message() -> add msg = %s\n", s_message);
        fprintf(stdout, "set_message() -> add msg_length = %d\n", strlen(s_message));
    }

    s_flag_message_setted = TRUE;
}

int print_receipt(const char* filename, time_t timestamp)
{
    FILE* receipt_file;
    struct tm* time_set;
    char buffer[MAX_LENGTH] = { 0, };

    size_t item_count = 0;
    size_t item_name_count = 0;
    size_t message_length = 0;
    size_t message_count = 0;

    double price = 0.0;
    double price_sum = 0.0;
    double tax = 0.0;
    double total_price = 0.0;

    if (filename == NULL) {
        return FALSE;
    }

    if (s_flag_item_setted == FALSE) {
        return FALSE;
    }

    if (s_item_count == 0) {
        return FALSE;
    }

    assert(s_receipt_count <= 99999);

    /* open receipt file */
    receipt_file = fopen(filename, "w+");

    /* get current local time */
    time(&timestamp);
    time_set = gmtime(&timestamp);

    /* print default info */
    sprintf(buffer, "Charles' Seafood");
    fprintf(receipt_file, "%s\n", buffer);
    sprintf(buffer, "--------------------------------------------------");
    fprintf(receipt_file, "%s\n", buffer);
    
    /* print time info */
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", time_set->tm_year + 1900, time_set->tm_mon + 1, time_set->tm_mday, time_set->tm_hour, time_set->tm_min, time_set->tm_sec);
    fprintf(receipt_file, "%s", buffer);
    sprintf(buffer, "%26s%05d", " ", s_receipt_count);
    fprintf(receipt_file, "%s\n", buffer);

    sprintf(buffer, "--------------------------------------------------");
    fprintf(receipt_file, "%s\n", buffer);

    /* print item info */
    for (item_count = 0; item_count < s_item_count; item_count++) {
        sprintf(buffer, "%33s", s_item_name_array[item_count]);
        fprintf(receipt_file, "%s", buffer);

        price = s_item_price_array[item_count];
        price_sum += price;

        sprintf(buffer, "%17.2lf", price);
        fprintf(receipt_file, "%s\n\n", buffer);
    }

    /* print calc info */
    tax = price_sum * 0.05;
    total_price = price_sum + s_tip + tax;

    sprintf(buffer, "%33s", "Subtotal");
    fprintf(receipt_file, "%s", buffer);
    sprintf(buffer, "%17.2lf", price_sum);
    fprintf(receipt_file, "%s\n", buffer);

    if (s_flag_tip_setted == TRUE) {
        sprintf(buffer, "%33s", "Tip");
        fprintf(receipt_file, "%s", buffer);

        sprintf(buffer, "%17.2lf", s_tip);
        fprintf(receipt_file, "%s\n", buffer);
    }

    sprintf(buffer, "%33s", "Tax");
    fprintf(receipt_file, "%s", buffer);
    sprintf(buffer, "%17.2lf", tax);
    fprintf(receipt_file, "%s\n", buffer);

    
    sprintf(buffer, "%33s", "Total");
    fprintf(receipt_file, "%s", buffer);
    sprintf(buffer, "%17.2lf", total_price);
    fprintf(receipt_file, "%s\n", buffer);

    /* print message info */
    if (s_flag_message_setted == TRUE) {
        fputc('\n', receipt_file);

        message_length = strlen(s_message);

        if (message_length > 50) {
            
            for (message_count = 0; message_count < LINE_LENGTH; message_count++) {
                fputc(s_message[message_count], receipt_file);
            }
            fputc('\n', receipt_file);

            
            for (message_count = LINE_LENGTH; message_count < message_length; message_count++) {
                assert(message_count < MAX_MESSAGE_LENGTH);
                fputc(s_message[message_count], receipt_file);
            }
            fputc('\n', receipt_file);
        } else {
            
            for (message_count = 0; message_count < message_length; message_count++) {
                fputc(s_message[message_count], receipt_file);
            }
            fputc('\n', receipt_file);
        }
    } else {

        fputc('\n', receipt_file);
    }

    /* print final info */
    sprintf(buffer, "==================================================");
    fprintf(receipt_file, "%s\n", buffer);
    sprintf(buffer, "%50s", "Tax#-51234");
    fprintf(receipt_file, "%s", buffer);

    /* initialize for next receipt */
    s_receipt_count++;

    for (item_count = 0; item_count < MAX_ITEM_COUNT; item_count++) {
        for (item_name_count = 0; item_name_count < MAX_ITEM_NAME_LENGTH; item_name_count++) {
            s_item_name_array[item_count][item_name_count] = 0;
        }

        s_item_price_array[item_count] = 0.0;
    }
    s_flag_item_setted = FALSE;
    s_item_count = 0;

    if (s_flag_message_setted == TRUE) {
        for (message_count = 0; message_count < MAX_MESSAGE_LENGTH; message_count++) {
            s_message[message_count] = 0;
        }

        s_flag_message_setted = FALSE;
    }

    if (s_flag_tip_setted == TRUE) {
        s_tip = 0.0;
        s_flag_tip_setted = FALSE;
    }

    fclose(receipt_file);

    return TRUE;
}
