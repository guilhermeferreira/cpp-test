# PThreads
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(Threads_LIBRARIES  Threads::Threads)

find_package(Threads REQUIRED)
