#include <kipr/wombat.h>
#include <stdlib.h>

// Port assignments
#define LEFT_MOTOR       3
#define RIGHT_MOTOR      0
#define SERVO_PORT       0

// Servo positions
#define RAMP_UP          0
#define RAMP_DOWN        500

// Calibration constants
#define TICKS_PER_INCH   195.0   // Measured encoder ticks per inch driven
#define TICKS_PER_DEGREE 11.21   // Encoder ticks per degree for pivot turning
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

// Drives straight for a set distance while actively correcting drift using the gyro
void drive_inches_gyro(int velocity, float inches, double bias) {
    float target_ticks = inches * TICKS_PER_INCH;
    double dev = 0.0;
    
    cmpc(LEFT_MOTOR);
    cmpc(RIGHT_MOTOR);

    if (inches > 0) {
        // Forward drive
        while (abs(gmpc(LEFT_MOTOR)) < target_ticks) {
            dev += (gyro_z() - bias);
            
            // Scaled correction with a safety clamp to prevent runaway spinning
            int delta = (int)(dev / 10.0);
            if (delta > 250) delta = 250;
            if (delta < -250) delta = -250;
            
            // Steer against drift (adjust sign if opposite)
            mav(LEFT_MOTOR, velocity - delta);
            mav(RIGHT_MOTOR, velocity + delta);
            
            msleep(10);
        }
    } else {
        // Reverse drive
        while (abs(gmpc(LEFT_MOTOR)) < target_ticks) {
            dev += (gyro_z() - bias);
            
            int delta = (int)(dev / 10.0);
            if (delta > 250) delta = 250;
            if (delta < -250) delta = -250;
            
            mav(LEFT_MOTOR, -velocity - delta);
            mav(RIGHT_MOTOR, -velocity + delta);
            
            msleep(10);
        }
    }
    ao();
    msleep(200); // Settle chassis momentum
}

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

// Pivot left in place (uses abs() so it safely stops regardless of encoder sign)
void pivot_left(float degrees) {
    float target_ticks = degrees * TICKS_PER_DEGREE;
    cmpc(RIGHT_MOTOR);
    cmpc(LEFT_MOTOR);
    while (abs(gmpc(RIGHT_MOTOR)) < target_ticks) {
        motor(LEFT_MOTOR, -50);
        motor(RIGHT_MOTOR, 50);
    }
    ao();
    msleep(200);
}

// Pivot right in place (uses abs() so it safely stops regardless of encoder sign)
void pivot_right(float degrees) {
    float target_ticks = degrees * TICKS_PER_DEGREE;
    cmpc(LEFT_MOTOR);
    cmpc(RIGHT_MOTOR);
    while (abs(gmpc(LEFT_MOTOR)) < target_ticks) {
        motor(LEFT_MOTOR, 50);
        motor(RIGHT_MOTOR, -50);
    }
    ao();
    msleep(200);
}
  
int main() {
    // Calibrate Gyro while robot is stationary
    double gyro_bias = calibrate_gyro_z();

    enable_servos();

    // Move servo up and cycle ramp into starting position
    slow_servo(SERVO_PORT, RAMP_DOWN, 10, 5);
    slow_servo(SERVO_PORT, RAMP_UP, 10, 5);
    msleep(500);

    // Pivot left 90 degrees
    pivot_left(90.0);

    // Drive forward 13.5 inches with gyro stabilization
    drive_inches_gyro(DRIVE_VELOCITY, 13.5, gyro_bias);
    msleep(500);

    // Pivot back straight
    pivot_right(90.0);

    // Lower ramp into position
    slow_servo(SERVO_PORT, RAMP_DOWN, 10, 5);
    msleep(500);

    // Drive forward 14 inches with gyro stabilization
    drive_inches_gyro(DRIVE_VELOCITY, 14.0, gyro_bias);

    // Pivot left 55 degrees (move duck to Zone 2)
    pivot_left(55.0);

    // Pivot straight
    pivot_right(55.0);
    
    // Move servo up
    slow_servo(SERVO_PORT, RAMP_UP, 10, 5);

    msleep(500);
    
    // Drive forward 10 inches with gyro stabilization
    drive_inches_gyro(DRIVE_VELOCITY, 10.0, gyro_bias);

    // Pivot left 50 degrees
    pivot_left(50.0);    
    
    // Move ramp down
    slow_servo(SERVO_PORT, RAMP_DOWN, 10, 5);
    
    msleep(500);
    
    // Pivot right 70 degrees (move duck to Zone 4)
    pivot_right(70.0);
    
    // Move ramp up and weight for loading
    slow_servo(SERVO_PORT, RAMP_UP, 10, 5);
    msleep(500);    

    // Pivot left 200 degrees
    pivot_left(200);    

    // Drive backwards 8 inches with gyro stabilization
    drive_inches_gyro(DRIVE_VELOCITY, -8.0, gyro_bias);    
    
    disable_servos();
    ao();
    return 0;
}
