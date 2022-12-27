#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/driver.h"
#include "../include/ride.h"
#include "../include/catalog.h"
#include "../include/query9_stats.h"
#include "../include/utils.h"
struct stat
{
	char *date_a;
	char *date_b;
	CATALOG *c;
	GHashTable *ht;

	char *id;
	char *date;
	char *city;
	int distance;
	double tip;
};

char *get_query9_stat_id(QUERY9_STAT *s)
{
    return strdup(s->id);
}

char *get_query9_stat_date(QUERY9_STAT *s)
{
    return strdup(s->date);
}

char *get_query9_stat_city(QUERY9_STAT *s)
{
	return strdup(s->city);
}

int get_query9_distance(QUERY9_STAT *s)
{
    return s->distance;
}

double get_query9_tip(QUERY9_STAT *s)
{
	return s->tip;
}

void destroy_query9_stat(void *v)
{
    QUERY9_STAT *s = v;

    free(s->id);
    free(s->date);
    free(s->city);
    free(s);
}

void build_query9_stat(gpointer key, gpointer value, gpointer userdata)
{
	RIDE *r = value;
	QUERY9_STAT *s = userdata;
	double tip = get_ride_tip(r);
	char *date = get_ride_date(r);

	if ((tip > 0) && (compare_dates(s->date_a, date) <= 0) && (compare_dates(date, s->date_b) <= 0))
		{
			QUERY9_STAT *ride_stat = malloc(sizeof(QUERY9_STAT));

			ride_stat->id = get_ride_id(r);
			ride_stat->date = date;
			ride_stat->city = get_ride_city(r);
			ride_stat->distance = get_ride_distance(r);
			ride_stat->tip = tip;

			g_hash_table_insert(s->ht, ride_stat->id, ride_stat);
		}
}

void create_query9_stats(GHashTable *query9_stats, char *date_a, char *date_b, CATALOG *c)
{
	QUERY9_STAT *s = malloc(sizeof(QUERY9_STAT));

	s->ht = query9_stats;
	s->date_a = date_a;
	s->date_b = date_b;
	s->c = c;

	g_hash_table_foreach(get_catalog_rides(c), build_query9_stat, s);

	free(s);
}