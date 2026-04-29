*This project has been created as part of the 42 curriculum by pedde-al.*

---

## Description

**Codexion** is a concurrency simulation in C where multiple coders compete for a limited number of shared USB dongles to compile their quantum code. Each coder runs as an independent POSIX thread and must acquire two dongles simultaneously to compile. If a coder goes too long without compiling, they burn out and the simulation stops.

The challenge combines classic concurrency problems — deadlock prevention, starvation avoidance, and precise timing — with a configurable scheduling policy (FIFO or EDF) that governs fair access to shared resources.

---

## Instructions

### Compilation

```
make
```

Produces the executable `codexion`. Use `make clean` to remove object files, `make fclean` to also remove the binary, and `make re` to rebuild from scratch.

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. Times are in milliseconds. `scheduler` must be exactly `fifo` or `edf`.

### Example

```
./codexion 4 800 200 200 200 5 50 fifo
```

Four coders each need to compile 5 times. They have 800 ms between compiles before burning out, compiling takes 200 ms, and each dongle has a 50 ms cooldown after release. Requests are served in arrival order.

### Expected log format

```
0 1 has taken a dongle
2 1 has taken a dongle
2 1 is compiling
202 1 is debugging
402 1 is refactoring
```

Each line: `timestamp_in_ms coder_id message`.

---

## Blocking cases handled

### Deadlock prevention
A classic deadlock scenario occurs when every coder picks up their left dongle and waits forever for the right one. This is prevented by **resource ordering**: even-numbered coders acquire left then right, odd-numbered coders acquire right then left. This breaks the circular wait condition (one of Coffman's four necessary conditions), making deadlock impossible.

### Starvation prevention
Under both schedulers, each dongle maintains a wait queue of up to two requests. Under FIFO, requests are served strictly in arrival order, guaranteeing progress for every coder. Under EDF, the coder with the earliest burnout deadline is served first; since a waiting coder's deadline only approaches over time, it will eventually become the most urgent and be granted access before burning out (provided parameters are feasible).

### Cooldown handling
After a coder releases a dongle, the dongle is held unavailable for `dongle_cooldown` milliseconds. The releasing thread busy-waits on `get_time()` before re-locking the mutex and broadcasting availability. This ensures no other thread can acquire the dongle during cooldown, even if it is first in the queue.

### Precise burnout detection
A dedicated monitor thread continuously polls `get_time() - coder->last_compile` for every coder. When the elapsed time exceeds `time_to_burnout`, the burnout message is logged immediately (within the required 10 ms window), `simulation_running` is set to 0, and `pthread_cond_broadcast` is sent to every dongle condition variable to wake any threads blocked in `wait_for_dongle` so they can exit cleanly.

### Log serialization
All calls to `log_state` are protected by `sim->mutex_log`. The mutex is held for the entire `printf` call, preventing interleaved output from concurrent threads.

---

## Thread synchronization mechanisms

### `pthread_mutex_t` — dongle and log protection

Each `t_dongle` has its own `mutex_dongle` that serializes access to the dongle's `available` flag, `timestamp`, and wait queue. The global `sim->mutex_log` serializes all output.

```c
pthread_mutex_lock(&dongle->mutex_dongle);
// inspect/modify dongle state
pthread_mutex_unlock(&dongle->mutex_dongle);
```

### `pthread_cond_t` — blocking wait for dongle availability

Coders that cannot immediately acquire a dongle block on `cond_dongle` inside `wait_for_dongle`. The condition is re-checked after every wakeup (spurious-wakeup safe) using a `while` loop:

```c
while ((!dongle->available || dongle->queue[0].coder_id != coder->id)
    && coder->sim->simulation_running)
    pthread_cond_wait(&dongle->cond_dongle, &dongle->mutex_dongle);
```

A coder only proceeds when **both** the dongle is available AND it is at the front of the queue, preventing unfair bypassing.

### Priority queue for FIFO/EDF

Each dongle maintains a two-slot `t_request` queue. On enqueue, if two requests exist and they are out of order (by arrival time for FIFO, by deadline for EDF), the two entries are swapped. On dequeue, slot 0 is overwritten by slot 1 and `queue_size` is decremented. Since at most two coders share any given dongle, a two-slot array is sufficient and no heap rebalancing is needed.

### Monitor thread — burnout and completion detection

A separate thread runs `monitor_routine`, which loops over all coders checking burnout and compile counts. On either stop condition it sets `simulation_running = 0` and broadcasts to all dongle condition variables, unblocking every waiting coder thread so the simulation terminates promptly without any thread hanging indefinitely.

### Deadlock-free acquisition order

```c
// even coder: left first
if (coder->id % 2 == 0)
    take_left_first(coder);   // acquires left, then right
else
    take_right_first(coder);  // acquires right, then left
```

This global ordering ensures no circular wait can form across the ring of coders.

---

## Resources

- **POSIX Threads Programming** — Blaise Barney, LLNL:
  https://hpc-tutorials.llnl.gov/posix/
- **The Little Book of Semaphores** — Allen B. Downey (deadlock, dining philosophers analogues)
- **`pthread_cond_wait` man page** — POSIX specification for condition variables
- **`gettimeofday` man page** — Used for millisecond-precision timestamps
- **Earliest Deadline First scheduling** — Wikipedia / real-time systems literature

### AI usage

AI was used during this project for the following tasks:

- Understanding concepts and helping create the structure of the project
- Completing and correcting the `Makefile` (adding missing source files, object file rules, and `clean`/`fclean` targets).
- Fixing norm errors.
