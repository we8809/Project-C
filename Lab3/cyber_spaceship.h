#ifndef CYBER_SPACESHIP_H
#define CYBER_SPACESHIP_H

#include <limits.h>
#include <stdio.h>

#define DANGER   (0U)
#define SAFE     (1U)

#define TRUE   (1)
#define FALSE  (0)

const char* get_longest_safe_zone_or_null(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count, size_t* out_longest_safe_area_length);
int get_travel_time(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count);
int is_cluster_index_in_cab(const char* const cab_start_location, const size_t cab_length, const size_t cab_index, const char* const cluster_start_location, const size_t cluster_length);

void show_cab_cluster_info(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_location, const size_t cluster_lengths[], const size_t cluster_count);

#endif  /* CYBER_SPACESHIP */
