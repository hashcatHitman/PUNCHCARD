/**
 * @author Sam K
 * @date October 8th, 2024
 * @name PUNCHCARD
 *
 * @details A simple terminal program meant to simplify calculating work-hours
 * calculations for employees. At the time of writing, employees are required to
 * round their worked hours to the nearest quarter-hour (X.00, X.25, X.50, X.75,
 * X+1.00). This program takes a start time and end time separated by a hyphen
 * and determines the hours worked, assuming the hours worked are less than 24
 * and that the start and end times are always in the order start - end, such
 * that working from 8:00pm-7:59pm is a valid input, suggesting you worked 23
 * hours and 59 minutes. The program will continue to do this repeatedly until
 * stopped. You can stop the program with Ctrl + C, closing the window, or
 * entering the same start and end time.
 */

// Libraries in use:
#include <stdio.h>

// Functions
/**
 * Skips past any unwanted characters between the start time and end time.
 */
void skipBufferJunk() {
    int temporary;
    while ((temporary = getchar()) != '-' && temporary != EOF) {}
}

/**
 * Flushes the input buffer.
 */
void cleanBuffer() {
    int temporary;
    while ((temporary = getchar()) != '\n' && temporary != EOF ) {}
}

/**
 * Attempts to read the next available time from stdin, in the format HH:MMcc,
 * where HH is the hour, MM is the minute, and cc is the meridiem indicator
 * ("am" or "pm").
 *
 * @param hour A pointer to the int storing the hour for this time.
 * @param minute A pointer to the int storing the minute for this time.
 * @param meridiem A pointer to the char storing the meridiem indicator for
 *                 this time.
 *
 * @return 0 if a valid time was read, -1 if something wasn't right with the
 *         read time.
 */
int readTime(int* hour, int* minute, char* meridiem) {
    // Scan the time in.
    scanf_s(" %d : %d %c", hour, minute, meridiem);

    // It's a lot easier if we just convert uppercase to lowercase.
    if (*meridiem == 'A') {
        *meridiem = 'a';
    } else if (*meridiem == 'P') {
        *meridiem = 'p';
    }

    // If we got what looks like a valid time, return 0.
    if (*hour > 0 && *hour < 13 && *minute > -1 && *minute < 60 &&
        (*meridiem == 'a' || *meridiem == 'p')) {
        return 0;
    // Else...
    } else {
        // Print a message explaining what was wrong, return -1.
        if (*hour <= 0) {
            printf_s("[ERROR]\tHOUR TOO SMALL: \"%d\", should be "
                     "greater than 0.\n", *hour);
        }
        if (*hour >= 13) {
            printf_s("[ERROR]\tHOUR TOO BIG: \"%d\", should be less "
                     "than 13.\n", *hour);
        }
        if (*minute <= -1) {
            printf_s("[ERROR]\tMINUTE TOO SMALL: \"%d\", should be "
                     "greater than -1.\n", *minute);
        }
        if (*minute >= 60) {
            printf_s("[ERROR]\tMINUTE TOO BIG: \"%d\", should be less "
                     "than 60.\n", *minute);
        }
        if ((*meridiem != (int) 'a') && (*meridiem != (int) 'p')) {
            printf_s("[ERROR]\tUNRECOGNIZED MERIDIEM: \"%cm\", should "
                     "be \"am\" or \"pm\".\n", *meridiem);
        }
        return -1;
    }
}

/**
 * Converts 12-hour time to 24-hour time, because it is easier to do math with.
 *
 * @param hour A pointer to the int storing the hour for the time to convert.
 * @param meridiem A pointer to the char storing the meridiem indicator for the
 *                 time to convert.
 */
void toMilitaryTime(int* hour, const char* meridiem) {
    // If it's the 12th hour...
    if (*hour == 12) {
        // ...and the am, add 12 so 12am = 24:00/00:00.
        if (*meridiem == 'a') {
            *hour += 12;
        }
    // Else...
    } else {
        // ...if it's the pm, add 12.
        if (*meridiem == 'p') {
            *hour += 12;
        }
    }
}

/**
 * Rounds the time difference to the nearest quarter-hour.
 *
 * @param hourDifference A pointer to the integer storing the hour portion of
 *                       the time spent working.
 * @param minuteDifference A pointer to the integer storing the minutes portion
 *                         of the time spent working.
 */
