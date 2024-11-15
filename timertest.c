#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void timer_handler(int signum) {
    static int count = 0;
    // printf("Timer fired %d times\n", ++count);
    count++;
}

int main() {
    struct sigaction sa;
    struct itimerval timer;

    // Install timer_handler as the signal handler for SIGALRM.
    sa.sa_handler = &timer_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    // Configure the timer to expire after 1ms.
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000;

    // Configure the timer to reset to 1ms after expiring.
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;

    // Start the timer.
    setitimer(ITIMER_REAL, &timer, NULL);

    // Do something while waiting for the timer to expire.
    while (1) {
        pause(); // Wait for signal
    }

    return 0;
}

// $ gcc -O3 timertest.c -o timertest
// $ taskset -c 1 ./timertest 
// 
// - results on Intel(R) Core(TM) i5-2520M
// 
// interval	util(%)
// --------------------
// 1 us		100	
// 10 us	60	
// 100 us	20	
// 1 ms		3		3.3%	 
// 10 ms	1		0.6 - 1.3% 
// 100 ms   	0		0.0% 	
// -------------------
// 200 us	10
// 400 us	8 
// 600 us	5
// 800 us	4
//
// - results on Intel(R) Celeron(R) CPU  N2807  @ 1.58GHz
//
// interval	util(%)
// --------------------
// 10 us 	100
// 100 us	20
// 1 ms		3
