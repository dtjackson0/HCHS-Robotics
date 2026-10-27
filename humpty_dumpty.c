#include <kipr/wombat.h>

// Port assignments (adjust ports if your robot is wired differently)
#define LEFT_MOTOR  0
#define RIGHT_MOTOR 3
#define SERVO_PORT  0

// Calibration constants (adjust based on your wheel size and chassis width)
#define TICKS_PER_INCH   150.0  // Measured encoder ticks per inch driven
#define TICKS_PER_DEGREE 12.11  // Encoder ticks per degree for pivot turning

void slow_servo(int port, int target_pos, int delay_ms, int step_size) {
    // 1. Get the current physical position of the servo
    int current_pos = get_servo_position(port);
    
    // 2. Loop until the current position reaches the target position
    while (current_pos != target_pos) {
        if (current_pos < target_pos) {
            // Moving forward / up
            current_pos += step_size;
            if (current_pos > target_pos) {
                current_pos = target_pos; // Cap to prevent overshooting
            }
        } else if (current_pos > target_pos) {
            // Moving backward / down
            current_pos -= step_size;
            if (current_pos < target_pos) {
                current_pos = target_pos; // Cap to prevent overshooting
            }
        }
        
        // 3. Update the servo hardware and pause briefly
        set_servo_position(port, current_pos);
        msleep(delay_ms);
    }
}


// Helper function to drive forward (positive inches) or reverse (negative inches)
void drive_inches(int power, float inches) {
    float target_ticks = inches * TICKS_PER_INCH;
    cmpc(LEFT_MOTOR);  // Clear motor position counter
    cmpc(RIGHT_MOTOR);

    if (inches > 0) { // Drive forward
        while (gmpc(LEFT_MOTOR) < target_ticks) { //
            motor(LEFT_MOTOR, power);              //
            motor(RIGHT_MOTOR, power);
        }
    } else { // Drive in reverse
        while (gmpc(LEFT_MOTOR) > target_ticks) { //
            motor(LEFT_MOTOR, -power);            //
            motor(RIGHT_MOTOR, -power);
        }
    }
    ao();          // Stop all motors
    msleep(200);   // Short pause to settle momentum
}

// Helper function to pivot left in place
void pivot_left(float degrees) {
    float target_ticks = degrees * TICKS_PER_DEGREE; //
    cmpc(RIGHT_MOTOR); //
    while (gmpc(RIGHT_MOTOR) < target_ticks) { //
        motor(LEFT_MOTOR, -50);                //
        motor(RIGHT_MOTOR, 50);
    }
    ao();                                      //
    msleep(200);                               //
}

// Helper function to pivot right in place
void pivot_right(float degrees) {
    float target_ticks = degrees * TICKS_PER_DEGREE; //
    cmpc(LEFT_MOTOR);  //
    while (gmpc(LEFT_MOTOR) < target_ticks) {  //
        motor(LEFT_MOTOR, 50);                 //
        motor(RIGHT_MOTOR, -50);
    }
    ao();                                      //
    msleep(200);                               //
}

int main() {
    enable_servos(); // Enable servo ports

    // Move servo on port 0 to position 430 slowly (10ms delay, 5 ticks per step)
    slow_servo(0, 430, 10, 3);

    // Gently lower the arm back to position 0
    slow_servo(0, 0, 10, 3);

    disable_servos();
    return 0;
}
/*
    // 1. Set the servo at 450
    set_servo_position(SERVO_PORT, 0); // Set servo position
    msleep(500);                          // Wait for servo to reach position

    // 2. Make the robot go forward 2 feet (24 inches)
    drive_inches(50, 24.0); //

    // 3. Pivot left 80 degrees
    pivot_left(80.0); //

    // 4. Pivot back straight (pivot right 80 degrees)
    pivot_right(80.0); //

    // 5. Go forward 6 more inches
    drive_inches(50, 6.0); //

    // 6. Pivot right 80 degrees
    pivot_right(80.0); //

    // 7. Pivot back straight (pivot left 80 degrees)
    pivot_left(80.0); //

    // 8. Reverse 12 inches
    drive_inches(50, -12.0); //

    // 9. Pivot 180 degrees
    pivot_left(180.0); //

    // 10. Set the servo to 430
    set_servo_position(SERVO_PORT, 430); //
    msleep(500);                          //

    // 11. Go straight 36 inches
    drive_inches(50, 36.0); //

    disable_servos(); // Turn off servo ports
    ao();             // Turn off all motors
    return 0;
}*/
