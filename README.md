# Cldlib

* Semaphore
  * STL of C++11 have no semaphore.
  * Use condition variable implement semaphore.

* I need a queue with limitations of maximum and minimum.
  * If the queue size => max, the thread who want to push() will hang up for waiting.
  * If the queue size <= min, the thread who want to pop() will hang up for wating.

* Also there is a class of 'Wakable Sleep'
