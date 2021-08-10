// Autogenerated by firmware-libraries/SerialComms/python/cmd_template_parser.py on Tue Aug 10 02:51:24 2021

#include <stdint.h>

// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
#include "autosequence.h"  // for set_state()
#include "serial_data.h"   // flash and telem
#include "globals.h"       // for setting constants
#include "valves.h"        // manual valve control
#include "constants.h"
#include "status_flags.h"
#include "tank_pressure_control.h"
#include "nonvolatile_memory.h"
#include "calibrations.h"
#include "hardware.h"
#include "board_commands.h"

#include "W25N01GV.h"
//#include "L6470.h"


extern W25N01GV_Flash flash;
extern uint8_t telem_disabled;

extern TPC_Info tanks[NUM_TANKS];

extern TIM_HandleTypeDef TIM_MICROS;

// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

void set_vlv(uint8_t* data, uint8_t* status){

	uint32_t vlv_num = (data[3]<<24|data[2]<<16|data[1]<<8|data[0])/1;
	uint8_t state = (data[4])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

	// Don't allow operator intervention during the autosequence
	if (STATE == Ignition ||
			STATE == Hotfire ||
			STATE == Post ||
			STATE == Abort) {
		return;
	}
	else {
		set_valve_channel(vlv_num, state);
	}

	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_kp(uint8_t* data, uint8_t* status){

	uint8_t motor_num = (data[0])/1;
	double gain = (data[8]<<56|data[7]<<48|data[6]<<40|data[5]<<32|data[4]<<24|data[3]<<16|data[2]<<8|data[1])/100.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (motor_num < NUM_MOTORS) {
		tanks[motor_num].K_p = gain;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_ki(uint8_t* data, uint8_t* status){

	uint8_t motor_num = (data[0])/1;
	double gain = (data[8]<<56|data[7]<<48|data[6]<<40|data[5]<<32|data[4]<<24|data[3]<<16|data[2]<<8|data[1])/100.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (motor_num < NUM_MOTORS) {
		tanks[motor_num].K_i = gain;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_kd(uint8_t* data, uint8_t* status){

	uint8_t motor_num = (data[0])/1;
	double gain = (data[8]<<56|data[7]<<48|data[6]<<40|data[5]<<32|data[4]<<24|data[3]<<16|data[2]<<8|data[1])/100.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (motor_num < NUM_MOTORS) {
		tanks[motor_num].K_d = gain;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_state(uint8_t* data, uint8_t* status){

	uint8_t next_state = (data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	manual_state_transition(next_state);
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void download_flash(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	transmit_flash_data();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void wipe_flash(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	erase_flash(&flash);
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void start_logging(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	LOGGING_ACTIVE = 1;
	HAL_GPIO_WritePin(LED_TELEM_PORT, LED_TELEM_PIN, GPIO_PIN_SET);
	add_test_delimiter(&flash);
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void stop_logging(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	LOGGING_ACTIVE = 0;
	finish_flash_write(&flash);
	HAL_GPIO_WritePin(LED_FLASH_LOGGING_PORT, LED_FLASH_LOGGING_PIN, GPIO_PIN_RESET);
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_stepper_pos(uint8_t* data, uint8_t* status){

	uint8_t motor_num = (data[0])/1;
	float position = (data[4]<<24|data[3]<<16|data[2]<<8|data[1])/100.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (motor_num < NUM_MOTORS) {
		manual_stepper_pos_override[motor_num] = 1;
		targetPos[motor_num] = position; // position converted form deg to steps
		curDir[motor_num] = (curPos[motor_num] < targetPos[motor_num]) ? 1 : -1; // CCW facing the motor
		mtr_set[motor_num] = position; // save new motor position setpoint
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_stepper_zero(uint8_t* data, uint8_t* status){

	uint8_t motor_num = (data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (motor_num < NUM_MOTORS) {
	    mtr_pos[motor_num] = 0;
	    curPos[motor_num] = 0;
		//L6470_zero_motor(&(tanks[motor_num].motor));
	}

	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_control_target_pressure(uint8_t* data, uint8_t* status){

	uint8_t tank_num = (data[0])/1;
	float target_pressure = (data[4]<<24|data[3]<<16|data[2]<<8|data[1])/1000.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (tank_num < NUM_TANKS) {
		tanks[tank_num].target_pres = target_pressure;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void ambientize_pressure_transducers(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	for (uint8_t i = 0; i < NUM_PTS; i++) {
		pt_ambients[i] = pressure[i] + pt_ambients[i];
	}
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_low_toggle_percent(uint8_t* data, uint8_t* status){

	uint8_t tank_num = (data[0])/1;
	float lower_threshold_pct = (data[4]<<24|data[3]<<16|data[2]<<8|data[1])/1000.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (tank_num < NUM_TANKS) {
		tanks[tank_num].PID_ctrl_vlv_low_pres = tanks[tank_num].target_pres
				* lower_threshold_pct;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_high_toggle_percent(uint8_t* data, uint8_t* status){

	uint8_t tank_num = (data[0])/1;
	float upper_threshold_pct = (data[4]<<24|data[3]<<16|data[2]<<8|data[1])/1000.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (tank_num < NUM_TANKS) {
		tanks[tank_num].PID_ctrl_vlv_high_pres = tanks[tank_num].target_pres
				* upper_threshold_pct;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_telem(uint8_t* data, uint8_t* status){

	uint8_t state = (data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	// Toggle telem on/off
	telem_disabled = state;
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_presstank_status(uint8_t* data, uint8_t* status){

	uint8_t tank_num = (data[0])/1;
	uint8_t state = (data[1])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (tank_num < NUM_TANKS) {
		tanks[tank_num].tank_enable = state;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void ambientize_pot(uint8_t* data, uint8_t* status){

	uint8_t pot_num = (data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (pot_num < NUM_POTS) {
		// flip pot sign to decouple direction with motor before applying ambient
		pot_ambients[pot_num] = -epot[pot_num]+pot_ambients[pot_num];
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void led_write(uint8_t* data, uint8_t* status){

	uint8_t led_num = (data[0])/1;
	uint8_t state = (data[1])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	switch(led_num) {
	case 0:
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, state);
		break;
	case 1:
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, state);
		break;
	case 2:
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, state);
		break;
	case 3:
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, state);
		break;
	default:
		break;
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_system_clock(uint8_t* data, uint8_t* status){

	uint32_t system_time = (data[3]<<24|data[2]<<16|data[1]<<8|data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	__HAL_TIM_SetCounter(&TIM_MICROS, system_time);
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void remove_pressure_ambients(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	for (uint8_t i = 0; i < NUM_PTS; i++) {
		pt_ambients[i] = 0;
	}
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_fuel_mpv_on_delay(uint8_t* data, uint8_t* status){

	uint8_t mpv_delay_ms = (data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	autosequence.hotfire_fuel_mpv_delay_ms = mpv_delay_ms;
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_engine_test_duration(uint8_t* data, uint8_t* status){

	uint32_t engine_test_duration_ms = (data[3]<<24|data[2]<<16|data[1]<<8|data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	autosequence.hotfire_test_duration_ms = engine_test_duration_ms;
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_pid_start_delay(uint8_t* data, uint8_t* status){

	uint16_t pid_delay_ms = (data[1]<<8|data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	autosequence.hotfire_pid_start_delay_ms = pid_delay_ms;
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_ignitor_on_delay(uint8_t* data, uint8_t* status){

	uint16_t ignitor_on_delay_ms = (data[1]<<8|data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	autosequence.ignition_ignitor_on_delay_ms = ignitor_on_delay_ms;
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_ignitor_high_duration(uint8_t* data, uint8_t* status){

	uint16_t ignitor_high_duration_ms = (data[1]<<8|data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	autosequence.ignition_ignitor_high_duration_ms = ignitor_high_duration_ms;
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_film_cooling_on_delay(uint8_t* data, uint8_t* status){

	uint16_t film_cooling_delay_ms = (data[1]<<8|data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	autosequence.hotfire_film_cooling_on_time_ms = film_cooling_delay_ms;
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_init_motor_deg_factor(uint8_t* data, uint8_t* status){

	double deg_correction_factor = (data[7]<<56|data[6]<<48|data[5]<<40|data[4]<<32|data[3]<<24|data[2]<<16|data[1]<<8|data[0])/10000.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	init_motor_pos_deg_correction_factor = deg_correction_factor;
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void clear_status_flags(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	status_flags = 0;
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void start_simulation(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void advance_simulation(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void stop_simulation(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_pt_lower_voltage(uint8_t* data, uint8_t* status){

	uint8_t pt_num = (data[0])/1;
	float lower_voltage = (data[4]<<24|data[3]<<16|data[2]<<8|data[1])/10.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (pt_num < NUM_PTS) {
		pt_cal_lower_voltage[pt_num] = lower_voltage;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_pt_upper_voltage(uint8_t* data, uint8_t* status){

	uint8_t pt_num = (data[0])/1;
	float upper_voltage = (data[4]<<24|data[3]<<16|data[2]<<8|data[1])/10.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (pt_num < NUM_PTS) {
		pt_cal_upper_voltage[pt_num] = upper_voltage;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_pt_upper_pressure(uint8_t* data, uint8_t* status){

	uint8_t pt_num = (data[0])/1;
	float pressure_range = (data[4]<<24|data[3]<<16|data[2]<<8|data[1])/1.0;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (pt_num < NUM_PTS) {
		pt_cal_upper_pressure[pt_num] = pressure_range;
		save_nonvolatile_variables();
	}
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void refresh_calibrations(uint8_t* data, uint8_t* status){

	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	send_calibration_data();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void send_gse_vlv_cmd(uint8_t* data, uint8_t* status){

	uint32_t vlv_num = (data[3]<<24|data[2]<<16|data[1]<<8|data[0])/1;
	uint8_t state = (data[4])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	send_gse_set_vlv_cmd(vlv_num, state);
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

void set_automatic_abort_enable(uint8_t* data, uint8_t* status){

	uint8_t abort_enable_status = (data[0])/1;
	
	// USER CODE BEGIN - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED
	if (abort_enable_status) {
		autosequence.enable_auto_aborts = 1;
	}
	else {
		autosequence.enable_auto_aborts = 0;
	}
	save_nonvolatile_variables();
	// USER CODE END - MODIFICATIONS OUTSIDE THIS SECTION WILL BE DELETED

}

