Nabil_RTOS - Real-Time Operating System
Overview
Nabil_RTOS is a custom real-time operating system designed to manage concurrent tasks with efficient scheduling and synchronization. It is built to support embedded systems applications with features such as task scheduling, mutexes, semaphores, and interrupt handling.

This implementation provides a robust platform for developing multi-tasking applications using RTOS concepts, including task creation, termination, priority inheritance, and inter-task communication.

Features
Task Management:

Task creation with stack initialization.
Task suspension, activation, and termination.
Task priority-based scheduling.
Idle task for low-power management.
Scheduler:

Preemptive task scheduling using the PendSV and SysTick interrupts.
Ready queue for managing tasks based on priority and state.
Dynamic priority inversion resolution.
Synchronization:

Mutexes with priority inheritance for preventing deadlocks.
Semaphores for resource locking and signaling.
Interrupt Handling:

Support for enabling and disabling interrupts.
Service Call (SVC) for system operations.
System Tick Timer:

Time-based task waiting using SysTick.
Accurate time management for tasks and delays.
Platform Support:

Designed for ARM Cortex-M architecture.
Uses Main Stack Pointer (MSP) and Process Stack Pointer (PSP) for context switching.
File Structure
scheduler.c: Core RTOS implementation, including task management, synchronization mechanisms, and interrupt handlers.
Nabil_RTOS_FIFO.h: Header for the FIFO buffer used in the ready queue.



API Reference
Task Management
enum RTOS_ERROR Nabil_Rtos_Create_Task(task_ref *p)
Creates a new task with a specific stack size and function pointer.

Parameters:
task_ref *p: Pointer to the task reference.
Returns: NO_ERROR on success or error code.
enum RTOS_ERROR Nabil_Activate_task(task_ref *p)
Activates a task, transitioning it to the ready state.

enum RTOS_ERROR Nabil_Terminate_task(task_ref *p)
Terminates a task, transitioning it to the suspended state.

void Nabil_RTOS_Task_wait(unsigned int no_of_mSecs, task_ref *p)
Puts a task into a blcoking state for a specified duration.

Synchronization
enum RTOS_ERROR Nabil_Rtos_Aquire_Mutex(Mutex_ref *m, task_ref *t)
Acquires a mutex, ensuring exclusive access to a shared resource.

enum RTOS_ERROR Nabil_Rtos_Release_Mutex(Mutex_ref *m)
Releases a mutex, allowing other tasks to access the shared resource.

Scheduler Control
void OS_Start()
Starts the RTOS, initializing the system idle task and scheduler.

void Nabil_RTOS_update_Sch_table()
Updates the scheduling table to determine the next task to execute.

Interrupt Handling
void OS_SVC_Set(SVC_ID index)
Triggers a service call (SVC) for privileged operations.

void SysTick_Handler(void)
Handles the SysTick interrupt for task time management.

void PendSV_Handler(void)
Handles the PendSV interrupt for context switching.

Code Walkthrough
Scheduler
The scheduler manages task transitions between waiting, ready, and running states. It maintains a scheduling table (OS_tasks) and a ready queue for task execution.

Context Switching
Context switching is achieved through the PendSV interrupt, saving the current task's context and restoring the next task's context from its stack.

Priority Inversion
Priority inversion is resolved dynamically by temporarily elevating the priority of the current mutex handler when a higher-priority task requests the mutex.

Limitations and Future Improvements
Limitations:

Supports up to 100 tasks due to static memory allocation.
Designed specifically for ARM Cortex-M architecture.
Future Improvements:

Add dynamic memory allocation for tasks and stacks.
Extend support for additional architectures.
Implement inter-task messaging (message queues).