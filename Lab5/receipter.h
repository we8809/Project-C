#ifndef RECEIPTER_H
#define RECEIPTER_H

#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#endif /* _CRT_SECURE_NO_WARNINGS */

#include <stdio.h>
#include <string.h>
#include <time.h>

#define TRUE    (1)
#define FALSE   (0)

int add_item(const char* name, double price);
void set_tip(double tip);
void set_message(const char* message);
int print_receipt(const char* filename, time_t timestamp);

#endif  /* RECEIPTER_H */
