#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

typedef struct product
{
	char type;  // 0=fried chicken, 1=French fries
	int amount; // pieces or weight
	char unit;  // 0=piece, 1=gram
} item;

// Create Buffer
item buffer[BUFFER_SIZE];

// Initialize first and last
int first = 0;
int last = 0;

// Initialize item
item *chicken, wing;

// Semaphore and Mutex lock
sem_t empty, full;
pthread_mutex_t mutex;

// Threads
pthread_t tid[2]; // Thread ID
pthread_attr_t attr; // Thread attributes

void init()
{
	pthread_mutex_init(&mutex, NULL); // Initialize mutex lock
	pthread_attr_init(&attr); // Initialize pthread attributes to default
	sem_init(&full, 0, 0); // Initialize full semaphore
	sem_init(&empty, 0, BUFFER_SIZE); // Initialize empty semaphore
}

void initChicken(item *smt) {
	smt->type = 0;
	smt->amount = 0;
	smt->unit = 0;
}

void initFries(item *smt) {
    smt->type = 1;
    smt->amount = 0;
    smt->unit = 1;
}

void produce(item *i)
{
	sem_wait(&empty); // Lock empty semaphore if not zero
	pthread_mutex_lock(&mutex);
	while ((first + 1) % BUFFER_SIZE == last)
	{
		printf("No free buffer item!\n");
		return;
	}
	memcpy(&buffer[first], i, sizeof(item));
	i->amount += 1;
	printf("Chicken wings: %d piece\n", i->amount);
	first = (first + 1) % BUFFER_SIZE;
	pthread_mutex_unlock(&mutex);
	sem_post(&full); // Increments semaphore for # of full
	printf("First = %d\n", first);
}

item *consume(item *p)
{
	sem_wait(&full); // Lock full semaphore if not zero
	pthread_mutex_lock(&mutex);
	item *i = malloc(sizeof(item));
	while (first == last)
	{
		printf("Nothing to consume!\n");
	}
	memcpy(i, &buffer[last], sizeof(item));
	p->amount -= 1;
	printf("Chicken wings: %d piece\n", p->amount);
	last = (last + 1) % BUFFER_SIZE;
	pthread_mutex_unlock(&mutex);
	sem_post(&empty); // Increments semaphore for # of empty
	printf("Last = %d\n", last);
	return i;
}

void *producerThread(void *param) {
	/* code */
	printf("3 Chicken Wings coming right up!\n");
	produce(chicken);
	produce(chicken);
	produce(chicken);
	pthread_exit(NULL);
}

void *consumerThread(void *param) {
	/* code */
	printf("Consuming 2 Chicken Wings!\n");
	consume(chicken);
	consume(chicken);
	pthread_exit(NULL);
}

int main() {
	chicken = &wing;
	initChicken(chicken);
	init();
	printf("This is the main thread speaking\n");
	printf("Creating producerThread ...\n");
	pthread_create(&tid[0], NULL, producerThread, NULL);
    printf("Creating consumerThread ...\n");
	pthread_create(&tid[1], NULL, consumerThread, NULL);
	pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
	pthread_exit(NULL);
}