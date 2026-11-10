#include <kipr/wombat.h>
#include <stdlib.h>
//This code will allow a Wombat with Mecanum wheels to calculate Ticks_Per_Inch to enter as a conastant into your code when using the
//Gyro to move the Wombat.
// --- Motor Port Assignments ---
#define LF_MOTOR 0
#define RF_MOTOR 1
#define LR_MOTOR 2
#define RR_MOTOR 3

// Test distance in raw encoder ticks
#define TEST_TICKS 3000

int main() {
    printf("Starting Calibration Drive in 2 seconds...\n");
    msleep(2000);

    // 1. Reset all four motor position counters to 0
    cmpc(LF_MOTOR);
    cmpc(RF_MOTOR);
    cmpc(LR_MOTOR);
    cmpc(RR_MOTOR);

    // 2. Drive all 4 wheels forward at a smooth, constant speed
    // Use an average or the primary tracking wheel to monitor distance
    while (abs(gmpc(LF_MOTOR)) < TEST_TICKS) {
        motor(LF_MOTOR, 60);
        motor(LR_MOTOR, 60);
        motor(RF_MOTOR, 60);
        motor(RR_MOTOR, 60);
    }

    // 3. Stop all motors and let momentum settle
    ao();
    msleep(500);

    // Read the actual tick count recorded by the encoders
    int actual_left_ticks = abs(gmpc(LF_MOTOR));
    int actual_right_ticks = abs(gmpc(RF_MOTOR));
    int avg_ticks = (actual_left_ticks + actual_right_ticks) / 2;

    printf("\n========================================\n");
    printf("Drive Complete!\n");
    printf("Encoder Ticks Recorded: %d\n", avg_ticks);
    printf("----------------------------------------\n");
    printf("INSTRUCTIONS:\n");
    printf("1. Measure the exact inches traveled with a ruler.\n");
    printf("2. Divide %d by your measured inches.\n", avg_ticks);
    printf("   Example: If it drove 15.5 inches:\n");
    printf("   TICKS_PER_INCH = %d / 15.5 = %.2f\n", avg_ticks, (float)avg_ticks / 15.5);
    printf("========================================\n");

    return 0;
}#include <kipr/wombat.h>
#include <stdlib.h>

// --- Motor Port Assignments ---
#define LF_MOTOR 0
#define RF_MOTOR 1
#define LR_MOTOR 2
#define RR_MOTOR 3

// Test distance in raw encoder ticks
#define TEST_TICKS 3000

int main() {
    printf("Starting Calibration Drive in 2 seconds...\n");
    msleep(2000);

    // 1. Reset all four motor position counters to 0
    cmpc(LF_MOTOR);
    cmpc(RF_MOTOR);
    cmpc(LR_MOTOR);
    cmpc(RR_MOTOR);

    // 2. Drive all 4 wheels forward at a smooth, constant speed
    // Use an average or the primary tracking wheel to monitor distance
    while (abs(gmpc(LF_MOTOR)) < TEST_TICKS) {
        motor(LF_MOTOR, 60);
        motor(LR_MOTOR, 60);
        motor(RF_MOTOR, 60);
        motor(RR_MOTOR, 60);
    }

    // 3. Stop all motors and let momentum settle
    ao();
    msleep(500);

    // Read the actual tick count recorded by the encoders
    int actual_left_ticks = abs(gmpc(LF_MOTOR));
    int actual_right_ticks = abs(gmpc(RF_MOTOR));
    int avg_ticks = (actual_left_ticks + actual_right_ticks) / 2;

    printf("\n========================================\n");
    printf("Drive Complete!\n");
    printf("Encoder Ticks Recorded: %d\n", avg_ticks);
    printf("----------------------------------------\n");
    printf("INSTRUCTIONS:\n");
    printf("1. Measure the exact inches traveled with a ruler.\n");
    printf("2. Divide %d by your measured inches.\n", avg_ticks);
    printf("   Example: If it drove 15.5 inches:\n");
    printf("   TICKS_PER_INCH = %d / 15.5 = %.2f\n", avg_ticks, (float)avg_ticks / 15.5);
    printf("========================================\n");

    return 0;
}
