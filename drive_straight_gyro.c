void drive_inches_gyro(int speed, float inches, double bias) {
    float target_ticks = inches * TICKS_PER_INCH;
    double dev = 0.0;

    cmpc(LEFT_MOTOR);
    cmpc(RIGHT_MOTOR);

    while (gmpc(LEFT_MOTOR) < target_ticks) {
        double delta = dev / 5.0; // Gyro sensitivity divisor

        // Steer motors in opposite directions of the drift
        mav(LEFT_MOTOR, speed - delta);
        mav(RIGHT_MOTOR, speed + delta);

        msleep(10);
        dev += gyro_z() - bias; // Integrate heading error
    }
    ao();
    msleep(200);
}