void roundTime(int* hourDifference, int* minuteDifference) {
    // Round the minutes worked to the nearest multiple of 15.
    *minuteDifference = ((*minuteDifference + 7) / 15) * 15;

    // If rounded to 60 minutes, reset to 0 and increment hours worked.
    if (*minuteDifference == 60) {
        *minuteDifference = 0;
        (*hourDifference)++;
    }
}

/**
 * Gives the user a brief introduction, then prompts the user to enter their
 * start and end times. Calculates the hours worked, and presents the actual
 * work time as well as the rounded hours format. Repeats this process starting
 * from prompting the user until the program is stopped in some way or the user
 * enters the exact same start and end time.
 */
int main(void) {
    // Introduction
    printf_s("\nWelcome to PUNCHCARD! This program is meant to help you "
             "record your work hours\nas an employee. To get started, just "
             "enter your start time and end time, in the\nformat "
             "HH:MMcc-HH:MMcc. For example, if you worked from noon to 3pm "
             "today, you'd\nenter 12:00pm-3:00pm. You can quit the program by "
             "closing this window, pressing\nCtrl + C, or entering a start "
             "time and end time that are identical (such as\n1:00pm-1:00pm)"
             ".\n\n");

    // Until given a reason to stop...
    while (1) {
        // Declare our variables...
        /**
         * The hour work was started at (12-hour time).
         */
        int startHour;

        /**
         * The minute work was started at.
         */
        int startMinute;

        /**
         * The first character of the meridiem indicator for the time work was
         * started at ('a' or 'p').
         */
        char startMeridiem;

        /**
         * The hour work ended at (12-hour time).
         */
        int endHour;

        /**
         * The minute work ended at.
         */
        int endMinute;

        /**
         * The first character of the meridiem indicator for the time work
         * ended at ('a' or 'p').
         */
        char endMeridiem;

        /**
         * The hour value of the difference between the end time and start time.
         */
        int hourDifference;

        /**
         * The minute value of the difference between the end time and start
         * time.
         */
        int minuteDifference;

        /**
         * The time spent working in hours, rounded to the nearest quarter-hour.
         */
        float roundedTime;

        // Prompt the user.
        printf_s("Enter your times:\n");

        // Read the start time. If something went wrong...
        if (readTime(&startHour, &startMinute, &startMeridiem) == -1) {
            // Clean the buffer and try again.
            printf_s("Something was wrong with your given start time!\n");
            cleanBuffer();
            continue;
        }

        // Skip anything particularly annoying between the two times.
        skipBufferJunk();

        // Read the end time. If something went wrong...
        if (readTime(&endHour, &endMinute, &endMeridiem) == -1) {
            // Clean the buffer and try again.
            printf_s("Something was wrong with your given end time!\n");
            cleanBuffer();
            continue;
        }

        // Print the times read back, for confirmation/debugging reasons.
        printf_s("\n");
        printf_s("START:\t%02d:%02d%cm\n", startHour, startMinute,
                 startMeridiem);
        printf_s("END:\t%02d:%02d%cm\n", endHour, endMinute, endMeridiem);

        // If the start time and end time are identical...
        if (startHour == endHour && startMinute == endMinute && startMeridiem
            == endMeridiem) {
            // Stop the program, we're done here.
            break;
        }

        // Convert both times to 24-hour time.
        toMilitaryTime(&startHour, &startMeridiem);
        toMilitaryTime(&endHour, &endMeridiem);

        // Calculate the difference
        hourDifference = endHour - startHour;
        minuteDifference = endMinute - startMinute;

        // If the minutes are less than 0...
        if (minuteDifference < 0) {
            // Tick down an hour and set the minutes to the correct value.
            hourDifference--;
            minuteDifference += 60;
        }

        // If the hour is less than 0...
        if (hourDifference < 0) {
            // Add 24 to find the actual hour.
            hourDifference += 24;
        }

        // Print the time worked.
        printf_s("ACTUAL TIME:\t%02d hours and %02d minutes.\n", hourDifference,
                 minuteDifference);

        // Round to the nearest quarter-hour and print.
        roundTime(&hourDifference, &minuteDifference);
        roundedTime = ((float) minuteDifference / 60) + (float) hourDifference;
        printf_s("ROUNDED TIME:\t%0.2f hours.\n\n", roundedTime);

        // Clean the buffer before the next iteration.
        cleanBuffer();
    }

    // Finish the program.
    return 0;
}
