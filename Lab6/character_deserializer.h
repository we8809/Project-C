#ifndef CHARACTER_DESERIALIZER_H
#define CHARACTER_DESERIALIZER_H

#ifndef _CRT_SECURE_NO_WARNINGS_
#define _CRT_SECURE_NO_WARNINGS_
#endif

#include <stdio.h>
#include <string.h>

#define TRUE    (0)
#define FALSE   (1)

#define MAX_MINION_NAME_LENGTH      (51U)   
#define MAX_CHARACTER_NAME_LENGTH   (51U)
#define MAX_MINION_NAME_LENGTH      (51U)

#define MAX_MINION_COUNT            (3U)

typedef enum {
    VERSION_ONE = 1,
    VERSION_TWO,
    VERSION_THREE
} version_t;
typedef enum {
    PERSPECTIVE_V2_NAME,
    PERSPECTIVE_V2_LEVEL,
    PERSPECTIVE_V2_STRENGTH,
    PERSPECTIVE_V2_DEXTERITY,
    PERSPECTIVE_V2_INTELLIGENCE,
    PERSPECTIVE_V2_ARMOUR,
    PERSPECTIVE_V2_EVASION,
    PERSPECTIVE_V2_MAGIC_RESISTANCE,
    PERSPECTIVE_V2_HEALTH,
    PERSPECTIVE_V2_MANA
} perspective_character_v2_t;
typedef enum {
    PERSPECTIVE_V3_NAME,
    PERSPECTIVE_V3_LEVEL,
    PERSPECTIVE_V3_HEALTH,
    PERSPECTIVE_V3_MANA,
    PERSPECTIVE_V3_STRENGTH,
    PERSPECTIVE_V3_DEXTERITY,
    PERSPECTIVE_V3_INTELLIGENCE,
    PERSPECTIVE_V3_ARMOUR,
    PERSPECTIVE_V3_EVASION,
    PERSPECTIVE_V3_FIRE_RES,
    PERSPECTIVE_V3_COLD_RES,
    PERSPECTIVE_V3_LIGHTNING_RES,
    PERSPECTIVE_V3_LEADERSHIP,
    PERSPECTIVE_V3_MINION_COUNT
} perspective_character_v3_t;
typedef enum {
    PERSPECTIVE_MINION_V3_NAME,
    PERSPECTIVE_MINION_V3_HEALTH,
    PERSPECTIVE_MINION_V3_STRENGTH,
    PERSPECTIVE_MINION_V3_DEFENCE
} perspective_minion_v3_t;

typedef struct {
    char name[MAX_MINION_NAME_LENGTH];
    int health;
    int strength;
    int defence;
} minion_t;
typedef struct {
    int fire;
    int cold;
    int lightning;
} elemental_resistance_t;
typedef struct {
    char name[MAX_CHARACTER_NAME_LENGTH];
    int level;
    int health;
    int mana;
    int strength;
    int dexterity;
    int intelligence;
    int armour;
    int evasion;
    int leadership;
    size_t minion_count;
    elemental_resistance_t elemental_resistance;
    minion_t minions[MAX_MINION_COUNT];
} character_v3_t;

int get_character(const char* filename, character_v3_t* out_character);
int get_version_info(FILE* file_p);

void transform_version_one(FILE* file_p, character_v3_t* out_character);
void transform_version_two(FILE* file_p, character_v3_t* out_character);
void transform_version_three(FILE* file_p, character_v3_t* out_character);

#endif /* CHARACTER_DESERIALIZER_H */
