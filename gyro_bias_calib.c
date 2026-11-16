double calibrate_gyro() {
    int i;
    double sum = 0;
    for (i = 0; i < 50; i++) {
        sum += gyro_z();
        msleep(10);
    }
    return sum / 50.0;
}