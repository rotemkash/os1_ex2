/*
* Ex #2a:
 * This program runs in a loop, 10 times.
 * each run starts after the user hits enter.
 * then the user has one second to enter a positive number
 * if the user enters a number the program prints it then it loops again.
 * if the user doesn't manage to enter a number in one second
 *  the program print "):".
 * After the program is done it prints how many numbers the user
 *  managed to enter and the maximum number the user entered.
 *
 * Written by: Rotem Kashani, Mahmood Jazmawy
 *              Id: 209073352, 211345277
* Login: rotemkash, Mahmoodja
*/

//===================== INCLUDES ===========================
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

//============ PROTOTYPES ============================
void catch_alarm();


//==================== MAIN =============================
int main() {
    int i = 0, max = 0;
    int input_num, total = 0;
    struct sigaction act;
    act.sa_handler = catch_alarm;

    // main loop
    while (i < 10) {
        input_num = 0; // init value
        puts("Hit enter when ready");

        // Scheduled alarm after 2 seconds
        while (getchar() != '\n') {}

        sigaction(SIGALRM, &act, NULL); // Register signal action
        alarm(1); // set an alarm for one second

        // ask the user for a number
        scanf("%d", &input_num);
        // num==0 then the user didn't enter a number
        if (input_num > 0) {
            total++;
            if (max < input_num) { // compare the max
                max = input_num;
            }
            printf("%d\n", input_num);
        }
        i++;
    }

    // print the result
    printf("%d %d\n", total, max);
    return EXIT_SUCCESS;
}


// sigaction
void catch_alarm() {
    printf("):\n");
}
