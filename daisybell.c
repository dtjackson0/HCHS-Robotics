#include <kipr/wombat.h>
#include <stdlib.h>

// --- Port Assignments ---
// Front Left = 0, Front Right = 1, Rear Left = 2, Rear Right = 3
#define LF_MOTOR         0
#define RF_MOTOR         1
#define LR_MOTOR         2
#define RR_MOTOR         3
#define SERVO_PORT       0

// --- Servo Positions ---
#define RAMP_UP          0
#define RAMP_DOWN        500

// --- Calibration Constants ---
#define TICKS_PER_INCH   205.75   // Measured encoder ticks per inch driven
#define TICKS_PER_DEGREE 21.9   // Encoder ticks per degree for pivot turning
#define DRIVE_VELOCITY   750     // Motor ticks/sec velocity for mav() (max: 1500)

// Measures the resting offset/drift of the Z-axis gyro
double calibrate_gyro_z() {
    int i;
    int iters = 300;
    double total = 0.0;
    
    printf("Calibrating Gyro... DO NOT MOVE ROBOT\n");
    for (i = 0; i < iters; i++) {
        total += gyro_z();
        msleep(3);
    }
    double bias = total / (double)iters;
    printf("Gyro Bias: %f\n", bias);
    return bias;
}

// Drives 4 wheels straight with active gyro heading stabilization
void drive_inches_gyro(int velocity, float inches, double bias) {
    float target_ticks = inches * TICKS_PER_INCH;
    double dev = 0.0;
    
    // Clear all motor counters
    cmpc(LF_MOTOR);
    cmpc(RF_MOTOR);
    cmpc(LR_MOTOR);
    cmpc(RR_MOTOR);

    if (inches > 0) {
        // Forward Drive: All wheels forward
        while (abs(gmpc(LF_MOTOR)) < target_ticks) {
            dev += (gyro_z() - bias);
            
            // Scaled correction with a clamp to prevent over-reacting
            int delta = (int)(dev / 10.0);
            if (delta > 250) delta = 250;
            if (delta < -250) delta = -250;
            
            // Adjust left side vs right side to correct heading
            mav(LF_MOTOR, velocity - delta);
            mav(LR_MOTOR, velocity - delta);
            mav(RF_MOTOR, velocity + delta);
            mav(RR_MOTOR, velocity + delta);
            
            msleep(10);
        }
    } else {
        // Reverse Drive: All wheels backward
        while (abs(gmpc(LF_MOTOR)) < target_ticks) {
            dev += (gyro_z() - bias);
            
            int delta = (int)(dev / 10.0);
            if (delta > 250) delta = 250;
            if (delta < -250) delta = -250;
            
            mav(LF_MOTOR, -velocity - delta);
            mav(LR_MOTOR, -velocity - delta);
            mav(RF_MOTOR, -velocity + delta);
            mav(RR_MOTOR, -velocity + delta);
            
            msleep(10);
        }
    }
    ao();
    msleep(200); // Allow chassis momentum to settle
}

// Pivot left in place using all 4 wheels
void pivot_left(float degrees) {
    float target_ticks = degrees * TICKS_PER_DEGREE;
    cmpc(RF_MOTOR);
    cmpc(LF_MOTOR);
    
    // Left side backward, Right side forward
    while (abs(gmpc(RF_MOTOR)) < target_ticks) {
        motor(LF_MOTOR, -50);
        motor(LR_MOTOR, -50);
        motor(RF_MOTOR, 50);
        motor(RR_MOTOR, 50);
    }
    ao();
    msleep(200);
}

// Pivot right in place using all 4 wheels
void pivot_right(float degrees) {
    float target_ticks = degrees * TICKS_PER_DEGREE;
    cmpc(LF_MOTOR);
    cmpc(RF_MOTOR);
    
    // Left side forward, Right side backward
    while (abs(gmpc(LF_MOTOR)) < target_ticks) {
        motor(LF_MOTOR, 50);
        motor(LR_MOTOR, 50);
        motor(RF_MOTOR, -50);
        motor(RR_MOTOR, -50);
    }
    ao();
    msleep(200);
}
/*
// Smoothly moves servo in increments to prevent abrupt jerking
void slow_servo(int port, int target_pos, int delay_ms, int step_size) {
    int current_pos = get_servo_position(port);

    while (current_pos != target_pos) {
        if (current_pos < target_pos) {
            current_pos += step_size;
            if (current_pos > target_pos) {
                current_pos = target_pos;
            }
        } else if (current_pos > target_pos) {
            current_pos -= step_size;
            if (current_pos < target_pos) {
                current_pos = target_pos;
            }
        }

        set_servo_position(port, current_pos);
        msleep(delay_ms);
    }
}
*/

void servo_arm() {

}
void servo_elbow() {
    
}
void servo_claw() {

}

int main() {
    // 1. Calibrate Gyro while robot is stationary
    double gyro_bias = calibrate_gyro_z();

    // Pivot left 90 degrees
    pivot_right(90.0);
    msleep(150);
    // Drive 7 inches
    drive_inches_gyro(DRIVE_VELOCITY, -9, gyro_bias);
    msleep(150);
    ao();
    return 0;
}
