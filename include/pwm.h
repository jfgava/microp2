// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

/**
 * @file pwm.h
 * 
 * @author Jonas Gava, Juei Weng, Rafael K. Peres
 * 
 * @date 29 Jun 2018
 * 
 * @brief Headers stabilishing the API galileo's PWM.
 * 
 * Interfaces with the /sys/class/pwm/pwmchip0/pwm1 channel.
 * The frequency must be set with the setup() function, with a given starting duty cycle.
 * The frequency ranges from 24Hz to 1525Hz.
 * The duty cycle may be update with the dutyCycle() function. 
 * Further details about the dutyCycle are specified in the ducyCycle().
 * When done, teardown() should be called in order to correctly disable the PWM.
 * 
 */
#ifndef PWM_H
#define PWM_H

#define PWM_1 1
#define PWM_2 3
#define PWM_3 5
#define PWM_4 7
#define PWM_5 11
#define PWM_6 9

/**
 * @brief Sets up PWM for /sys/class/pwm/pwmchip0/pwm1.
 * 
 * Sets up PWM for /sys/class/pwm/pwmchip0/pwm1 with given @p frequency and @p dutyCycle.
 * The duty cycle may still be changed afterwards through the dutyCycle() function.
 * After setup(), once PWM is no more needed, a call to teardown() is required.
 * 
 * @param frequency PWM frequency. Must be greater or equal than 24 and lesser or equal than 1525.
 * @param dutyCycle PWM duty cycle. This value is set before PWM is enabled.
 * @return int Returns 0 on success; -1 on operation failure; -2 on invalid parameters.
 */
int pwmSetup(float frequency, float dutyCycle);

/**
 * @brief Sets PWM duty cycle.
 * 
 * Sets PWM duty cycle output. Expects a relative value between 0 and 1.
 * setup() is required for dutyCycle() to work.
 * 
 * @param value Duty cycle value. Should be between 0 and 1.
 * @return int Returns 0 on success; -1 on operation failure; -2 on invalid parameters.
 */
int pwmDutyCycle(float value);

/**
 * @brief Tears down PWM for /sys/class/pwm/pwmchip0/pwm1.
 * 
 * Tears down PWM for /sys/class/pwm/pwmchip0/pwm1.
 * 
 * @return int Returns 0 on success; -1 on operation failure.
 */
int pwmTeardown();

int servoSetup(int pwm_n, double start_angle);

// Angle in °
int servoSetAngle(int pwm_n, double angle);

#endif
