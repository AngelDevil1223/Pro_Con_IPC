#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>

#define MY_WRITE_SEM "/buffer_produce_consumer_write"
#define MY_READ_SEM "/buffer_produce_consumer_read"

void consumer(const char TGT_FILE[], int BUFFER_SIZE, int CHUNK_SIZE);
void producer(const char SRC_FILE[], int BUFFER_SIZE, int CHUNK_SIZE);

int main(int argc, char *argv[]) {
    if (argc != 5)
        return 1;

    int child_pid;
    const char *SRC_FILE = argv[1], *TGT_FILE = argv[2];
    const int CHUNK_SIZE = strtol(argv[3], (char **)NULL, 10) + 1, // +1 is added to make space for NULL char for string ending
          BUFFER_SIZE = strtol(argv[4], (char **)NULL, 10);

    // printf("%s\n%s\n%d\n%d\n", SRC_FILE, TGT_FILE, CHUNK_SIZE, BUFFER_SIZE);

    /*child_pid = fork();*/

    /*if ((child_pid = fork()) < 0) {*/
    /*printf("Error Occured while forking");*/
    /*return 2;*/
    /*} */
    child_pid = fork();
    if (child_pid == 0) {
        // Child Process
        consumer(TGT_FILE, BUFFER_SIZE, CHUNK_SIZE);
        exit(0);
    } else {
        // Parent Process
        producer(SRC_FILE, BUFFER_SIZE, CHUNK_SIZE);
        wait(NULL);
    }

    // printf("pid: %d********************************************\n\n", child_pid);

    return 0;
}

void consumer(const char TGT_FILE[], int BUFFER_SIZE, int CHUNK_SIZE) {
    FILE *fptr;
    const char *name = "OS"; /* name of the shared memory object */
    int shm_fd;              /* shared memory file descriptor */
    void *ptr, *temp_ptr;    /* pointer to shared memory object */
    char *message;
    size_t len = CHUNK_SIZE - 1, bytes_wrote = 0;
    sem_t *my_write_sem, *my_read_sem;

    fptr = fopen(TGT_FILE, "w");

    shm_fd = shm_open(name, O_RDONLY, 0666); /* open the shared memory object */

    /* memory map the shared memory object */
    ptr = temp_ptr = mmap(0, BUFFER_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    // sleep(1); // sleeping for 1 sec in order to allow producer to take a lead
    // over consumer

    my_write_sem = sem_open(MY_WRITE_SEM, O_CREAT, S_IRWXU, 1);
    if (my_write_sem == NULL) {
        perror("In sem_open() opening write semaphore.");
        exit(1);
    }

    my_read_sem = sem_open(MY_READ_SEM, O_CREAT, S_IRWXU, 0);
    if (my_read_sem == NULL) {
        perror("In sem_open() opening read semaphore.");
        exit(1);
    }


    /* read from the shared memory object */
    while (len == CHUNK_SIZE - 1) {
        sem_wait(my_read_sem);
        message = (char *)temp_ptr;
        len = strlen(message);
        /*fwrite(message, 1, len, fptr);*/
        fprintf(fptr, "%s", message);

        printf("CHILD: IN = %lu\n", len);
        printf("CHILD: ITEM = %s\n", message);

        bytes_wrote += len;

        temp_ptr = ptr + ((((bytes_wrote % BUFFER_SIZE) + CHUNK_SIZE) < BUFFER_SIZE)
                ? (bytes_wrote % BUFFER_SIZE)
                : 0);
        if (temp_ptr == ptr)
            sem_post(my_write_sem);
    }

    printf("Consumer Done Consuming\n");

    /* remove the shared memory object */
    shm_unlink(name);
    fclose(fptr);
}

void producer(const char SRC_FILE[], int BUFFER_SIZE, int CHUNK_SIZE) {
    FILE *fptr = fopen(SRC_FILE, "rt");
    const char *name = "OS"; /* name of the shared memory object */
    int shm_fd;              /* shared memory file descriptor */
    void *ptr, *temp_ptr;    /* pointer to shared memory object */
    char message[CHUNK_SIZE];
    size_t len = CHUNK_SIZE - 1, bytes_wrote = 0;
    sem_t *my_write_sem, *my_read_sem;

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    /* configure the size of the shared memory object */
    ftruncate(shm_fd, BUFFER_SIZE);

    /* memory map the shared memory object */
    ptr = temp_ptr = mmap(0, BUFFER_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    my_write_sem = sem_open(MY_WRITE_SEM, O_CREAT, S_IRWXU, 1);
    if (my_write_sem == NULL) {
        perror("In sem_open() opening write semaphore.");
        exit(1);
    }

    my_read_sem = sem_open(MY_READ_SEM, O_CREAT, S_IRWXU, 0);
    if (my_read_sem == NULL) {
        perror("In sem_open() opening read semaphore.");
        exit(1);
    }

    /* write to the shared memory object */
    while (len == CHUNK_SIZE - 1) {
        sem_wait(my_write_sem);
        len = fread(message, 1, CHUNK_SIZE - 1, fptr);
        message[len] = '\0';
        len = sprintf(temp_ptr, "%s", message);

        printf("PARENT: IN = %lu\n", len);
        printf("PARENT: ITEM = %s\n", message);
        bytes_wrote += strlen(message);
        temp_ptr = ptr + ((((bytes_wrote % BUFFER_SIZE) + CHUNK_SIZE) < BUFFER_SIZE)
                ? (bytes_wrote % BUFFER_SIZE)
                : 0);
        if (temp_ptr == ptr)
            sem_post(my_read_sem);
    }
    printf("Producer Done Producing\n");
    fclose(fptr);
}