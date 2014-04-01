/* for the definitions of input and output ((f)scanf, (f)printf) */
#include <stdio.h>
/* for the definitions of exit codes and absolute value */
#include <stdlib.h>
/* for assigning (copying) the month string */
#include <string.h>
/*
 * for use to confirm that the return date is a Sunday
 * with Zeller's congruence
 */
#include <assert.h>

/* true value */
#define TRUE        (1)
/* the return value for failure stemming from a range error in the
 * calculate_Easter_date function
 */
#define RANGE_ERROR 0
/* the maximum acceptable year for the algorithm to calculate */
#define MAX_YEAR    39999
/*
 * the minimum acceptable year for the algorithm to calculate
 * (when the current definition of Easter was introduced)
 */
#define MIN_YEAR    1582
/* Sunday's day-of-the-week number in Zeller's congruence */
#define SUNDAY      1
/* the month of the year (1..12) for March and April */
#define MARCH_NUM   3
#define APRIL_NUM   4

/*
 * ALGORITHM (from The Art of Computer Programming by Donald E. Knuth)
 *
 * GIVEN:   Y: the year for which the date of Easter is to be determined.
 * FIND:    The date (month and day) of Easter
 * 
 * STEP E1: Set G to (Y mod 19) + 1.
 * [G is the "golden year" in the 19-year Metonic cycle.]
 * STEP E2: Set C to (Y / 100) + 1. [C is the century]
 * STEP E3: Set X to (3C / 4) - 12. [X is the skipped leap years.]
 * Set Z to ((8C + 5) / 25) - 5.
 * [Z is a correction factor for the moon's orbit.]
 * STEP E4: Set D to (5Y / 4) - X - 10.
 * [March ((-D) mod 7 + 7) is a Sunday.]
 * STEP E5: Set E to (11G + 20 + Z - X) mod 30.
 * If E is 25 and G is greater than 11 or if E is 24,
 * increment E.
 * [E is the "epact" which specifies when a full moon occurs.]
 * STEP E6: Set N to 44 - E.  [March N is a "calendar full moon".]
 * If N is less than 21 then add 30 to N.
 * STEP E7: Set N to N + 7 - ((D + N) mod 7).
 * [N is a Sunday after full moon.]
 * STEP E8: If N > 31 the date is APRIL (N - 31),
 * otherwise the date is MARCH N.
 */

/* function prototypes */
int main(void);
int calculate_Easter_date(int year);
void printDate(int year, int date);
int zellerCongruence(int year, int date);

/*
 * read the years from stdin, print the "year - month day" to stdout
 * return: exit status
 */
int main(void)
{
    /* the year to calculate Easter on */
    int year;
    /* the return value of scanf (EOF at the end of the file) */
    int scan_return;
    /* the day of the month on which Easter falls */
    int easter_date;
    /* the exit status of the program, defaulting to EXIT_SUCCESS */
    int exit_status = EXIT_SUCCESS;
    while TRUE
    {
        /* read an integer from stdin */
        scan_return = scanf("%d", &year);
        /* if the end of the file has been reached, break */
        if (scan_return == EOF) break;
        /* otherwise, calculate the date on which Easter falls */
        easter_date = calculate_Easter_date(year);
        /*
         * if the function returns FALSE (0), an error,
         * then print the error message to stderr
         */
        if (easter_date == RANGE_ERROR)
        {
            fprintf(stderr, "invalid entry date %d", year);
            exit_status = EXIT_FAILURE;
        } else {
            printDate(year, easter_date);
            assert(zellerCongruence(year, easter_date) == SUNDAY);
        }
    }
    /* return the exit status
     * (EXIT_SUCCESS if no problems were encountered)
     */
    return exit_status;
}

/*
 * printDate: print the date given a year and a date
 * arguments: year: the year for the date of Easter
 *            date: the date
 *                  (of format (+/-)day of month, where the + indicates
 *                  April and the - indicates March)
 * return value: none
 */
void printDate(int year, int easter_date)
{
    /*
     * the month on which Easter falls, with a max length of 5 characters
     * (March, April)
     */
    char easter_month[5];
    /*
     * if the date int is greater than zero, the month is April,
     * otherwise, (given that FALSE is 0), the date int is positive
     * and the month is March
     */
    if (easter_date > 0)
    {
        strcpy(easter_month, "April");
    } else {
        strcpy(easter_month, "March");
    }
    /* remove the month determining sign */
    easter_date = abs(easter_date);
    /*
     * print the Easter date to stdin in the format
     * "year - month day"
     */
    printf("%d - %s %d\n", year, easter_month, easter_date);
}

