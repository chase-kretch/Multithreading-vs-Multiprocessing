# Multithreading-vs-Multiprocessing
A comparison between MT and MP when doing matrix multiplication
### Scripts
- dual_threading: 2 thread/cores
- multithreading: uses as many threads/cores as the host pc
- sharedmemory: uses mmap to share memory between child and parent processes
- piping: uses piping as inter-process communication to share data between child and parent processes
