#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 10

typedef struct product
{
	char type;  // 0=fried chicken, 1=French fries
	int amount; // pieces or weight
	char unit;  // 0=piece, 1=gram
} item;

item buffer[BUFFER_SIZE];
int first = 0;
int last = 0;
item *chicken, wing;

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
	while ((first + 1) % BUFFER_SIZE == last)
	{
		printf("No free buffer item!\n");
		return;
	}
	memcpy(&buffer[first], i, sizeof(item));
	i->amount += 1;
	first = (first + 1) % BUFFER_SIZE;
	printf("First = %d\n", first);
}

item *consume()
{
	item *i = malloc(sizeof(item));
	while (first == last)
	{
		printf("Nothing to consume!\n");
	}
	memcpy(i, &buffer[last], sizeof(item));
	i->amount -= 1;
	last = (last + 1) % BUFFER_SIZE;
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
	consume();
	consume();
	pthread_exit(NULL);
}

int main() {
	chicken = &wing;
	pthread_t tid[2];
	initChicken(chicken);
	printf("This is the main thread speaking\n");
	printf("Creating producerThread ...\n");
	pthread_create(&tid[0], NULL, producerThread, NULL);
    printf("Creating consumerThread ...\n");
	pthread_create(&tid[1], NULL, consumerThread, NULL);
	pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
	pthread_exit(NULL);
}