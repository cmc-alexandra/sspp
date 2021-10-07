#define M 1000000

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>

long seg = 0;

double func(double x) {
	return 4 / (1 + x * x);
}

struct info {
	long index_beg;
	long index_end;
	double sum;
};

double part(long N, long begin, long end) {
	double sum = 0;

	for(long i = begin; i <= end; i++) {
		double x = i / (double) N;
		sum += func(x);
	}

	return sum / (double) N;
}

void *routine(void *arg) {
    struct info *parg = (struct info *)(arg);
    parg->sum = part(seg, parg->index_beg, parg->index_end);
    arg = (void *)(parg);
    return NULL;
}

double calc_pi(long seg, long threads) {
    double res = 0;
    struct info seg_borders[threads];
	long len = seg / threads;

	for(int i = 0; i < threads; i++) {
		seg_borders[i].index_beg = len * i;
		seg_borders[i].index_end = seg_borders[i].index_beg + len - 1;
	}

	seg_borders[threads - 1].index_end = seg - 1;

	pthread_t thread[threads];

    for (long i = 0; i < threads; i++) {
       	int retVal = pthread_create(&thread[i], NULL, routine, seg_borders + i);
       	assert(retVal == 0);
    }

    for (long i = 0; i < threads; i++) {
       	void *status = NULL;
       	int retVal = pthread_join(thread[i], &status);
    
   	    if (retVal) {
            fprintf(stderr, "pthread_join failed");
        }
   	}

	for(int i = 0; i < threads; i++) {
        res += seg_borders[i].sum;
    }

	return res;
}

struct timeval t1, t2, t;
struct timezone zone;

//void timer_start() { gettimeofday(&t1, &t); }

double timer_stop() {
    gettimeofday(&t2, &t);
    t.tv_sec = t2.tv_sec - t1.tv_sec;
    t.tv_usec = t2.tv_usec - t1.tv_usec;
    if (t.tv_usec < 0) {
        t.tv_sec--;
        t.tv_usec += 1000000;
    }
    return t.tv_sec + (double)t.tv_usec / 1000000;
}

int main(int argc, char**argv){
	gettimeofday(&t1, &t);

	seg = atoi(argv[1]);
	long thread = atoi(argv[2]);

	printf("Nr of segments = %s\n", argv[1]);
	printf("Nr of threads = %s\n", argv[2]);
	printf("pi = %.10lf\n", calc_pi(seg, thread));
    printf("time = %lf\n\n", timer_stop());

	return 0;
}
