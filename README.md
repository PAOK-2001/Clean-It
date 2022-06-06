# Clean-It
## System Summary: What can Clean It do?
Clean it is an automatic vacuum cleaner that has an autonomous mode that mimics the algorithm used by the Roomba™ for ensuring maximum coverage of the space. It also has 4 preprogrammed paths to follow:

1. **Empty room**: Move straight until colliding with wall, upon collision it turns aroud 180° while slightly moving to the side and move straight until colliding again. This will repeat until timer is done.

2. **Spill clean**: Follow a spiral path to ensure thorough cleaning around a specific area.

3. **Battery Test**: This is a system validation tool, it simply sets the motors to full capacity for calibration and testing purposes.

4. **Room Outline**: In this mode, Clean It will detect a wall to its left and follow along its perimeter. It will also turn on concave and convex corners. 

Both the autonomous mode as well as each of the patterns can be set to run for a certain amount of time, which can be entered using the keypad on the lid. Additionally, the patterns have a *Deep Clean* mode in which Clean It will stop between movements to ensure it will stay in place longer.

## How does Clean It work?

Clean It has the following key components that make up its functionality:
- **Limit Switches**: Used to detect collisions with walls.
-  **Proximity sensors**: Infrared sensors that output variable voltage depending on the distance.
- **H-Bridge and motors**: These form the drive train of the system, using PWM to control vacuum at different speeds.
- **Keypad and LCD**: used for IO.
-  **Turbine**: a turbine is needed to create the preassure difference necessary to displace air in the intake.
-  **Power Distribution**: Clean It is powered by 3 18650 cells, however 5V are necessary for many of the above components including the microprocessor, thus a 5V regulator was implemented and screw terminals where used to map all the power lines.
      
      <img src="https://user-images.githubusercontent.com/29895011/172117967-681e6bf7-85c3-4d9a-90fb-a7f03895905a.jpg" width=45% height=45%>

-  **KL25Z Board and GPIO Board**: The brains of this project is NXP's KL25Z Freedom Board. To interface the board with the components screw terminals where routed to the GPIO pins using a copper-etched PCB, ensuring a robust system.

      <img src="https://user-images.githubusercontent.com/29895011/172119613-5b08e581-fb3c-4350-b638-d6fa6669bb72.jpg" width=45% height=45%>


## Deep dive into the code

Clean It was built using the KL25Z Freedom Board. The code is structured in layers of abstraction, first including simple headers to interface the sensors and actuators and then processing this data to understand Clean It's surroundings. The end goal for the sensor abstractions was to have functions that detect collisions (using limit switches) and posible collisions (using proximity) in boolean terms. As for the actuators, the movements of the vacuum where expressed in terms of direction and parameterized using timers.

In managing this layered approach, the final driver code for the vacuum is much less complex, and it also encourages further contribution and personalization from the community.

| Abstraction Level | Description |
| --- | -------- |
| Hardware level  | The necessary registers are configured for each sensor and actuator, forming an *init* function for each component. The actual reading and/or writting to the GPIO pins also occurs here. |
| Data in context | Once the sensors have been interfaced, the data is read and used to trigger *flags* depending on the value. For example, the proximity sensors and limit switches work together to form the collision avoidance system. |
| Route Planning | Finally the processed data can be used to encapsulate the routines the vacuum will perform. |

## Demo Reel

https://youtu.be/Ws9m0iw0h-8


