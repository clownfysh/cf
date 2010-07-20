#ifndef chage_tools_h
#define chage_tools_h

#define CHAGE_CONTAINED_ATOMS 256
#define CHAGE_INTERVAL_MICROSECONDS (1000000.0 / 24)
#define CHAGE_INTERVAL_VOID 255

typedef unsigned char chage_distance_t;
typedef unsigned char chage_interval_t;
typedef unsigned char chage_scale_t;

chage_interval_t chage_determine_interval();

#endif
