#define TICKS_PER_DEGREE 12.22 // Replace with your calibrated value via ticks_per_degree_cal.c

void pivot_right(float degrees) {
    float target_ticks = degrees * TICKS_PER_DEGREE; //[cite: 1]
    cmpc(LF_MOTOR);
    cmpc(RF_MOTOR);

    while (abs(gmpc(LF_MOTOR)) < target_ticks) {
        motor(LF_MOTOR, 50);
        motor(LR_MOTOR, 50);
        motor(RF_MOTOR, -50);
        motor(RR_MOTOR, -50);
    }
    ao();
    msleep(200);
}
