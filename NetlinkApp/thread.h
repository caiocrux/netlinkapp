#ifndef THREAD_H
#define THREAD_H

#define MSG_PAYLOAD 256
#define MAX_PAYLOAD 256

typedef struct thread_arg_ {
    int sock_fd;
} thread_arg_t;

void start_kernel_data_receiver_thread(thread_arg_t *arg);

#endif  // THREAD_H
