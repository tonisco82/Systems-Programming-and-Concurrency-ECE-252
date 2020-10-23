# ECE 252 Assignment 3: It is only logical, Captain

### Intended to be completed after lecture: 13

On the USS _Enterprise_, when not otherwise busy, Mr. Spock enjoys three-dimensional chess, finding two-dimensional chess to be too easy. He is, after all, a Vulcan. An alien species that they have encountered has recently challenged him to solve the **N-Queens** problem. 

This problem is a generalization of the [Eight Queens Puzzle](https://en.wikipedia.org/wiki/Eight_queens_puzzle) about placement of queens on a chessboard so that they cannot attack one another. A chessboard of NxN has N queens and the goal of the program is to calculate how many valid solutions there are. The wikipedia page explans it pretty well. Yes, Wikipedia still exists in the 23rd century.


### Primary Objective
The primary objective of this exercise is to practice using threads in a simple C program.

### Description of Behaviour
The starter code as provided does a sequential version of solving the problem. It takes as its argument the size of the board to use. Choosing a very large board will take a very long time, so choose something more reasonable (approximately 13).

The goal is to rewrite the provided program so that the the calculation of the number of valid solutions is completed in parallel by multiple threads (for simplicity, make `n` threads for a chessboard of size NxN). The externally visible behaviour of the program should not change, and you should not have any race conditions.

To divide the work up amongst multiple threads, you need a strategy for how to share the work. One suggestion (that you do not have to follow if you prefer another strategy) is to create each of the threads with a different initial condition. 

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
