#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include <stdbool.h>

typedef struct cache_buffer {
	unsigned long blkno;
	struct cache_buffer *next, *prev;
	struct cache_buffer *hash_next, *hash_prev;
} cache_buffer;

cache_buffer* cache[8192];
cache_buffer** first; // first of cache
cache_buffer** last; // last of cache
int size = 0;

int search(unsigned long num) { //search cache

	if (*first == NULL && *last == NULL) {
		return 0;
	}
	cache_buffer* loc;
	bool front = true;
	for (loc = (*first); loc != (*last); loc = (loc->next)) {
		if ((loc->blkno) == num) { // exist
			if (front) { //first data -> last data
				cache_buffer* before = *last;
				(*last)->next = loc;
				*last = (*last)->next;
				(*last)->prev = before;
				(*first) = loc->next;
			}
			else { //middle data -> last data
				loc->prev->next = loc->next;
				loc->next->prev = loc->prev;

				cache_buffer* before = *last;
				(*last)->next = loc;
				*last = (*last)->next;
				(*last)->prev = before;
			}
			
			return 1;
		}
		front = false;
	}

	if ((loc->blkno) == num) { // exist (last)
		return 1;
	}

	return 0; // not exist
}

cache_buffer* hash_add(unsigned long num) { // add value in hash table
	cache_buffer* temp = (cache_buffer*)malloc(sizeof(cache_buffer));
	temp->blkno = num;
	temp->next = NULL;

	int index = num % 8192;
	if ((cache[index]->blkno) == -1) { // if empty hash table
		cache[index] = temp;
	}
	else { // not empty hash table
		temp->hash_next = cache[index];
		cache[index] = temp;
	}
	return temp;
}

int is_full() { // checking cache is full
	if (size >= 8192) {
		return 1;
	}
	else {
		return 0;
	}
}

void first_remove() { // remove first
	*first = (*first)->next;
	size--;
}

void set_last(cache_buffer* new_node) { // add last
	if (*first == NULL && *last == NULL) { // if first and last are null
		*first = new_node;
		*last = new_node;
	}
	else { // add last
		cache_buffer* before = *last;
		(*last)->next = new_node;
		*last = (*last)->next;
		(*last)->prev = before;
	}
	size++;
}

int main(void) {
	FILE* file;

	long all = 0;
	long success = 0;
	long failed = 0;

	char line[20];
	unsigned long num;
	cache_buffer* new_node;

	for (int i = 0; i < 8192; i++) {
		cache[i] = (cache_buffer*)malloc(sizeof(cache_buffer));
		(cache[i]->blkno) = -1;
		(cache[i]->next) = NULL;
		(cache[i]->prev) = NULL;
		(cache[i]->hash_next) = NULL;
		(cache[i]->hash_prev) = NULL;
	}
	first = (cache_buffer**)malloc(sizeof(cache_buffer*));
	last = (cache_buffer**)malloc(sizeof(cache_buffer*));

	*first = NULL;
	*last = NULL;


	if ((file = fopen("test_trace.txt", "r")) == NULL) { // file open
		perror("File Open Failed");
		exit(-1);
	}
	else {
		while (fgets(line, sizeof(line), file) != NULL) {
			num = atol(line);
			if (search(num) == 1) {
				all++;
				success++;
			}
			else {
				all++;
				failed++;
				new_node = hash_add(num);
				if (is_full() == 1) {
					first_remove();
				}
				set_last(new_node);
			}

		}
	}

	printf("hit ratio = %.6f, miss ratio = %.6f\n", success / (float)all, failed / (float)all);
	printf("total access = %ld, hit = %ld, miss = %ld\n", all, success, failed);
	printf("Hit ratio = %.6f\n", success / (float)all);

	for (int i = 0; i < 8192; i++) {
		free(cache[i]);
	}

	return 0;
}
