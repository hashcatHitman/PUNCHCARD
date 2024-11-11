# PUNCHCARD
A simple terminal program meant to simplify calculating work-hours calculations
for employees. At the time of writing, employees are required to round their
worked hours to the nearest quarter-hour (X.00, X.25, X.50, X.75, X+1.00). This
program takes a start time and end time separated by a hyphen and determines the
hours worked, assuming the hours worked are less than 24 and that the start and
end times are always in the order start - end, such that working from
8:00pm-7:59pm is a valid input, suggesting you worked 23 hours and 59 minutes.
The program will continue to do this repeatedly until stopped. You can stop the
program with Ctrl + C, closing the window, or entering the same start and end
time.
