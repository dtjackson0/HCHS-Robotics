#include <kipr/wombat.h>
#include <stdlib.h>

/* Calibration Program for 2-Motor Drive: This program rotates both drive wheels
   in place for a specified test tick count, brings the robot to a complete stop,
   reads the exact encoder values, and calculates both ratios directly on the screen. */

// --- Motor Port Assignments ---
// Left Motor = 0, Right Motor = 1
#define L_MOTOR 0
#define R_MOTOR 1

// Test tick count to turn
// 1000 to 1200 ticks is typically close to a 90-degree turn (adjust for your wheel/gear ratio)
#define TEST_TICKS 1100

int main() {
    printf("Place robot aligned on a reference line.\n");
    printf("Starting pivot test in 2 seconds...\n");
    msleep(2000);

    // 1. Reset both motor position counters to 0
    cmpc(L_MOTOR);
    cmpc(R_MOTOR);

    // 2. Pivot in place: Left forward, Right backward
    while (abs(gmpc(R_MOTOR)) < TEST_TICKS) {
        motor(L_MOTOR, 50);
        motor(R_MOTOR, -50);
    }

    // 3. Stop all motors and let momentum settle
    ao();
    msleep(500);

    // 4. Read the recorded encoder values
    int ticks_l = abs(gmpc(L_MOTOR));
    int ticks_r = abs(gmpc(R_MOTOR));
    int avg_ticks = (ticks_l + ticks_r) / 2;

    printf("\n========================================\n");
    printf("Pivot Test Complete!\n");
    printf("Left Ticks: %d | Right Ticks: %d\n", ticks_l, ticks_r);
    printf("Average Ticks Recorded: %d\n", avg_ticks);
    printf("----------------------------------------\n");
    printf("INSTRUCTIONS:\n");
    printf("1. Measure the exact degrees the robot turned.\n");
    printf("   (Example: If it turned 90.0 degrees)\n");
    printf("2. Degrees Per Tick:\n");
    printf("   Degrees / %d\n", avg_ticks);
    printf("   (e.g., 90.0 / %d = %f)\n", avg_ticks, 90.0 / (double)avg_ticks);
    printf("3. Ticks Per Degree (Standard for Botball):\n");
    printf("   %d / Degrees\n", avg_ticks);
    printf("   (e.g., %d / 90.0 = %.2f)\n", avg_ticks, (double)avg_ticks / 90.0);
    printf("========================================\n");

    return 0;
}