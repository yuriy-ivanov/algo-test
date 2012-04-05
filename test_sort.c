#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <assert.h> /* assert */
#include <sys/time.h> /* gettimeofday */
#include <sys/types.h>
#include "algo_common.h"
#include "quick_sort.h"
#include "merge_sort.h"
#include "bubble_sort.h"

/* include other sorts here */

#define AVAIL_TEST_NR_MAX (10)

struct test_detail {
	sort_t		 td_func;
	const char	*td_name;
};

struct test {
	struct test_detail	st_test[AVAIL_TEST_NR_MAX];
	/* start position of the test */
	size_t			st_start;
	/* end position of the test */
	size_t			st_end;
};

#define AVAIL_PROFILE_NR_MAX (10)

struct run_profile_detail {
	size_t	pd_length;
	size_t	pd_run_time;
};

struct run_profile {
	struct run_profile_detail	rp_profile[AVAIL_PROFILE_NR_MAX];
	/* start position of the test */
	size_t				rp_start;
	/* end position of the test */
	size_t				rp_end;
};

/** 
 * @TODO: define this function...
 * allocates an array of given @length with randomly set elements
 */
int init_array(elem_t **out, size_t length)
{
	*out = (elem_t*)malloc(sizeof(elem_t)*length);
	return 0;
}

/**
 * @TODO: define this function...
 */
void fini_array(elem_t *out)
{
	free(out);
}

/**
 *  @return time in **microseconds**
 */
size_t checkpoint(void)
{
	struct timeval  tv;
	int		rc;
 
	rc = gettimeofday(&tv, NULL);
	assert(rc == 0); /* XXX: provide more correct solution */
	return 1000000*tv.tv_sec + tv.tv_usec;
}

/** 
 * @TODO: define this function...
 * Reports time and algorithm profile params
 */
void report(size_t elapsed, struct run_profile_detail prf,
	    struct test_detail tst)
{
	const size_t tm = prf.pd_run_time;
	/* printf("test: %s, arr_len: %lu, test_times: %lu // elapsed time: %lu || avg time: %5.5f\n", */
	/* 	 tst.td_name, prf.pd_length, tm, elapsed, ((float)elapsed)/tm); */
	printf("%s:\t%lu\t%5.5f\n", tst.td_name, prf.pd_length, ((float)elapsed)/tm);
}

/**
 * Tests algorithms, reports execution time
 */
void test(size_t profile_start, size_t profile_end,
	  size_t test_start, size_t test_end)
{
	const struct run_profile prof = {
		.rp_profile = {
			[0] = {10, 10000},
			[1] = {20, 10000},
			[2] = {50, 10000},
			[3] = {100, 1000},
			[4] = {200, 1000},
			[5] = {500, 1000},
			[6] = {1000, 100},
			[7] = {5000, 100},
			[8] = {10000, 10},
			[9] = {0, 0}  /* end */
		},
		.rp_start = profile_start,
		.rp_end   = profile_end
	};
	const struct test rtst = {
		.st_test = {
			[0] = {merge_sort, "merge_sort"},
			[1] = {quick_sort, "quick_sort"},
			[2] = {bubble_sort, "bubble_sort"},
			[3] = {NULL, NULL} /* end */
		},
		.st_start = test_start,
		.st_end   = test_end
	};
	size_t tst, prf, len, tim;
	size_t start_time, end_time;
	elem_t *array;
	sort_t func;
	int rc;

	assert(prof.rp_start >= 0);
	assert(rtst.st_start >= 0);
	
	for (tst = rtst.st_start; tst < rtst.st_end; ++tst) {
		for (prf = prof.rp_start; prf < prof.rp_end; ++prf) {
			len = prof.rp_profile[prf].pd_length;
			assert(len != 0);
			func = rtst.st_test[tst].td_func;
			assert(func != NULL);

			rc = init_array(&array, len);
			assert(rc == 0);

			start_time = checkpoint();

			for (tim = 0; tim < prof.rp_profile[prf].pd_run_time; ++tim) {
				func(array, len);
			}

			end_time = checkpoint();
			fini_array(array);
			report(end_time - start_time,
			       prof.rp_profile[prf],
			       rtst.st_test[tst]);
		}
	}
}

int main(int argc, char **argv)
{
	printf("First one to init internals of libc\n");
	checkpoint();

	test(0, 9, 0, 3);
	return 0;
}
