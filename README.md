*This project has been created as part of the 42 curriculum by nedo-nas.*

# Codexion

<div align="center">
   
![Codexion](https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExdmxreXFyOXM4MmJ5dHlhNTd0MnM5a2lrZGJtaGpxem93MzA5dmRoZSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/L0waa8YxiEwmZ1jB54/giphy.gif)

</div>

## Description

**Codexion** is a concurrency simulation written in C with POSIX threads. A group of coders sits around a circular co-working hub. Between each pair of neighbours lies one USB dongle, so there are exactly as many dongles as coders. To compile their quantum code, a coder needs **two dongles at the same time**, one on the left and one on the right. After compiling, the coder debugs, then refactors, then tries to compile again.

The goal is to keep every coder alive. A coder **burns out** if they do not start a new compile within `time_to_burnout` milliseconds of their previous compile start (or of the beginning of the simulation). The simulation ends when either:

* a coder burns out, or
* every coder has compiled at least `number_of_compiles_required` times.

The project is a variation of the classic Dining Philosophers problem, with extra constraints:

* **Dongle cooldown**: after being released, a dongle cannot be taken again until `dongle_cooldown` milliseconds have passed.
* **Scheduling policy**: when several coders request the same dongle, it is granted according to a policy, either `fifo` (First In, First Out) or `edf` (Earliest Deadline First, where the deadline is `last_compile_start + time_to_burnout`).
* **Monitor thread**: a dedicated thread detects burnout and stops the simulation, printing the burnout message within 10 ms of the real event.
* **Serialized logging**: messages from different threads never interleave.

### Project structure

```
.
├── Makefile
├── README.md
├── srcs/
│   ├── main.c              entry point
│   ├── parsing.c           argument validation
│   ├── init.c              allocation and initialization
│   ├── simulation.c        thread creation and joining
│   ├── coder.c             coder life cycle
│   ├── monitor.c           burnout detection
│   ├── scheduler.c         request / grant logic
│   ├── scheduler_heap.c    priority queue (heap)
│   ├── codexion.h          structures and prototypes
│   └── helpers/            utilities (time, logging, parsing, coder, dongle)
└── tests/
```

## Blocking cases handled

### Deadlock prevention (Coffman's conditions)

A deadlock requires four conditions to hold at once. Here is how each one is handled:

| Condition | Status in Codexion |
|-----------|--------------------|
| Mutual exclusion | Inherent: a dongle can be held by only one coder at a time |
| Hold and wait | Inherent: a coder holds one dongle while waiting for the second |
| No preemption | Inherent: a dongle is never taken away from a coder |
| **Circular wait** | **Broken by design** |

Circular wait is eliminated with a **global resource ordering**. Every coder has a left and a right dongle, but always requests the one with the **lower id first**, and only then the other. Because every coder follows the same total order, a cycle of "A waits for B, B waits for C, ... Z waits for A" can never form. The last coder in the circle, whose left and right ids are inverted relative to the others, is the one that "breaks" the ring.

```
for each compile attempt:
    first  = dongle with the lower id (between left and right)
    second = dongle with the higher id
    request(first)  -> wait until granted -> take
    request(second) -> wait until granted -> take
    compile
    release both
```

### Starvation prevention

Starvation is handled by the scheduler attached to each dongle:

* **FIFO**: every request receives an increasing ticket number. The lowest ticket is served first, so no request can be overtaken indefinitely.
* **EDF**: the request with the earliest burnout deadline is served first. The coder closest to burning out always has priority. If two deadlines are equal, the **ticket number is the tie-breaker**, which makes the policy fully deterministic.

The priority comparison follows this flow:

```
has_priority(A, B):
    if policy is EDF and deadline(A) != deadline(B):
        return deadline(A) < deadline(B)
    return ticket(A) < ticket(B)
```

A coder is only allowed to take a dongle when its request is at the **top of that dongle's heap**, the dongle is free, and its cooldown has expired.

### Dongle cooldown

When a coder releases a dongle, the dongle stores a timestamp `available_at = release_time + dongle_cooldown`. A coder whose request is first in the queue but arrives before that timestamp does not spin. It sleeps on the dongle's condition variable with a **timed wait** whose duration is exactly the remaining cooldown, then re-evaluates the conditions when it wakes up.

### Precise burnout detection

A separate monitor thread continuously compares the current time with each coder's burnout deadline (last compile start plus `time_to_burnout`). When a deadline is exceeded:

1. The monitor marks the simulation as over.
2. It prints the burnout message.
3. It wakes every waiting coder so that all threads can exit.

The monitor checks at short intervals, so the burnout message is displayed well within the 10 ms tolerance required by the subject.

### Clean termination of blocked threads

A coder may be sleeping on a dongle's condition variable when the simulation ends (by burnout or by reaching the required number of compiles). To prevent threads from hanging forever, the end of the simulation triggers a **broadcast** on every dongle's condition variable. Each coder checks the "simulation over" flag as soon as it wakes up and exits without taking the dongle.

### Single coder edge case