/*
 * caclculate_Easter_date: calculates the date on which easter falls for
 *                         a given year
 * arguments: year: the year (Gregorian) to calculate the date Easter falls on
 * return value: the day on which easter falls (in the format (+/-)date,
 *               where + indicates April, and - indicates March)
 */
int calculate_Easter_date(int year)
{
    /* the golden year (years into the Metonic cycle (1..19)) */
    int golden_year;
    /* the century CE */
    int century;
    /* the number of skipped leap years */
    int skipped_leap_years;
    /* a correction factor for the orbit of the moon */
    int moon_factor;
    /* March (((-sunday_determinant) mod 7) + 7) is a sunday */
    int sunday_determinant;
    /* the epact (days since the moon was new on the 1st Jan that year) */
    int epact;
    /* the date of the calendar full moon for March */
    int calendar_full_moon;
    /* if the year is outside of the predefined range, return a RANGE_ERROR */
    if (year > MAX_YEAR || year < MIN_YEAR) return RANGE_ERROR;
    /* otherwise, preceed with calculation */
    /* the number of years into the 19 year Metonic cycle */
    golden_year = (year % 19) + 1;
    /* the century CE (1st, 2nd ...) */
    century = (year / 100) + 1;
    /*
     * the number of skipped leap years (every multiple of 100 year
     * is skipped, except for those that are multiples of 400
     * (97/100 are skipped))
     * (includes a correction for how many were not skipped up until 1582)
     */
    skipped_leap_years = (3 * century / 4) - 12;
    /* a correction factor for the orbit of the moon */
    moon_factor = ((8 * century + 5) / 25) - 5;
    /* March (((-sunday_determinant) mod 7) + 7) is a sunday */
    sunday_determinant = (5 * year / 4) - skipped_leap_years - 10;
    /* the epact (days since the moon was new on the 1st Jan that year) */
    epact = (11 * golden_year + 20 + moon_factor - skipped_leap_years) % 30;
    /*
     * if the epact is 25 and the golden year is greater than eleven
     * or the epact is 24, increment the epact
     */
    if ((epact == 25 && golden_year > 11) || (epact == 24))
    {
        epact++;
    }
    /* the date of the calendar full moon for March */
    calendar_full_moon = 44 - epact;
    /* if the calendar full moon is less than the 21st, add 30 to it */
    if (calendar_full_moon < 21)
    {
        calendar_full_moon += 30;
    }
    calendar_full_moon = calendar_full_moon + 7
                         - ((sunday_determinant + calendar_full_moon) % 7);
    /*
     * if the calendar full moon is outside of March (beyond the 31st),
     * it is in April, so subtract the days of March (31) from it
     * otherwise, the day is still in March, negate it to reflect this
     */
    if (calendar_full_moon > 31)
    {
        calendar_full_moon -= 31;
    } else {
        calendar_full_moon *= -1;
    }
    /* return the formatted date of Easter */
    return calendar_full_moon;
}

/*
 * zellerCongruence: returns the day of the week number
 *                   for the given date and year
 * arguments: year: the year (Gregorian) to calculate the date Easter falls on
 *            day:  the day on which easter falls (in the format (+/-)date,
 *                  where + indicates April, and - indicates March)
 * return value: the day of the week (Saturday = 0 ... Friday = 7)
 */
int zellerCongruence(int year, int date)
{
    /* the day of the month (1..31) */
    int day_of_month;
    /* the month number (Jan = 1 ... Dec = 12) */
    int month_num;
    /* the year of the century (2013 -> 13) */
    int year_of_century;
    /*
     * century (1st, 2nd ...) (modified from the Zeller value to avoid
     * confusion)
     */
    int century;
    /* the number of the day in the week (Saturday = 0 ... Friday = 7) */
    int day_num;
    /* the day of the month (1..31) */
    day_of_month = abs(date);
    /* the month number (Jan = 1 ... Dec = 12) */
    month_num = date < -1 ? MARCH_NUM : APRIL_NUM;
    /* the year of the century (2013 -> 13) */
    year_of_century = year % 100;
    /*
     * century (1st, 2nd ...) (modified from the Zeller value to avoid
     * confusion)
     */
    century = year / 100 + 1;
    /*
     * the number of the day in the week (Saturday = 0 ... Friday = 7)
     * Zeller's congruence (modified century use to avoid confusion)
     * (also modified for C implementation of mod (%))
     */
    day_num = (day_of_month + (13 * (month_num + 1) / 5)
              + year_of_century + (year_of_century / 4)
              + ((century - 1) / 4) + 5 * (century - 1))
              % 7;
    /* return the day of the week (see above) */
    return day_num;
}
