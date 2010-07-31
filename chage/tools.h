#ifndef cf_chage_tools_h
#define cf_chage_tools_h

#define CF_CHAGE_CONTAINED_ATOMS 256
#define CF_CHAGE_INTERVAL_MICROSECONDS (1000000.0 / 24)
#define CF_CHAGE_INTERVAL_VOID 255

typedef unsigned char cf_chage_distance_t;
typedef unsigned char cf_chage_interval_t;
typedef unsigned char cf_chage_scale_t;

cf_chage_interval_t cf_chage_determine_interval();

#endif
