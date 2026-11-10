#include <kipr/wombat.h>
#include <stdlib.h>

/* Calibration Program for 4-Wheel Mecanum Drive: This program rotates all four wheels in place for a specified test tick count,
brings the robot to a complete stop, reads the exact encoder values, and calculates both ratios directly on the screen. */

// --- Motor Port Assignments ---
// Front Left = 0, Front Right = 1, Rear Left = 2, Rear Right = 3
#define LF_MOTOR 0
#define RF_MOTOR 1
#define LR_MOTOR 2
#define RR_MOTOR 3

// Test tick count to turn
// 1000 to 1200 ticks is typically close to a 90-degree turn
#define TEST_TICKS 1100

int main() {
    printf("Place robot aligned on a reference line.\n");
    printf("Starting pivot test in 2 seconds...\n");
    msleep(2000);

    // 1. Reset all motor position counters to 0
    cmpc(LF_MOTOR);
    cmpc(RF_MOTOR);
    cmpc(LR_MOTOR);
    cmpc(RR_MOTOR);

    // 2. Pivot in place: Left side forward, Right side backward
    // Wheels turn in opposite directions to spin the chassis
    while (abs(gmpc(RF_MOTOR)) < TEST_TICKS) {
        motor(LF_MOTOR, 50);
        motor(LR_MOTOR, 50);
        motor(RF_MOTOR, -50);
        motor(RR_MOTOR, -50);
    }

    // 3. Stop all motors and let momentum settle
    ao();
    msleep(500);

    // 4. Read the recorded encoder values
    int ticks_lf = abs(gmpc(LF_MOTOR));
    int ticks_rf = abs(gmpc(RF_MOTOR));
    int avg_ticks = (ticks_lf + ticks_rf) / 2;

    printf("\n========================================\n");
    printf("Pivot Test Complete!\n");
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
