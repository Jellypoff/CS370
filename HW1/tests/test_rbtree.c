#include "rbtree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static int free_count = 0;

static void count_free(void *value)
{
	free_count++;
	free(value);
}

static int *make_int(int n)
{
	int *p = malloc(sizeof *p);
	assert(p != NULL);
	*p = n;
	return p;
}

static void test_create_destroy(void)
{
	rbtree_t *t = rb_create(NULL);
	assert(t != NULL);
	assert(rb_size(t) == 0);
	rb_destroy(t);
	rb_destroy(NULL);
	printf("test_create_destroy passed\n");
}

static void test_insert_find_basic(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);
	free_count = 0;

	assert(rb_insert(t, "alpha", make_int(1)) == 0);
	assert(rb_insert(t, "beta", make_int(2)) == 0);
	assert(rb_insert(t, "gamma", make_int(3)) == 0);
	assert(rb_insert(t, "delta", make_int(4)) == 0);

	assert(rb_size(t) == 4);
	assert(*(int *)rb_find(t, "alpha") == 1);
	assert(*(int *)rb_find(t, "beta") == 2);
	assert(*(int *)rb_find(t, "gamma") == 3);
	assert(*(int *)rb_find(t, "delta") == 4);

	rb_destroy(t);
	assert(free_count == 4);
	printf("test_insert_find_basic passed\n");
}

static void test_find_missing(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	assert(rb_insert(t, "one", make_int(1)) == 0);
	assert(rb_find(t, "two") == NULL);
	assert(rb_find(t, "") == NULL);

	rb_destroy(t);
	printf("test_find_missing passed\n");
}

static void test_overwrite(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);
	free_count = 0;

	assert(rb_insert(t, "key", make_int(10)) == 0);
	assert(rb_size(t) == 1);

	assert(rb_insert(t, "key", make_int(20)) == 0);
	assert(rb_size(t) == 1);
	assert(free_count == 1);
	assert(*(int *)rb_find(t, "key") == 20);

	rb_destroy(t);
	assert(free_count == 2);
	printf("test_overwrite passed\n");
}

static void test_size_tracking(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	const char *keys[] = { "m", "d", "z", "a", "f", "y" };
	size_t n = sizeof keys / sizeof keys[0];

	/* invariant: after i inserts of distinct keys, rb_size(t) == i */
	for (size_t i = 0; i < n; i++) {
		assert(rb_insert(t, keys[i], make_int((int)i)) == 0);
		assert(rb_size(t) == i + 1);
	}

	assert(rb_insert(t, keys[0], make_int(100)) == 0);
	assert(rb_size(t) == n);

	rb_destroy(t);
	printf("test_size_tracking passed\n");
}

int main(void)
{
	test_create_destroy();
	test_insert_find_basic();
	test_find_missing();
	test_overwrite();
	test_size_tracking();
	printf("All tests passed\n");
	return 0;
}
