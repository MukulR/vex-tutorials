#include "main.h"

// Controller definition
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Ball detection using vision Sensor
// Uses Object signatures to judge which ball is present in the frame
// This function is designed to print the colored object that is visible in
// that instant's frame. Press y on your controller to do this.
// You may need to re-tune the signatures using the V5 vision utility
// to match with your positioning/light input
// This system is only reliable when properly tuned and at an adequate distance
// from a target, so position it accordingly.

void detect(void *param){
	// Initialize Vision sensor to use port 16 on the cortex
	pros::Vision vision_sensor(16);
	// Declare signatures for the red and blue ball, (I used the v5 vision utility for this, you may have to tune it to fit your needs)
	// This is where you paste the copied codes from the utility. See the bottom for a deeper explanation.
	pros::vision_signature_s_t RED_SIG = pros::Vision::signature_from_utility(1, 6383, 9161, 7772, 295, 1195, 745, 4.700, 0);
	pros::vision_signature_s_t BLUE_SIG = pros::Vision::signature_from_utility(2, -3799, -3103, -3451, 10837, 13745, 12291, 4.500, 0);
	// Make the vision sensor aware of these signatures
	vision_sensor.set_signature(1, &RED_SIG);
	vision_sensor.set_signature(2, &BLUE_SIG);

	// While true, await a press of the Y button
	while (true) {
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
			// When a new press is registered on the button Y, find the biggest object in view that match the two above signatures
			pros::vision_object_s_t obj = vision_sensor.get_by_size(0);
			// If the found object's signature is 1, then its a Red Ball, as defined above.
			// If the found object's signature is 2, then its a Blue Ball, as defined above.
			// If the signature is something other than those two, the sensor was unable to detect those objects.
			if (obj.signature == 1){
				std::cout << "Red Ball" << std::endl;
			} else if (obj.signature == 2) {
				std::cout << "Blue Ball" << std::endl;
			} else {
				std::cout << "Unkown Object" << std::endl;
			}
		}
		// Delay to ensure that the loop doesn't hog.
		pros::Task::delay(10);
	}
}

/* 





/* Usage in driver control:
void opcontrol() {
	pros::Task ball_detection(detect)
}
*/
