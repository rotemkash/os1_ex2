/*
 *EX2B:
In this program the father gives birth to a child. 
Before it gives birth to a child, it sets a signal handler that will be 
activated when the signals SIGUSR1, SIGUSR2 are sent to the process.
Now in an infinite loop each of them: 
a.Goes to sleep for a random amount of time of at most 2 seconds.
b.Grills a number between 0 and 100 including 0 and including 100.
c.If he grills a thousand numbers >= 95, he announces: I win, 
sends the SIGUSR1 signal to the second process and finishes.
d.If he grills sixty thousand numbers < 55, he announces: I lose, 
sends the SIGUSR2 signal to the second process and finishes.
Each of the two processes in the signal handler performs:
A. If received SIGUSR1 notifies: congrats! Shows how many 
numbers >= 95 and how many numbers < 55 is the grill and finishes.
B. If received the SIGUSR2 informs: thanks! Shows how many 
numbers >= 95 and how many numbers < 55 is the grill and finishes
possible race modes between the father and son:
1: the father wins or loses and completes all the prints then it exits the program.
2: the son wins/loses and completes all of it’s prints, kills the process and exits.
3: the father and son win/lose at the same time so one of them prints
“I win”/ “I lose” but doesn’t get enough time to kill the process so
 other one prints and kills the process
 * Written by: Rotem Kashani, Mahmood Jazmawy
 *              Id: 209073352, 211345277
* Login: rotemkash, Mahmoodja
*/

//===================== INCLUDES ===========================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//==================== CONSTS =================================
pid_t son_pid, father_pid;
int WIN = 1000, LOSE = 60000;
int counter_win, counter_lose;

//============ PROTOTYPES ============================
void do_son(pid_t pid, pid_t other_pid);

void do_father(pid_t pid, pid_t other_pid);

void signal_handler(int signo);


//=================== MAIN ===========================
int main() {
    // register signals
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    counter_win = counter_lose = 0; // counters

    pid_t son;
    son = fork();
    father_pid = getpid();
    son_pid = son;

    // if fork failed
    if (son < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (son == 0) { // if the current proccess is the son
        do_son(son_pid, father_pid);
    } else { // if it's the father
        do_father(father_pid, son_pid);
    }

    return EXIT_SUCCESS;
}


/**
 * This function is responsible of running the son
 */
void do_son(pid_t pid, pid_t other_pid) {
    int num;
    srand(18); // set seed for srand
    while (1) {
        sleep(rand() % 3); // sleep random time
        num = rand() % 101;
        if (num >= 95) { // if the random number was >=95
            counter_win++;
            if (counter_win == WIN) { // if we reached 95 numbers above 95
                printf("I win\n"); // print win
                kill(other_pid, SIGUSR1); // kill process
                exit(EXIT_SUCCESS);
            }
        } else if (num <= 55) { // if the number is lower than 55
            counter_lose++;
            if (counter_lose == LOSE) {
                printf("I lose\n"); // print lose
                kill(other_pid, SIGUSR2); // kill process
                exit(EXIT_SUCCESS);
            }
        }
    }
}


/**
 * This function is responsible of running the father
 */
void do_father(pid_t pid, pid_t other_pid) {
    int num;
    srand(17); // set seed for srand
    while (1) {
        sleep(rand() % 3); // sleep a random time
        num = rand() % 101;
        if (num >= 95) { // if the random number was >=95
            counter_win++;
            if (counter_win == WIN) {
                printf("I win\n"); // print in
                kill(other_pid, SIGUSR1); // kill process
                exit(EXIT_SUCCESS);
            }
        } else if (num <= 55) { // if the number if lower than 55
            counter_lose++;
            if (counter_lose == LOSE) {
                printf("I lose\n"); // print lose
                kill(other_pid, SIGUSR2); // kill process
                exit(EXIT_SUCCESS);
            }
        }
    }
}

void signal_handler(int signo) {
    // if the signal was SIGUSR1 congrat them
    if (signo == SIGUSR1) {
        printf("congrats! %d, %d\n", counter_win, counter_lose);
        exit(EXIT_SUCCESS);
    }
    // if the signal was SIGUSR2 thanks him for losing
    if (signo == SIGUSR2) {
        printf("thanks! %d, %d\n", counter_win, counter_lose);
        exit(EXIT_SUCCESS);
    }
}

