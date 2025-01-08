Traffic Light Control System with Nabil_RTOS



Overview :-
This project implements a real-time traffic light control system for a four-way intersection using the custom-designed Nabil_RTOS. The system features dynamic traffic light management with pedestrian crossing requests, interrupt handling, and LCD-based notifications.

The project is part of the ReNile embedded systems evaluation process and showcases the ability to integrate custom RTOS features with real-world applications.


Features :-
Real-Time Traffic Light Management: Operates based on a sequence for green, yellow, and red lights.
Pedestrian Request Handling: Processes pedestrian crossing requests via push buttons.
Concurrency Management: Utilizes semaphores and mutexes to synchronize tasks.
LCD Notifications: Displays crossing permissions for pedestrians.
Interrupt-Driven Operation: Handles button interrupts for precise pedestrian request detection.
Scalable Design: Structured to add more features or extend functionality.
System Requirements



Hardware:-
ARM Cortex-M3 Microcontroller (or compatible).
GPIO ports for LEDs and buttons.
LCD display for notifications.
Push buttons for pedestrian requests.

Software:
Custom RTOS: Nabil_RTOS.
Compiler: STM32CUBEIDE.
Debugging tools for ARM Cortex-M3.    ---> used kiel u-vision 5


Project Structure
main.c: The main implementation file containing initialization, task definitions, and system logic.
scheduler.c : My rtos kernel implemntation
GPIO.c : GPIO features implemntation
EXTI.c : EXTI features implemntation
Buttons.c : buttons configuration
LCD.c : Lcd functions
traffic_light.c : traffic light control functions
Nabil_RTOS_FIFO.c : mange tasks fifo for correct scheduling
Cortex_MX_OS_Porting.c : mange core configuration
+ arm auxiliary files CMSIS for ARM Cortex M3 that facilitate core interacion


Header Files:
GPIO.h: GPIO configuration and control.
EXTI.h: External interrupt handling.
traffic_light.h: Traffic light state management.
lcd.h: LCD control and display functions.
buttons.h: Push button initialization and handling.
Nabil_RTOS_FIFO.h: Custom RTOS features for tasks, semaphores, and mutexes.




Usage
Traffic Light Control:
Lights transition automatically based on predefined timings:
North/South green: 40 seconds.
North/South yellow: 10 seconds.
East/West green: 40 seconds.
East/West yellow: 10 seconds.




Pedestrian Requests:
Press the corresponding button for the desired crossing direction.
LCD displays a message when the pedestrian can cross.
System ensures smooth traffic flow by disabling interrupts temporarily after handling a request.



Design Details

Task 1:
Manages the main traffic light sequence.
Monitors the mailbox for pedestrian requests.
Synchronizes with Task 2 using semaphores and mutexes.



Task 2:
Handles pedestrian requests from button interrupts.
Updates the mailbox and notifies Task 1 using semaphores.
Displays crossing permission messages on LCD.


Interrupt Handling:
Four external interrupts handle button presses for each direction.
Temporarily disables interrupts to ensure smooth traffic flow.



Contributing
Contributions are welcome! If you have suggestions for improvements or additional features, please open an issue or submit a pull request.


Author
Ahmed Nabil
Embedded Systems Engineer
Email: [ahmed.nabil.9711@gmail.com]
GitHub: [https://github.com/AhmedNabil97]

