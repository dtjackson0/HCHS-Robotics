#include <kipr/botball.h>
#include <stdio.h>
#include <stdlib.h>

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

// Calibrates ticks per inch while using gyro_z() to keep the run straight
void calibrate_ticks_per_inch(int speed, int duration_ms) {
    long start_time = systime();
    long accumulated_error = 0;

    // Reset motor position counters (BEMF ticks) before starting the test run
    cmpc(LEFT_MOTOR);
    cmpc(RIGHT_MOTOR);
    msleep(50);

    while ((systime() - start_time) < duration_ms) {
        int gyro_val = gyro_z();
        accumulated_error += gyro_val;
        int correction = accumulated_error / 40;

        if (speed >= 0) {
            mav(LEFT_MOTOR, speed - correction);
            mav(RIGHT_MOTOR, speed + correction);
        } else {
            mav(LEFT_MOTOR, speed + correction);
            mav(RIGHT_MOTOR, speed - correction);
        }

        msleep(10);
    }

    motor(LEFT_MOTOR, 0);
    motor(RIGHT_MOTOR, 0);
    msleep(200); // Allow wheels to settle before reading counters

    long left_ticks = labs(gmpc(LEFT_MOTOR));
    long right_ticks = labs(gmpc(RIGHT_MOTOR));
    double avg_ticks = (left_ticks + right_ticks) / 2.0;

    double measured_inches = 0.0;
    printf("Left ticks: %ld | Right ticks: %ld | Avg ticks: %.1f\n",
           left_ticks, right_ticks, avg_ticks);
    printf("Measure actual distance traveled (inches): ");
    scanf("%lf", &measured_inches);

    if (measured_inches > 0.0) {
        double ticks_per_inch = avg_ticks / measured_inches;
        printf("\n>>> Ticks per inch: %.4f <<<\n\n", ticks_per_inch);
    } else {
        printf("Invalid distance entered.\n");
    }
}

int main() {
    printf("Keep robot still. Calibrating Gyro...\n");
    gyro_calibrate();
    msleep(1000);
    printf("Calibration complete. Starting straight-line tick test...\n");

    // Adjust speed and duration_ms as needed for a clean measured run
    calibrate_ticks_per_inch(700, 1500);

    return 0;
}