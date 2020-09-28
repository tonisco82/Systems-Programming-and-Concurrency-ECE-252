# ECE 252 Assignment 1: Run Smarter, Barry
### Intended to be completed after lecture: 5


Your name is Barry Allen and you are the fastest person alive. To the outside world, you are an ordinary forensic scientist, but secretly with the help of your friends in S.T.A.R. Labs, you fight crime and find other meta-humans like me. You hunted down the man who killed your mother, but in doing so, you opened up our world to new threats, and you the only one fast enough to stop them. You are The Flash. 

As the Flash, you sometimes need to travel rapidly between cities to deliver messages that are too secret to trust to any other communication method. While you are the fastest person alive, you sometimes need to run smarter, not harder: you should run the most efficient route, which is defined as the shortest distance.

This is a version of the Travelling Salesperson Problem. The Travelling Salesperson Problem is defined by Wikipedia as: "Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city and returns to the origin city?" It's one of those problems that mathematicians call NP-complete, which basically means it is hard. And you might have to use brute force. The provided starter code does, in fact, use brute force.

### Primary Objective
The primary objective of this exercise is to practice using `fork()` in C as a mechanism for creating multiple processes and having these processes work together to solve a problem.

You will also need the processes to communicate with one another. You can choose how you would like to do this. A simple approach that does not require anything introduced after the `fork()` topic in the lecture material is for one process to write to a file and then another process reads from that same file. You can also use other techniques, including those covered later in the course (particularly, in the Inter-Process Communication topics).

### Description of Behaviour

The goal is to implement the program so that the following behaviour occurs:


The output of the program is the route the person should take as well as the minimum cost. The output format should not be changed. A sample run out of the output (if the Flash were running in Canada) might look like:

```
Route: Toronto - Kingston - Ottawa - Montreal - Quebec - Toronto
Distance: 2020
```


Modify the starter code to be a multiple-process version of this program. The parent process should generate each possibility (route) to check, and the child process should evaluate the route and return the result. If the parent process sees this route is better than the current best, it should update the best route it has. 

The parent process should not evaluate the distance of any routes; that should be done in the child process. 

You can do this sequentially, where the parent process creates the child and immediately waits for it to be done. You can, optionally, decide to parallelize the problem more, where multiple child processes run at the same time. This will be more difficult to coordinate, but would speed up the program significantly.

### General Guidance 

If you're having trouble, try checking these things, and it may resolve your problem.

* Check the documentation for how functions work if you are unfamiliar with them (google is your friend!)
* Check return values (and possibly `errno`) for functions if things are going wrong.
* Have you initialized all variables? It is easy to forget; `malloc` does not initialize the value...
* Pay attention to compiler warnings. Warnings can sometimes be ignored, but may be a hint about a semantic error that you will want to resolve. If the compiler is trying to tell you something, listen.
* You can make the parent process wait for a child process with `wait()`
* Don't forget null terminators for strings!
* Is there a missing or extra `*` (dereference) on a pointer somewhere?
* Does every memory allocation have a matching deallocation?
* It may be helpful to put `printf()` statements to follow along what the program is doing and it may help you narrow down where the issue is.

### Submitting Your Code. 
When you're done, use the command `git commit -a` to add all files to your commit and enter a commit message. Then `git push` to upload your changes to your repository. You can commit and push as many times as you like; we'll look at whatever was last pushed.
