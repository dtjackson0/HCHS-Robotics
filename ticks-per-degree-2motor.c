#include <kipr/botball.h>
#include <stdio.h>
#include <stdlib.h>

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

// Pivots left (-1) or right (+1) to target_degrees using gyro integration,
// then calculates motor ticks per degree from wheel encoder counts.
void calibrate_ticks_per_degree(int direction, int target_degrees, int turn_speed) {
    long turned_degrees = 0;
    long last_time = systime();

    // Reset motor position counters before pivoting
    cmpc(LEFT_MOTOR);
    cmpc(RIGHT_MOTOR);
    msleep(50);
    last_time = systime();

    while (turned_degrees < target_degrees) {
        long now = systime();
        double dt_sec = (now - last_time) / 1000.0;
        last_time = now;

        int gyro_val = gyro_z();
        // Noise deadband filter matching the gyro pivot logic
        int filtered_rate = (gyro_val > 10 || gyro_val < -10) ? gyro_val : 0;

        // Accumulate absolute rotation toward target_degrees
        turned_degrees += (long)llabs((long)(filtered_rate * dt_sec));

        if (direction < 0) {
            // Pivot left
            mav(LEFT_MOTOR, -turn_speed);
            mav(RIGHT_MOTOR, turn_speed);
        } else {
            // Pivot right
            mav(LEFT_MOTOR, turn_speed);
            mav(RIGHT_MOTOR, -turn_speed);
        }

        msleep(10);
    }

    motor(LEFT_MOTOR, 0);
    motor(RIGHT_MOTOR, 0);
    msleep(200); // Allow wheels to settle before reading counters

    long left_ticks = labs(gmpc(LEFT_MOTOR));
    long right_ticks = labs(gmpc(RIGHT_MOTOR));
    double avg_ticks = (left_ticks + right_ticks) / 2.0;
    double ticks_per_degree = avg_ticks / (double)target_degrees;

    printf("Left ticks: %ld | Right ticks: %ld | Avg ticks: %.1f\n",
           left_ticks, right_ticks, avg_ticks);
    printf(">>> Ticks per degree (over %d deg): %.4f <<<\n\n",
           target_degrees, ticks_per_degree);
}

int main() {
    printf("Keep robot still. Calibrating Gyro...\n");
    gyro_calibrate();
    msleep(1000);
    printf("Calibration complete. Starting pivot tick test...\n");

    // Test a 360-degree pivot (or 90/180) to average tick counts cleanly
    // direction: -1 = left, +1 = right
    calibrate_ticks_per_degree(-1, 360, 400);

    return 0;
}