With only one coder there is only one dongle. The coder takes it, can never obtain a second one, and therefore cannot compile. The monitor detects the burnout at the deadline and ends the simulation normally, with no deadlock and no hang.

### Log serialization

All state messages go through a single logging function protected by a mutex. The function checks whether the simulation is over before printing, so no regular message can appear after a burnout message, and two messages can never be mixed on the same line.

## Thread synchronization mechanisms

### Primitives used

* **`pthread_mutex_t`**: protects every piece of shared data.
* **`pthread_cond_t`**: lets coders sleep while waiting for a dongle, instead of busy-waiting.
* **`pthread_cond_timedwait`**: used for the cooldown, so that a coder wakes at the exact moment the dongle becomes available.
* **`pthread_cond_broadcast`**: used by the monitor to wake every sleeping coder when the simulation ends.

### What each mutex protects

| Mutex | Protects | Accessed by |
|-------|----------|-------------|
| Dongle mutex (one per dongle) | The dongle's busy state, cooldown timestamp and request heap | Coders |
| Coder mutex (one per coder) | The coder's burnout deadline (last compile start) and number of compiles done | The coder (writes) and the monitor (reads) |
| Table mutex | The "simulation over" flag | Everyone |
| Log mutex | The standard output | Everyone |

### How the dongle wait works

Each dongle has its own condition variable, so a coder only wakes up when something changes on **the dongle it is waiting for**. The waiting logic follows this flow:

```
wait_dongle(dongle, coder):          (dongle mutex already locked)
    if simulation is over:
        return
    if coder is not at the top of the heap, or dongle is busy:
        wait on the dongle's condition variable
    else if the dongle is free but cooldown has not expired:
        timed wait until available_at
    unlock the dongle mutex
```

The caller re-evaluates the conditions after waking up, which protects against spurious wake-ups and against being woken for a state that has already changed.

### Race conditions prevented

**1. Two coders taking the same dongle.**
The dongle's busy flag is only read and changed while its mutex is locked. The "check if free and mark as busy" operation is atomic with respect to other coders, so two coders can never believe they own the same dongle.

**2. Monitor reading data while a coder updates it.**
When a coder starts compiling, it updates its last compile start and its compile counter under its own mutex. The monitor reads these values under the same mutex, so it never sees a half-updated deadline and never reports a false burnout.

**3. Coder printing after the simulation ended.**
The log function locks the log mutex and then checks the "simulation over" flag before writing. Once the monitor sets the flag, no further coder message can be printed.

**4. Lost wake-up at the end of the simulation.**
The flag is set before the broadcast, and a coder re-reads it after every wake-up. A coder that is about to sleep sees the flag already set and does not wait.

This keeps the coders unaware of each other and of the monitor, as the subject requires.

## Instructions

### Compilation

The project is compiled with `cc` and the flags `-Wall -Wextra -Werror -pthread`.

```
make          # builds the executable "codexion"
make clean    # removes object files
make fclean   # removes object files and the executable
make re       # fclean followed by all
```

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Description |
|----------|-------------|
| `number_of_coders` | Number of coders (and of dongles) |
| `time_to_burnout` | Milliseconds a coder can go without starting a compile before burning out |
| `time_to_compile` | Milliseconds spent compiling (two dongles held) |
| `time_to_debug` | Milliseconds spent debugging |
| `time_to_refactor` | Milliseconds spent refactoring |
| `number_of_compiles_required` | Compiles each coder must complete for the simulation to end successfully |
| `dongle_cooldown` | Milliseconds a dongle stays unavailable after being released |
| `scheduler` | `fifo` or `edf` |

All arguments are mandatory. The program rejects negative numbers, non-integers and any scheduler other than `fifo` or `edf`.

**Example:**

```
./codexion 5 800 200 200 200 7 50 edf
```

**Output format:**

```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
...
1204 3 burned out
```

## Resources

### References

* [POSIX Threads overview, Linux man page `pthreads(7)`](https://man7.org/linux/man-pages/man7/pthreads.7.html)
* Linux man pages for `pthread_create`, `pthread_join`, `pthread_mutex_*`, `pthread_cond_*`, `gettimeofday` and `clock_gettime`
* [GeeksforGeeks](https://www.geeksforgeeks.org/): articles on multithreading, mutexes, condition variables and heaps in C
* YouTube tutorials on POSIX threads, mutexes and the Dining Philosophers problem

### AI Usage

Artificial intelligence was used as a **learning support tool**, rather than as a shortcut:

* **Clarifying concepts**: AI was used to explain how threads, mutexes and condition variables work, and the theory behind deadlock and starvation.
* **Debugging logic**: when dealing with edge cases, AI helped to visualize pointer arithmetic and the behaviour of threads during program execution.
* **Documentation**: AI assisted in structuring this README so that it meets the subject's requirements while remaining easy to read. The content describes the author's own implementation.
* **Code implementation**: all logic and code were written manually, ensuring a deep understanding of the underlying mechanisms, as required by the 42 pedagogical methodology.
