#ifndef __ROBOTCONTROLLER_H__
#define __ROBOTCONTROLLER_H__

#include "SSC32.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

/*                                        E'
                                         / \
          ^ B ^                         E   \
         / / \ \                       / \   \
        / /   \ L2          H         /   \   \
       / /     \ \        X          /     \   \
      / /       \ v       | L4      /       \   \
    L1 /         C--------G--------D         \   \
    / /          <------- L3 ------>          \   \
   / /                                         \   \
  / /_                    ^ z                  _\   F'
 / /  ) theta0            |            theta1 (  \ /  ) theta2
v A-----                O X----> x           -----F-----
  <---------------------- L0 --------------------->
  
* Lengths in millimeters
* Angles in degrees
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

class RobotController
{
public:

	void begin(SSC32& ssc, int servo0, int servo1, int servo2, int servo3);

	void config(float L0, float L1, float L2, float L3, float L4);

	void get_frame(float& theta0, float& theta1, float& theta2, float& theta3);

	void set_frame(float theta0, float theta1, float theta2, float theta3);

	void get_pose(float& x, float& z, float& theta, float &phi);

	void set_pose(float x, float z, float theta, float phi);

	void goto_pose(float x, float z, float theta, float phi, unsigned long time);

private:

	void forward_kinematics();

	void inverse_kinematics();

	SSC32 *m_ssc;
	SSC32::Servo *m_servo0, *m_servo1, *m_servo2, *m_servo3;
	float m_L0, m_L1, m_L2, m_L3, m_L4;
	float m_x, m_z, m_theta, m_phi;

	friend void setup();
	friend void loop();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __ROBOTCONTROLLER_H__

