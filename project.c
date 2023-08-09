#include "project.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <printf.h>

/**
 * \brief Funktion checks if driver with specific lastname is existing (returns 1 if driver with same name exist and 0 if doen't)
 *
 * \details Funktion does
 * 1. Firstly checks if there is any drivers (returns 0 and aborts if there is not)
 * 2. Goes throught whole driver list and compares drivers lastname to gives lastname
 *      a. If lastnames are same funktion returns 1 and aborts
 *
 * \param race the race that is going
 * \param name name of driver that we are trying to find
 */
int found(Race *race, char *name)
{
    if (race->num_of_drivers != 0)
    {
        for (int i = 0; i < race->num_of_drivers; i++)
        {
            if (!strcmp(race->list_of_drivers[i].name, name))
            {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * \brief Creates new driver to dynamical array in structure Race
 *
 * \details Function does:
 * 1.Checks with found funktion if driver with same lastname is already existing in a system
 * and prints message if found funktion returns true value
 * 2. Funktion intialize new driver and gives name, team and racing time (racing time is always 0 0 0 at the start)
 * 3. Then funktion allocates memmory in dynamical array for driver and checks if that was succesfull (print error message if not)
 * 4. Coppies new driver to just allocated place and updates number of drivers
 * 5. Prints SUCCESS message
 *
 * \param race The current race
 * \param name Name of new driver
 * \param team Team of new driver
 */
void add_driver(Race *race, char *name, char *team)
{
    if (found(race, name)) // Funktion first checks if driver is already existing and if it is then funkion returns error message
    {
        printf("Driver %c%s%c is already in the database.\n", '"', name, '"');
        return;
    }

    Driver driver;

    driver.name = NULL;
    driver.team = NULL;

    driver.name = malloc(strlen(name) + 1); // allocating memmory for name and team
    driver.team = malloc(strlen(team) + 1);

    if (driver.name == NULL || driver.team == NULL)
    { // checks that allocating was successfull and frees memmory if not
        printf("Error: not enought memmory\n");
        free(driver.name);
        free(driver.team);
        return;
    }

    strcpy(driver.name, name); // Then funktion initialize new driver
    strcpy(driver.team, team);
    driver.hour = 0;
    driver.minute = 0;
    driver.second = 0;

    race->list_of_drivers = realloc(race->list_of_drivers, (((size_t)race->num_of_drivers + 1) * sizeof(Driver))); // allocates memory for new driver

    if (race->list_of_drivers == NULL)
    {
        printf("Error: not enought memmory\n"); // checks if memmory allocation was succesful
        free(driver.name);
        free(driver.team);
        return;
    }

    memcpy(race->list_of_drivers + race->num_of_drivers, &driver, sizeof(Driver)); // coppies memmory to just allocated place

    race->num_of_drivers++; // update number of drivers
    printf("SUCCESS\n");
}

/**
 * \brief Checks if enter time is valid (returns 1 if time is not valid and 0 if time is valid)
 *          Time is valid if
 *          a. Hour is zero of positive number
 *          b. Minute and second value is between 0 and 59
 *
 * \details Funktion does:
 * 1. First there is if statement that checks if any of values in non valid
 * 2. If if statemt returns true value funktion with other if statements checks whitch value is non valid and prints
 * message about it
 *
 *
 * \param hour Given hour value
 * \param minute Given minute value
 * \param second Given second value
 */
int invalid_time(int hour, int minute, int second) // funktion checks if time is valid
{                                                  // time is valit if hour is not negative value and if minutes and second are in a range from 0 to 59
    if (hour < 0 || minute < 0 || second < 0 || minute > 59 || second > 59)
    {
        if (hour < 0)
        {
            printf("Hour cannot be negative.\n"); // if time is not valid funktion prints whats wrong with it
        }
        if (minute < 0 || minute > 59)
        {
            printf("Minute cannot be negative or greater than 59.\n");
        }
        if (second < 0 || second > 59)
        {
            printf("Second cannot be negative or greater than 59.\n");
        }
        return 1;
    }
    return 0;
}

/**
 * \brief Funktion updates time of given driver with given values
 *
 * \details Funktion does:
 * 1. Firstly funktion checks if there is any drivers at all existing (aborts if there is not)
 * 2. Checks if there is driver with given lastname (aborts if there is not)
 * 3. Checks if given time is valid with funktion invalid time (aborts if time is invalid)
 * 4. Then funktion finds right driver and updates drivers time
 * 5. Funktion checks if drivers time minute or second value is over 59 and corrects it if they are
 * 6. Prints SUCCESS if everything was succesful
 *
 * \param race Current race
 * \param name Lastname of driver
 * \param hour Given hour value
 * \param minute Given minute value
 * \param second Given second value
 */
void update_time(Race *race, char *name, int hour, int minute, int second)
{
    if (race->num_of_drivers == 0) // funktion checks if there is any drivers and if there not returns error messafe
    {
        printf("ERROR: Add drivers first\n");
        return;
    }

    if (!found(race, name)) // funktion checks if there is driver with given lastname and if there is not then returns error message
    {
        printf("Driver %c%s%c is not in the database.\n", '"', name, '"');
        return;
    }

    if (invalid_time(hour, minute, second)) // checks if entered by user time is valid and if not aborts funktion
    {
        return;
    }

    for (int i = 0; i < race->num_of_drivers; i++) // firstly funktion finds driver's index in array
    {
        if (!strcmp(race->list_of_drivers[i].name, name))
        {
            Driver *driver = &race->list_of_drivers[i]; // then for simplicity initialize it for simpler variable
            driver->hour = driver->hour + hour;
            driver->minute = driver->minute + minute; // updates time
            driver->second = driver->second + second;

            while (driver->second > 60) // funktion checks if drives second are over 60 and if they are then changes them for minutes
            {
                driver->second = driver->second - 60;
                driver->minute++;
            }

            while (driver->minute > 60) // same thing but with minutes and hours
            {
                driver->minute = driver->minute - 60;
                driver->hour++;
            }
            break;
        }
    }
    printf("SUCCESS\n");
}

/**
 * \brief Funktion prints all drivers in sorted order from driver with least time to driver with most time
 *
 * \details Funktion does
 * 1. Firstly checks if there is any drivers (aborts if there is not)
 * 2. Then sorts drivers in right order
 *      a. Funktion takes two drivers with two for loops
 *      b. Then gives drivers their total time driven in seconds
 *      c. Compares two drivers and swithces them second one (j) total time value is smaller then first ones(i)
 * 3. Then funktion prints out all drivers on separet lines in format of "lastname team hour minute second"
 * 4. Prints SUCCESS if everything was successfull
 *
 * \param race Current race
 */
void print_race(Race *race)
{
    if (race->num_of_drivers == 0) // checks if there are any drivers existing and if there are not then prints error message
    {
        printf("ERROR: Add drivers first\n");
        return;
    }

    for (int i = 0; i < race->num_of_drivers; i++)
    { // this sorts drivers by their total time
        for (int j = i + 1; j < race->num_of_drivers; j++)
        {
            int total_time_i = race->list_of_drivers[i].hour * 3600 + race->list_of_drivers[i].minute * 60 + race->list_of_drivers[i].second;
            int total_time_j = race->list_of_drivers[j].hour * 3600 + race->list_of_drivers[j].minute * 60 + race->list_of_drivers[j].second;
            if (total_time_j < total_time_i)
            {
                Driver temp = race->list_of_drivers[i];
                race->list_of_drivers[i] = race->list_of_drivers[j];
                race->list_of_drivers[j] = temp;
            }
        }
    }

    for (int i = 0; i < race->num_of_drivers; i++) // then for every driver in array prints their information
    {
        Driver *driver = &race->list_of_drivers[i];
        printf("%s %s %d %d %d\n", driver->name, driver->team, driver->hour, driver->minute, driver->second);
    }

    printf("SUCCESS\n");
}

/**
 * \brief Funktion writes all drivers information in a file
 *
 * \details Funtkion does
 * 1. Open a file with given name (prints message if that wasn't succesful and aborts)
 * 2. For every driver prints their information in a file in format of "lastname team hour minute second"
 * 3. Prints SUCCESS if everything was successful and closes file
 *
 * \param race Currect race
 * \param file_name Name of file where race should be saved
 */
void write_to_file(Race *race, char *file_name)
{
    FILE *file = fopen(file_name, "w"); // firstly opens file
    if (file == NULL)
    {
        printf("Cannot open file %s for writing.\n", file_name);
        return;
    }

    for (int i = 0; i < race->num_of_drivers; i++) // then for every driver writes their information to file
    {
        Driver *driver = &race->list_of_drivers[i];
        fprintf(file, "%s %s %d %d %d\n", driver->name, driver->team, driver->hour, driver->minute, driver->second);
    }
    printf("SUCCESS\n");
    fclose(file); // closes file
}

/**
 * \brief Funktion loads new race from given file and replace ongoing race with loaded one
 *
 * \details Funktion does
 * 1. Open a file with given name (prints message if that wasn't succesful and aborts)
 * 2. Frees memmory that was allocated for old race
 * 3. With while loop and fgets takes one line at the time from file
 *      a. Initialize variables for name, team and time variables
 *      b. Scans just got line from file and checks if there are exactly 5 arguments (print error if not)
 *      c. Allocates memmor firstly for new drivers information and coppies just got information to new driver (prints error if allocating was not successful)
 *      d. Allocates memmory for new driver in dynamical array and coppies new driver to array and updates number of drivers (prints error if allocating was not successful)
 * 4. Closes file and prints SUCCESS
 *
 * \param race Currect race
 * \param file_name Name of file from where race should be loaded
 */
void load_from_file(Race *race, char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL) // opens file and checks if that was successful
    {
        printf("Cannot open file %s for reading.\n", file_name);
        return;
    }

    for (int i = 0; i < race->num_of_drivers; i++)
    {
        free(race->list_of_drivers[i].name); // frees memmory used by old race
        free(race->list_of_drivers[i].team);
    }
    free(race->list_of_drivers);
    race->list_of_drivers = NULL;
    race->num_of_drivers = 0;

    char line[1000]; // variable for scanign files line (was given that maximun length is 1000 characters)
    while (fgets(line, sizeof(line), file))
    {
        char *name = NULL;
        char *team = NULL; // initialize variables
        int hour, minute, second;

        name = malloc(strlen(line) * sizeof(char)); // allocates memory for name and team variables
        team = malloc(strlen(line) * sizeof(char));

        if (name == NULL || team == NULL) // checks if allocating happend (frees memmory and aborts funktion if not)
        {
            printf("ERROR: Not enought memmory\n");
            free(name);
            free(team);
            return;
            break;
        }

        if (sscanf(line, "%s %s %d %d %d", name, team, &hour, &minute, &second) == 5) // scans line from file for variables (checks that there is 5 arguments)
        {
            Driver new_driver;
            new_driver.name = malloc(strlen(name) + 1); // initialize new driver and allocates memmory for name and team of driver
            new_driver.team = malloc(strlen(team) + 1);

            if (new_driver.name == NULL || new_driver.team == NULL)
            { // checks if allocating happend (frees memmory and aborts funktion if not)
                printf("ERROR: Not enought memmory\n");
                free(name);
                free(team);
                return;
                break;
            }

            strcpy(new_driver.name, name); // coppies name and team to new driver
            strcpy(new_driver.team, team);

            free(name); // frees allocated memmory after copying name and team to driver
            free(team);

            new_driver.hour = hour;
            new_driver.minute = minute; // coppies time of driver
            new_driver.second = second;

            race->list_of_drivers = realloc(race->list_of_drivers, sizeof(Driver) * (race->num_of_drivers + 1)); // allocates memmory in dynamical array
            memcpy(race->list_of_drivers + race->num_of_drivers, &new_driver, sizeof(Driver));                   // coppies memmory to just allocated place
            race->num_of_drivers++;                                                                              // updates number of drivers
        }
        else
        { // if there were not exactly 5 arguments prints error about it and aborts funktion
            printf("ERROR: Wrong ammount of commands\n");
            return;
            break;
        }
    }
    fclose(file); // closes file
    printf("SUCCESS\n");
}

int main()
{
    Race race; // initialize new race
    memset(&race, 0, sizeof(race));

    race.num_of_drivers = 0;
    while (1)
    {
        char buffer[1000]; // new variable where users input is stored. Was sad that max lenght of input is 1000 char

        fgets(buffer, sizeof(buffer), stdin); // takes users input

        char command = buffer[0]; // recognize what is users command
        char *name = NULL;
        char *team = NULL; // initilize variables
        int hour, minute, second;
        char *file_name = NULL;

        switch (command) // switch case for users command
        {
        case 'A':
            name = malloc(strlen(buffer) * sizeof(char)); // allocates memory for name and team.
            team = malloc(strlen(buffer) * sizeof(char));

            if (sscanf(buffer, "%*c %s %s", name, team) == 2) // scan for name and team, and if there are to many argument return error message in else case
            {
                add_driver(&race, name, team);
            }
            else
            {
                printf("A should be followed by exactly 2 arguments.\n");
            }
            free(name); // frees alocated memmory for name and team
            free(team);
            break;
        case 'U':
            name = malloc(strlen(buffer) * sizeof(char)); // allocats memmory with same idea asin case 'A', but only for name

            if (sscanf(buffer, "%*c %s %d %d %d", name, &hour, &minute, &second) == 4) // scan for name and time, and if there are to many argument return error message in else case
            {
                update_time(&race, name, hour, minute, second);
            }
            else
            {
                printf("U should be followed by exactly 4 arguments.\n");
            }
            free(name); // frees name memmory
            break;
        case 'L':
            print_race(&race);
            break;
        case 'W':
            file_name = malloc(strlen(buffer) * sizeof(char)); // allocates memmory with same logic that little bit upper

            if (sscanf(buffer, "%*c %s", file_name) == 1) // scans for file name and if there are wrong ammounts of commands print error message
            {
                write_to_file(&race, file_name);
            }
            else
            {
                printf("W should be followed by exactly 1 argument\n");
            }
            free(file_name);
            break;
        case 'O':
            file_name = malloc(strlen(buffer) * sizeof(char)); // allocates memmory with same logic that little bit upper

            if (sscanf(buffer, "%*c %s", file_name) == 1) // scans for file name and if there are wrong ammounts of commands print error message
            {
                load_from_file(&race, file_name);
            }
            else
            {
                printf("O should be followed by exactly 1 argument\n");
            }
            free(file_name);
            break;
        case 'Q':
            printf("SUCCESS\n"); // says ggood bye to user

            for (int i = 0; i < race.num_of_drivers; i++)
            {
                free(race.list_of_drivers[i].name);
                free(race.list_of_drivers[i].team);
            }

            free(race.list_of_drivers); // frees memmory
            return 0;                   // brakes while loop
            break;
        default:
            printf("Invalid command %c\n", command); // prints that command is wrong
            break;
        }
    }
    return 0;
}