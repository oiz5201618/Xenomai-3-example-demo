#include <stdio.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"

#define TASK_STKSZ	0	//default stack size
#define TASK_PRIO	20	//0-99
#define TASK_MODE	0	//no flags

#define A_PHASE_PIN 20
#define B_PHASE_PIN 21
#define STEP_PIN 5
#define DIR_PIN 6

void motor_control(void *arg);

int main(int argc, char** argv) {
	
    setup_io();

	INP_GPIO(A_PHASE_PIN); 
    INP_GPIO(B_PHASE_PIN);
	INP_GPIO(STEP_PIN);
    OUT_GPIO(STEP_PIN);
    INP_GPIO(DIR_PIN);
    OUT_GPIO(DIR_PIN);

	int i;
	int err;
	
	mlockall(MCL_CURRENT | MCL_FUTURE);
	RT_TASK rt_task;

	err = rt_task_spawn( &rt_task, "rt_motor_control", TASK_STKSZ, TASK_PRIO, TASK_MODE, &motor_control, NULL);
	
	if(err) {
		rt_task_delete(&rt_task);
		exit(1);
	}
    
	getchar();
	return 0;
}

void motor_control(void *arg) {

    int A_curr = 0;
    int B_curr = 0;
    int A_pre = 0;
    int B_pre = 0;

    if( GET_GPIO(20) ){
        if( GET_GPIO(21) ){
            A_pre = 1;                                                                                                    
            B_pre = 1;
        }else{
            A_pre = 1;
            B_pre = 0;
        }
    }else{
        if( GET_GPIO(21) ){
            A_pre = 0;
            B_pre = 1;
        }else{
            A_pre = 0;
            B_pre = 0;
        }
    }

    while(1){
        if( GET_GPIO(20) ){
            if( GET_GPIO(21) ){
                A_curr = 1;
                B_curr = 1;
            }else{
                A_curr = 1;
                B_curr = 0;
            }
        }else{
            if( GET_GPIO(21) ){
                A_curr = 0;
                B_curr = 1;
            }else{
                A_curr = 0;
                B_curr = 0;
            }
        }

        if( A_curr != A_pre || B_curr != B_pre) {
            if( A_pre == 1 && B_pre == 0 ) {
                if( A_curr == 1 ){
                    GPIO_SET = 1 << 6;
                }else{
                    GPIO_CLR = 1 << 6;
                }
            }else if( A_pre == 1 && B_pre == 1 ){
                if( A_curr == 0 ){
                    GPIO_SET = 1 << 6;
                }else{
                    GPIO_CLR = 1 << 6;
                }
            }else if( A_pre == 0 && B_pre == 1 ){
                if( A_curr == 0 ){
                    GPIO_SET = 1 << 6;
                }else{                                                                                                         
                    GPIO_CLR = 1 << 6;
                }
            }else if( A_pre == 0 && B_pre == 0 ){
                if( A_curr == 1 ){
                    GPIO_SET = 1 << 6;
                }else{                                                                                                         
                    GPIO_CLR = 1 << 6;
                }
            }
 
            GPIO_SET = 1 << 5;
			usleep(1);
            GPIO_CLR = 1 << 5;
            
			A_pre = A_curr;
            B_pre = B_curr;
		}
    }
}
