#include "cyber_spaceship.h"

const char* get_longest_safe_zone_or_null(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count, size_t* out_longest_safe_area_length)
{
    /* cluster's scan parameter */
    size_t cab_index = 0;
    size_t cluster_index = 0;
    size_t cluster_find_count = 0;

    size_t is_included = FALSE;

    size_t prev_state = DANGER;

    size_t longest_safe_area_count = 0;
    size_t prev_longest_safe_area_start_index = 0;
    size_t longest_safe_area_start_index = 0;

    size_t prev_longest_safe_area_length = 0;
    size_t longest_safe_area_length = 0;

    /* ���̹� ���༺�븦 �������� �ʾҽ��ϴ� ! */
    if ((cab_start_location == NULL) || (cab_length == 0)) {
        *out_longest_safe_area_length = 0;
        return NULL;
    }

    /* CAB�� cluster���� �������� �ʴ� clean load �Դϴ� ! */
    if (cluster_count == 0) {
        *out_longest_safe_area_length = cab_length;
        return cab_start_location;
    }

    for (cab_index = 0; cab_index < cab_length; cab_index++) {

        while (1) {
            if (cab_index >= cab_length) {
                break;
            }

            for (cluster_index = 0; cluster_index < cluster_count; cluster_index++) {

                is_included = is_cluster_index_in_cab(cab_start_location, cab_length, cab_index, cluster_start_locations[cluster_index], cluster_lengths[cluster_index]);

                /* �ƿ� overlay ���� ���� cluster space�� ������� ����. */
                if (is_included == FALSE) {
                    continue;
                }

                cluster_find_count++;
            }       

            /* safe zone */
            if (cluster_find_count % 2 == 0) {
                if (prev_state == DANGER) {
                    longest_safe_area_count++;
                    prev_longest_safe_area_start_index = cab_index;
                    prev_longest_safe_area_length++;
                    prev_state = TRUE;
                } else {
                    prev_longest_safe_area_length++;
                }

                if (cab_index >= (cab_length - 1)) {
                    if (longest_safe_area_length <= prev_longest_safe_area_length) {
                        longest_safe_area_start_index = prev_longest_safe_area_start_index;
                        longest_safe_area_length = prev_longest_safe_area_length;
                    }

                    break;
                }

                /* danger zone */
            } else {
                if (longest_safe_area_length <= prev_longest_safe_area_length) {
                    longest_safe_area_start_index = prev_longest_safe_area_start_index;
                    longest_safe_area_length = prev_longest_safe_area_length;
                }

                prev_longest_safe_area_length = 0;
                cluster_find_count = 0;

                break;
            }

            cluster_find_count = 0;
            cab_index++;
        }

        prev_state = DANGER;
    }

    if (longest_safe_area_count == 0) {
        *out_longest_safe_area_length = longest_safe_area_length;
        return NULL;
    }
    
    *out_longest_safe_area_length = longest_safe_area_length;

    return (cab_start_location + longest_safe_area_start_index);    
}

int get_travel_time(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count)
{
    size_t cab_index = 0;
    size_t cluster_index = 0;
    size_t cluster_find_count = 0;

    size_t is_included = FALSE;

    size_t danger_area_count = 0;
    size_t safe_area_count = 0;

    double total_travel_time = 0.0;


    /* ���̹� ���༺�븦 �������� �ʾҽ��ϴ� ! */
    if ((cab_start_location == NULL) || (cab_length == 0)) {
        return 0;
    }

    /* CAB�� cluster���� �������� �ʴ� clean load �Դϴ� ! */
    if (cluster_count == 0) {
        total_travel_time = (double)cab_length / 10.0;
        return (int)(total_travel_time + 0.5);
    }

    for (cab_index = 0; cab_index < cab_length; cab_index++) {

        for (cluster_index = 0; cluster_index < cluster_count; cluster_index++) {

            is_included = is_cluster_index_in_cab(cab_start_location, cab_length, cab_index, cluster_start_locations[cluster_index], cluster_lengths[cluster_index]);

            /* �ƿ� overlay ���� ���� cluster space�� ������� ����. */
            if (is_included == FALSE) {
                continue;
            }

            cluster_find_count++;
        }

        /* safe zone */
        if (cluster_find_count % 2 == 0) {
            safe_area_count++;

        } else {
            /* danger zone */
            danger_area_count++;

        }

        cluster_find_count = 0;
    }

    total_travel_time += (double)(danger_area_count) / 5.0;
    total_travel_time += (double)(safe_area_count) / 10.0;

    return (int)(total_travel_time + 0.5);
}

int is_cluster_index_in_cab(const char* const cab_start_location, const size_t cab_length, const size_t cab_index, const char* const cluster_start_location, const size_t cluster_length)
{
    if ((cluster_start_location == NULL) || (cluster_length == 0)) {
        return FALSE;
    }

    /* cluster ���� ��ġ�� CAB�� ���� ��ġ���� �տ� ��ġ�� �� */
    if (cluster_start_location < cab_start_location) {
        /* cluster�� ������ ��ġ�� CAB�� ���� �ȿ� ���� ��. */
        if ((cluster_start_location + cluster_length) <= (cab_start_location + cab_length)) {
            return ((cab_start_location + cab_length) < (cluster_start_location + cluster_length)) ? TRUE : FALSE;
        } else {
            /* cluster�� ������ ��ġ�� CAB�� ���� �ȿ� ���� ��� */
            return FALSE;
        }
    }

    /* cluster ���� ��ġ�� CAB�� ���� ��ġ���� �ڿ� ��ġ�� �� (����� cluster�� �ƴ�) */
    if (cluster_start_location > (cab_start_location + cab_length)) {
        return FALSE;
    }

    return (((cab_start_location + cab_index) >= cluster_start_location) && ((cab_start_location + cab_index) < (cluster_start_location + cluster_length))) ? TRUE : FALSE;
}

void show_cab_cluster_info(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_location, const size_t cluster_lengths[], const size_t cluster_count)
{
    size_t cab_index = 0;
    size_t cluster_number = 0;


}
