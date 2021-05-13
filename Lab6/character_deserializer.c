#include "character_deserializer.h"

#define MAX_BUFFER_LENGTH       (4096U)
#define MAX_LINE_LENGTH         (512U)

int get_character(const char* filename, character_v3_t* out_character)
{
    int version;
    size_t minion_count;
    FILE* character_file = NULL;

    if (filename == NULL) {
        return FALSE;
    }

    character_file = fopen(filename, "r");
    if (character_file == NULL) {
        perror("error while opening");
        return FALSE;
    }

    version = get_version_info(character_file);
    printf("version info = %d\n\n", version);

    memset(out_character, 0, sizeof(character_v3_t));

    rewind(character_file);

    switch (version) {
    case VERSION_ONE:
        transform_version_one(character_file, out_character);
        break;
    case VERSION_TWO:
        transform_version_two(character_file, out_character);
        break;
    case VERSION_THREE:
        transform_version_three(character_file, out_character);
        break;
    default:
        fclose(character_file);
        return FALSE;
    }

    printf("version : %d\n", version);
    printf("name : %s\n", out_character->name);
    printf("level : %d\n", out_character->level);
    printf("health : %d\n", out_character->health);
    printf("mana : %d\n", out_character->mana);
    printf("strength : %d\n", out_character->strength);
    printf("dexterity : %d\n", out_character->dexterity);
    printf("intelligence : %d\n", out_character->intelligence);
    printf("armour : %d\n", out_character->armour);
    printf("evasion : %d\n", out_character->evasion);
    printf("leadership : %d\n", out_character->leadership);
    printf("minion_count : %d\n", out_character->minion_count);
    printf("elemental_resistance.fire : %d\n", out_character->elemental_resistance.fire);
    printf("elemental_resistance.cold : %d\n", out_character->elemental_resistance.cold);
    printf("elemental_resistance.lightning : %d\n", out_character->elemental_resistance.lightning);

    if (version == 3) {
        for (minion_count = 0; minion_count < (out_character->minion_count); minion_count++) {
            printf("minion[%d]_name = %s\n", minion_count, out_character->minions[minion_count].name);
            printf("minion[%d]_health = %d\n", minion_count, out_character->minions[minion_count].health);
            printf("minion[%d]_strength = %d\n", minion_count, out_character->minions[minion_count].strength);
            printf("minion[%d]_defence= %d\n", minion_count, out_character->minions[minion_count].defence);
        }
    }

    fclose(character_file);

    return version;
}

int get_version_info(FILE* file_p)
{
    int data;

    while ((data = fgetc(file_p)) != EOF) {
        if ((char)data == ':') {
            return VERSION_ONE;
        } 

        if ((char)data == '|') {
            return VERSION_THREE;
        }
    }

    return VERSION_TWO;
}

