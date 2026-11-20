#include <kipr/wombat.h>
#include <stdlib.h>

/* Calibration Program for 2-Motor Drive: This program drives both wheels forward
   for a specified test tick count, brings the robot to a stop, reads the encoder
   values, and shows how to calculate TICKS_PER_INCH for gyro-assisted driving. */

// --- Motor Port Assignments ---
// Left Motor = 0, Right Motor = 1
#define L_MOTOR 0
#define R_MOTOR 1

// Test distance in raw encoder ticks
#define TEST_TICKS 3000

int main() {
    printf("Place robot aligned on a reference line.\n");
    printf("Starting Calibration Drive in 2 seconds...\n");
    msleep(2000);

    // 1. Reset both motor position counters to 0
    cmpc(L_MOTOR);
    cmpc(R_MOTOR);

    // 2. Drive both wheels forward at a smooth, constant speed
    while (abs(gmpc(L_MOTOR)) < TEST_TICKS) {
        motor(L_MOTOR, 60);
        motor(R_MOTOR, 60);
    }

    // 3. Stop all motors and let momentum settle
    ao();
    msleep(500);

    // 4. Read the actual tick count recorded by the encoders
    int actual_left_ticks = abs(gmpc(L_MOTOR));
    int actual_right_ticks = abs(gmpc(R_MOTOR));
    int avg_ticks = (actual_left_ticks + actual_right_ticks) / 2;

    printf("\n========================================\n");
    printf("Drive Complete!\n");
    printf("Left Ticks: %d | Right Ticks: %d\n", actual_left_ticks, actual_right_ticks);
    printf("Average Ticks Recorded: %d\n", avg_ticks);
    printf("----------------------------------------\n");
    printf("INSTRUCTIONS:\n");
    printf("1. Measure the exact inches traveled with a ruler.\n");
    printf("2. Divide %d by your measured inches.\n", avg_ticks);
    printf("   Example: If it drove 15.5 inches:\n");
    printf("   TICKS_PER_INCH = %d / 15.5 = %.2f\n", avg_ticks, (float)avg_ticks / 15.5);
    printf("========================================\n");

    return 0;
}