#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <assert.h> /* assert */
#include <limits.h> /* INT_MAX */
#include <sys/time.h> /* gettimeofday */
#include <sys/types.h>

/* misc. */
#define ARRAY_SIZE(a) ((sizeof (a)) / (sizeof (a)[0] ))
#define STATIC_ASSERT(cond) do { switch (1) {case 0: case !!(cond): ;} } while (0)

/* generic element type, use it in *all* collections */
typedef int elem_t;

/* the following type denotes the prototype of all sorting functions */
typedef void (*sort_t)(elem_t *arr, size_t length);

/* dummy time eater */
static void eat(void)
{
	static volatile int eater; /*noopt*/
	/* int eater; /\* try this with -O3 *\/ */
	for (eater = 0; eater < INT_MAX/10; eater++)
		;
}

/* algorithms, to be defined in tests */
void merge_sort(elem_t *arr, size_t length)
{
	eat();
	eat();
}
void quick_sort(elem_t *arr, size_t length)
{
	eat();
}

/**********************************************************************
 *                      TEST SYSTEM STARTS HERE...                    *
 **********************************************************************/

#define AVAIL_TEST_NR_MAX (10)

struct test_detail {
	sort_t		 td_func;
	const char	*td_name;
};

struct test {
	struct test_detail	st_test[AVAIL_TEST_NR_MAX];
	size_t			st_start; /* start position of the test */
	size_t			st_end;   /* end position of the test */
};

#define AVAIL_PROFILE_NR_MAX (5)

struct run_profile_detail {
	size_t	pd_length;
	size_t	pd_run_time;
};

struct run_profile {
	struct run_profile_detail	rp_profile[AVAIL_PROFILE_NR_MAX];
	size_t				rp_start; /* start position of the test */
	size_t				rp_end;   /* end position of the test */
};

/** 
 * allocates an array of given @length with randomly set elements
 */
int init_array(elem_t **out, size_t length)
{
	*out = (elem_t*)0xdeadbeaf;
	return 0;
}

/* ... */
void fini_array(elem_t *out)
{
}

/**
 *  @return time in **microseconds**
 */
size_t checkpoint(void)
{
	struct timeval  tv;
	struct timezone tz;
	int		rc;
 
	rc = gettimeofday(&tv, &tz);
	assert(rc == 0); /* XXX: provide more correct solution */
	return 1000000*tv.tv_sec + tv.tv_usec;
}

/** Reports time and algorithm profile params
 */
void report(size_t elapsed, struct run_profile_detail prf,
	    struct test_detail tst)
{
	printf("test: %s, arr_len: %lu, test_times: %lu // elapsed time: %lu\n",
	       tst.td_name, prf.pd_length, prf.pd_run_time, elapsed);
}

/**
 * Tests algorithms, reports execution time
 */
void test(size_t profile_start, size_t profile_end,
	  size_t test_start, size_t test_end)
{
	const struct run_profile prof = {
		.rp_profile = {
			[0] = {1, 4}, /* {10, 10000} */
			[1] = {2, 3}, /* {100, 1000} */
			[2] = {3, 2}, /* {1000, 100} */
			[3] = {4, 1}, /* {10000, 10} */
			[4] = {0, 0} /* end */
		},
		.rp_start = profile_start,
		.rp_end   = profile_end
	};
	const struct test rtst = {
		.st_test = {
			[0] = {merge_sort, "merge_sort"},
			[1] = {quick_sort, "quick_sort"},
			[2] = {NULL, NULL} /* end */
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

/* ... */
int main(int argc, char **argv)
{
	printf("First one to init internals of libc\n");
	checkpoint();

	test(0, 4, 0, 2);
	return 0;
}
