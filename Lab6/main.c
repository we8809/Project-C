#include "character_deserializer.h"

int main(void)
{
    int err_code;
    character_v3_t chr;

    /* err_code = get_character("Superman_v1.txt", &chr); */
    /* err_code = get_character("Batman_v2.txt", &chr); */
    /* err_code = get_character("Flash_v3.txt", &chr); */
    err_code = get_character("Wonderwoman_v3.txt", &chr); 
    
     return err_code;
}
