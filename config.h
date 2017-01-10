
/**
 * All settings are in this order:
 * base_motor, shoulder_motor, elbow_1, wrist_1, wrist_2
 */

const int MOTOR_IDS[] = {1, 2, 3, 4, 5}; // Motor Ids

const int ENCODER_LOWER_LIMIT[] = {0, 0, 0, 0, 0};

const int ENCODER_HIGHER_LIMIT[] = {16383, 16383, 16383, 16383, 16383};


// ToDo
// const int SOFT_LOWER_LIMIT[] = {} // Depending on the physical arrangement of the motor in the arm
// const int SOFT_HIGHER_LIMIT[] = {}
