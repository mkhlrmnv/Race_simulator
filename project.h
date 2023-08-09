#ifndef _PROJECT__H_
#define _PROJECT__H_

typedef struct // structure for drivers
{
    char *name; // name of driver (arbitrary length)
    char *team; // team of driver (arbitrary length)
    int hour;   // hours of drivers time (non negative value)
    int minute; // minutes of drivers time (value between 0 and 59)
    int second; // second of drivers time (value between 0 and 59)
} Driver;

typedef struct // structure for race (basicaly where all drivers information is stored)
{
    int num_of_drivers;      // number of drivers in array
    Driver *list_of_drivers; // Dynamical array of drivers in current race
} Race;

#endif //! _PROJECT__H_