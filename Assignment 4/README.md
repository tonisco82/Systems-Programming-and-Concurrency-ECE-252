# ECE 252 Assignment 4: Hogwarts School of Witchcraft and Wizardry

### Intended to be completed after lecture: 19

At Hogwarts School of Witchcraft and Wizardry, undesirable tasks are done by House
Elves, magical creatures who apparently are not covered under any sort of employment
standards or workers rights legislation. They work as a team under the direction of
Dobby, who is a free elf. At the beginning of the day, the headmaster, Dumbledore, tells Dobby how many tasks there are to do today. Dobby will communicate with Dumbledore about what the specific tasks to do are, and posts a partial list of items for the other elves to do. The elves take tasks from the list and do them. When the list is empty, Dobby asks for more directions and posts more tasks. This continues until the house elves have finished all the tasks for the day.

### Primary Objective
The primary objective of this exercise is to practice using synchronization constructs such as semaphores and mutexes in a simple C program.

## Description of Behaviour
The goal is to implement the program so that the following behaviour occurs:

*  At program start, the first argument provided to the program represents the total number of tasks to do today. So if, for example, the program is invoked with `./hogwarts 100`, there are 100 tasks to be done today (total tasks).

*  The `main` function should initialize and clean up, respectively, any global variables.

*  Your program should create five (5) house elves (`house_elf()`) and one (1) Dobby thread (`dobby()`).

*  Worker elves take tasks using the function
	      `take_task()` (which cannot be safely run in parallel) and then do the work by
	      calling `do_work()` (which can be done in parallel). If the list of tasks is
	      empty, an elf cannot call `take_task()` and must instead wake up Dobby, who will then post the next 10 tasks before going to sleep again; meanwhile the elf is blocked until the next group of tasks is ready.

*  When woken up, Dobby posts (up to) 10 tasks to do by calling `post_tasks()` and updates the number of available tasks (active tasks). After that, Dobby sleeps (is blocked). If there are fewer than 10 tasks remaining for the day then Dobby cannot post 10, but instead posts however many remain.

*   Note that Dobby and the elves are responsible for managing the number of active tasks.

*  House elves don't know that their day is over until they are told so using `pthread_cancel`. Dobby must dismiss the other elves using this function, if Dobby gets woken up and the remaining tasks for the day is zero. After cancelling the other threads, Dobby should use `pthread_join` to wait for each of them to finish before he can go home for the day (exit).

*  Your program should not leak memory; be sure to destroy/deallocate anything initialized/allocated.

*  There should not be any race conditions in your program either.

### Sample Output
Below is some sample output. The actual order of task completion will vary.

```
jzarnett@ecetesla0:~/ece252$ ./hogwarts 5
There are 5 tasks to do today.
Adding 5 tasks to the list of active tasks.
Task 5 was completed!
Task 3 was completed!
Task 4 was completed!
Task 2 was completed!
Task 1 was completed!
jzarnett@ecetesla0:~/ece252$ ./hogwarts 0
There are 0 tasks to do today.
jzarnett@ecetesla0:~/ece252$ ./hogwarts 20
There are 20 tasks to do today.
Adding 10 tasks to the list of active tasks.
Task 10 was completed!
Task 9 was completed!
Task 8 was completed!
Task 7 was completed!
Task 6 was completed!
Task 4 was completed!
Task 5 was completed!
Task 1 was completed!
Task 3 was completed!
Adding 10 tasks to the list of active tasks.
Task 2 was completed!
Task 20 was completed!
Task 19 was completed!
Task 17 was completed!
Task 15 was completed!
Task 13 was completed!
Task 11 was completed!
Task 16 was completed!
Task 12 was completed!
Task 18 was completed!
Task 14 was completed!
jzarnett@ecetesla0:~/ece252$ ./hogwarts 12
There are 12 tasks to do today.
Adding 10 tasks to the list of active tasks.
Task 10 was completed!
Task 9 was completed!
Task 8 was completed!
Task 7 was completed!
Task 6 was completed!
Task 2 was completed!
Task 1 was completed!
Task 4 was completed!
Adding 2 tasks to the list of active tasks.
Task 5 was completed!
Task 3 was completed!
Task 11 was completed!
Task 12 was completed!
```
### General Guidance 

If you're having trouble, try checking these things, and it may resolve your problem.

* Check the documentation for how functions work if you are unfamiliar with them (google is your friend!)
* Check return values (and possibly `errno`) for network functions if things are going wrong.
* Have you initialized all variables? It is easy to forget; `malloc` does not initialize the value...
* Pay attention to compiler warnings. Warnings can sometimes be ignored, but may be a hint about a semantic error that you will want to resolve. If the compiler is trying to tell you something, listen.
* You need a buffer for receiving the data, and the receive function tells you how many bytes the server sent.
* Don't forget null terminators for strings!
* Is there a missing or extra `*` (dereference) on a pointer somewhere?
* Does every memory allocation have a matching deallocation?
* It may be helpful to put `printf()` statements to follow along what the program is doing and it may help you narrow down where the issue is.
* Check carefully what values semaphores are initialized to.
* Is the number of `wait` and `post` statements on a given semaphore balanced?
* Does every memory allocation have a matching deallocation?
* Does every code path in a critical section protected by a mutex lead to an unlock statement?
* Are accesses to shared variables protected by a mutex or semaphore appropriately?
* Remember that both reads and writes of shared variables need to be inside critical sections.


### Submitting Your Code. 
When you're done, use the command `git commit -a` to add all files to your commit and enter a commit message. Then `git push` to upload your changes to your repository. You can commit and push as many times as you like; we'll look at whatever was last pushed.