void transform_version_one(FILE* file_p, character_v3_t* out_character)
{
    /* 
        * 파일은 다음 캐릭터 정보를 저장한다.
        -    id: 캐릭터의 ID. 양수
        -    lvl: 캐릭터의 레벨. 양수
        -    str: 캐릭터의 힘(strength). 양수
        -    dex: 캐릭터의 민첩성(dexterity). 양수
        -    intel: 캐릭터의 지능(intelligence). 양수
        -    def: 캐릭터의 방어력(defence). 양수
        -    hp: 캐릭터의 체력(health point). 양수
        -    mp: 캐릭터의 마나(mana point). 양수

        키는 어떤 순서로도 정렬 x
        예) lvl:10,intel:15,str:28,dex:20,def:14,id:15,hp:100,mp:25
    */
    char buffer[MAX_BUFFER_LENGTH] = { 0, };
    char* token = NULL;
    char* delims = ":,";

    int id;
    int value;

    fgets(buffer, MAX_BUFFER_LENGTH, file_p);
    token = strtok(buffer, delims);

    while (token != NULL) {
        if (strncmp(token, "id", strlen("id")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            id = value;
        } else if (strncmp(token, "lvl", strlen("lvl")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            out_character->level = value;

        } else if (strncmp(token, "intel", strlen("intel")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            out_character->intelligence = value;

        } else if (strncmp(token, "str", strlen("str")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            out_character->strength = value;

        } else if (strncmp(token, "dex", strlen("dex")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            out_character->dexterity = value;

        } else if (strncmp(token, "def", strlen("def")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            out_character->armour = value;

        } else if (strncmp(token, "hp", strlen("hp")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            out_character->health = value;

        } else if (strncmp(token, "mp", strlen("mp")) == 0) {
            token = strtok(NULL, delims);
            sscanf(token, "%d", &value);
            out_character->mana = value;
        }

        token = strtok(NULL, delims);
    }

    sprintf(buffer, "player_%d", id);
    strncpy(out_character->name, buffer, MAX_CHARACTER_NAME_LENGTH);
    out_character->name[MAX_CHARACTER_NAME_LENGTH - 1] = '\0';

    /* transform to ver_2 */
    out_character->evasion = out_character->dexterity / 2;
    out_character->elemental_resistance.cold = out_character->armour / 4;
    out_character->elemental_resistance.fire = out_character->armour / 4;
    out_character->elemental_resistance.lightning = out_character->armour / 4;

    /* transform to ver_3 */
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;
    out_character->elemental_resistance.cold /= 3;
    out_character->elemental_resistance.fire /= 3;
    out_character->elemental_resistance.lightning /= 3;
}

void transform_version_two(FILE* file_p, character_v3_t* out_character)
{
    char buffer[MAX_BUFFER_LENGTH] = { 0, };
    char* token = NULL;
    char* delims = ",";
    size_t perspective_count = 0;

    int value;

    fgets(buffer, MAX_BUFFER_LENGTH, file_p);
    fgets(buffer, MAX_BUFFER_LENGTH, file_p);

    token = strtok(buffer, delims);

    while (token != NULL) {
        switch (perspective_count) {
        case PERSPECTIVE_V2_NAME:
            strncpy(out_character->name, token, MAX_CHARACTER_NAME_LENGTH);
            out_character->name[MAX_CHARACTER_NAME_LENGTH - 1] = '\0';
            break;
        case PERSPECTIVE_V2_LEVEL:
            sscanf(token, "%d", &value);
            out_character->level = value;
            break;
        case PERSPECTIVE_V2_STRENGTH:
            sscanf(token, "%d", &value);
            out_character->strength = value;
            break;
        case PERSPECTIVE_V2_DEXTERITY:
            sscanf(token, "%d", &value);
            out_character->dexterity = value;
            break;
        case PERSPECTIVE_V2_INTELLIGENCE:
            sscanf(token, "%d", &value);
            out_character->intelligence = value;
            break;
        case PERSPECTIVE_V2_ARMOUR:
            sscanf(token, "%d", &value);
            out_character->armour = value;
            break;
        case PERSPECTIVE_V2_EVASION:
            sscanf(token, "%d", &value);
            out_character->evasion = value;
            break;
        case PERSPECTIVE_V2_MAGIC_RESISTANCE:
            sscanf(token, "%d", &value);
            out_character->elemental_resistance.fire = value;
            out_character->elemental_resistance.cold = value;
            out_character->elemental_resistance.lightning = value;
            break;
        case PERSPECTIVE_V2_HEALTH:
            sscanf(token, "%d", &value);
            out_character->health = value;
            break;
        case PERSPECTIVE_V2_MANA:
            sscanf(token, "%d", &value);
            out_character->mana = value;
            break;
        default:
            break;
        }

        perspective_count++;
        token = strtok(NULL, delims);
    }

    /* transform to ver_3 */
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;
    out_character->elemental_resistance.fire /= 3;
    out_character->elemental_resistance.cold /= 3;
    out_character->elemental_resistance.lightning /= 3;
}

void transform_version_three(FILE* file_p, character_v3_t* out_character)
{
    char buffer[MAX_BUFFER_LENGTH] = { 0, };
    char* token = NULL;
    char* delims = " |";

    int value;
    size_t minion_count = 0;
    size_t perspective_count = 0;

    fgets(buffer, MAX_BUFFER_LENGTH, file_p);
    fgets(buffer, MAX_BUFFER_LENGTH, file_p);

    token = strtok(buffer, delims);

    while (token != NULL) {
        switch (perspective_count) {
        case PERSPECTIVE_V3_NAME:
            strncpy(out_character->name, token, MAX_CHARACTER_NAME_LENGTH);
            out_character->name[MAX_CHARACTER_NAME_LENGTH - 1] = '\0';
            break;
        case PERSPECTIVE_V3_LEVEL:
            sscanf(token, "%d", &value);
            out_character->level = value;
            break;
        case PERSPECTIVE_V3_HEALTH:
            sscanf(token, "%d", &value);
            out_character->health = value;
            break;
        case PERSPECTIVE_V3_MANA:
            sscanf(token, "%d", &value);
            out_character->mana = value;
            break;
        case PERSPECTIVE_V3_STRENGTH:
            sscanf(token, "%d", &value);
            out_character->strength = value;
            break;
        case PERSPECTIVE_V3_DEXTERITY:
            sscanf(token, "%d", &value);
            out_character->dexterity = value;
            break;
        case PERSPECTIVE_V3_INTELLIGENCE:
            sscanf(token, "%d", &value);
            out_character->intelligence = value;
            break;
        case PERSPECTIVE_V3_ARMOUR:
            sscanf(token, "%d", &value);
            out_character->armour = value;
            break;
        case PERSPECTIVE_V3_EVASION:
            sscanf(token, "%d", &value);
            out_character->evasion = value;
            break;
        case PERSPECTIVE_V3_FIRE_RES:
            sscanf(token, "%d", &value);
            out_character->elemental_resistance.fire = value;
            break;
        case PERSPECTIVE_V3_COLD_RES:
            sscanf(token, "%d", &value);
            out_character->elemental_resistance.cold = value;
            break;
        case PERSPECTIVE_V3_LIGHTNING_RES:
            sscanf(token, "%d", &value);
            out_character->elemental_resistance.lightning = value;
            break;
        case PERSPECTIVE_V3_LEADERSHIP:
            sscanf(token, "%d", &value);
            out_character->leadership = value;
            break;
        case PERSPECTIVE_V3_MINION_COUNT:
            sscanf(token, "%d", &value);
            out_character->minion_count = value;
            break;
        default:
            break;
        }

        perspective_count++;
        token = strtok(NULL, delims);
    }

    perspective_count = 0;

    if ((out_character->minion_count) != 0) {
        if (out_character->minion_count >= MAX_MINION_COUNT) {
            out_character->minion_count = MAX_MINION_COUNT;
        }

        fgets(buffer, MAX_BUFFER_LENGTH, file_p);

        for (minion_count = 0; minion_count < (out_character->minion_count); minion_count++) {
            fgets(buffer, MAX_BUFFER_LENGTH, file_p);
            
            token = strtok(buffer, delims);
            
            while (token != NULL) {
                switch (perspective_count) {
                case PERSPECTIVE_MINION_V3_NAME:
                    strncpy(out_character->minions[minion_count].name, token, MAX_MINION_NAME_LENGTH);
                    out_character->minions[minion_count].name[MAX_MINION_NAME_LENGTH - 1] = '\0';
                    break;
                case PERSPECTIVE_MINION_V3_HEALTH:
                    sscanf(token, "%d", &value);
                    out_character->minions[minion_count].health = value;
                    break;
                case PERSPECTIVE_MINION_V3_STRENGTH:
                    sscanf(token, "%d", &value);
                    out_character->minions[minion_count].strength = value;
                    break;
                case PERSPECTIVE_MINION_V3_DEFENCE:
                    sscanf(token, "%d", &value);
                    out_character->minions[minion_count].defence = value;
                    break;
                default:
                    break;
                }

                perspective_count++;
                token = strtok(NULL, delims);
            }

            perspective_count = 0;
        }
    }
}
