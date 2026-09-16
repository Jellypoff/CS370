#include "rbtree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Each of the four tests below inserts three keys in an order that forces
 * rb_insert_fixup to rotate the true root. If a rotation forgets to update
 * t->root (the "stale root" bug), the promoted node becomes unreachable from
 * t->root and rb_find silently returns NULL for keys that are still present,
 * even though rb_size still reports the correct count. */

static void assert_three_keys_findable(rbtree_t *t, const char *k0, int v0,
					const char *k1, int v1,
					const char *k2, int v2)
{
	assert(rb_size(t) == 3);
	assert(*(int *)rb_find(t, k0) == v0);
	assert(*(int *)rb_find(t, k1) == v1);
	assert(*(int *)rb_find(t, k2) == v2);
}

static void test_stale_root_left_rotation(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	assert(rb_insert(t, "a", make_int(1)) == 0);
	assert(rb_insert(t, "b", make_int(2)) == 0);
	assert(rb_insert(t, "c", make_int(3)) == 0);

	assert_three_keys_findable(t, "a", 1, "b", 2, "c", 3);

	rb_destroy(t);
	printf("test_stale_root_left_rotation passed\n");
}

static void test_stale_root_right_rotation(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	assert(rb_insert(t, "c", make_int(3)) == 0);
	assert(rb_insert(t, "b", make_int(2)) == 0);
	assert(rb_insert(t, "a", make_int(1)) == 0);

	assert_three_keys_findable(t, "a", 1, "b", 2, "c", 3);

	rb_destroy(t);
	printf("test_stale_root_right_rotation passed\n");
}

static void test_stale_root_left_right_zigzag(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	assert(rb_insert(t, "c", make_int(3)) == 0);
	assert(rb_insert(t, "a", make_int(1)) == 0);
	assert(rb_insert(t, "b", make_int(2)) == 0);

	assert_three_keys_findable(t, "a", 1, "b", 2, "c", 3);

	rb_destroy(t);
	printf("test_stale_root_left_right_zigzag passed\n");
}

static void test_stale_root_right_left_zigzag(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	assert(rb_insert(t, "a", make_int(1)) == 0);
	assert(rb_insert(t, "c", make_int(3)) == 0);
	assert(rb_insert(t, "b", make_int(2)) == 0);

	assert_three_keys_findable(t, "a", 1, "b", 2, "c", 3);

	rb_destroy(t);
	printf("test_stale_root_right_left_zigzag passed\n");
}

static void test_validate_empty_tree(void)
{
	rbtree_t *t = rb_create(NULL);
	assert(t != NULL);

	assert(rb_validate(t) == 0);

	rb_destroy(t);
	printf("test_validate_empty_tree passed\n");
}

static void test_validate_single_insert(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	assert(rb_insert(t, "only", make_int(1)) == 0);
	assert(rb_validate(t) == 0);

	rb_destroy(t);
	printf("test_validate_single_insert passed\n");
}

static void test_validate_after_rotations(void)
{
	static const char *sequences[][3] = {
		{ "a", "b", "c" },
		{ "c", "b", "a" },
		{ "c", "a", "b" },
		{ "a", "c", "b" },
	};

	/* invariant: every sequence inserted so far has left the tree valid */
	for (size_t s = 0; s < sizeof sequences / sizeof sequences[0]; s++) {
		rbtree_t *t = rb_create(count_free);
		assert(t != NULL);

		for (size_t i = 0; i < 3; i++) {
			assert(rb_insert(t, sequences[s][i], make_int((int)i)) == 0);
			assert(rb_validate(t) == 0);
		}

		rb_destroy(t);
	}
	printf("test_validate_after_rotations passed\n");
}

static void test_validate_ascending_run(void)
{
	static const char *keys[] = { "a", "b", "c", "d", "e", "f", "g",
				       "h", "i", "j", "k", "l", "m" };
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	/* invariant: after each insert of a strictly increasing key, the
	 * tree is still a valid red-black tree */
	for (size_t i = 0; i < sizeof keys / sizeof keys[0]; i++) {
		assert(rb_insert(t, keys[i], make_int((int)i)) == 0);
		assert(rb_validate(t) == 0);
	}

	rb_destroy(t);
	printf("test_validate_ascending_run passed\n");
}

static void count_keys_cb(const char *key, void *value, void *ctx)
{
	(void)key;
	(void)value;
	(*(int *)ctx)++;
}

static void test_foreach_empty(void)
{
	rbtree_t *t = rb_create(NULL);
	assert(t != NULL);

	int count = 0;
	rb_foreach(t, count_keys_cb, &count);
	assert(count == 0);

	rb_destroy(t);
	printf("test_foreach_empty passed\n");
}

typedef struct {
	const char *keys[16];
	int count;
} foreach_keys_ctx_t;

static void collect_keys_cb(const char *key, void *value, void *ctx)
{
	(void)value;
	foreach_keys_ctx_t *c = ctx;
	c->keys[c->count++] = key;
}

static void test_foreach_inorder(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	const char *keys[] = { "m", "d", "z", "a", "f", "y", "b" };
	size_t n = sizeof keys / sizeof keys[0];
	for (size_t i = 0; i < n; i++) {
		assert(rb_insert(t, keys[i], make_int((int)i)) == 0);
	}

	foreach_keys_ctx_t ctx = { .count = 0 };
	rb_foreach(t, collect_keys_cb, &ctx);

	assert((size_t)ctx.count == n);
	assert((size_t)ctx.count == rb_size(t));
	/* invariant: keys collected so far are in strictly increasing order */
	for (int i = 1; i < ctx.count; i++) {
		assert(strcmp(ctx.keys[i - 1], ctx.keys[i]) < 0);
	}

	rb_destroy(t);
	printf("test_foreach_inorder passed\n");
}

typedef struct {
	const char *key;
	int value;
} kv_pair_t;

typedef struct {
	kv_pair_t pairs[16];
	int count;
} foreach_kv_ctx_t;

static void collect_kv_cb(const char *key, void *value, void *ctx)
{
	foreach_kv_ctx_t *c = ctx;
	c->pairs[c->count].key = key;
	c->pairs[c->count].value = *(int *)value;
	c->count++;
}

static void test_foreach_values(void)
{
	rbtree_t *t = rb_create(count_free);
	assert(t != NULL);

	assert(rb_insert(t, "beta", make_int(20)) == 0);
	assert(rb_insert(t, "alpha", make_int(10)) == 0);
	assert(rb_insert(t, "gamma", make_int(30)) == 0);

	foreach_kv_ctx_t ctx = { .count = 0 };
	rb_foreach(t, collect_kv_cb, &ctx);

	assert(ctx.count == 3);
	assert(strcmp(ctx.pairs[0].key, "alpha") == 0 && ctx.pairs[0].value == 10);
	assert(strcmp(ctx.pairs[1].key, "beta") == 0 && ctx.pairs[1].value == 20);
	assert(strcmp(ctx.pairs[2].key, "gamma") == 0 && ctx.pairs[2].value == 30);

	rb_destroy(t);
	printf("test_foreach_values passed\n");
}

int main(void)
{
	test_create_destroy();
	test_insert_find_basic();
	test_find_missing();
	test_overwrite();
	test_size_tracking();
	test_stale_root_left_rotation();
	test_stale_root_right_rotation();
	test_stale_root_left_right_zigzag();
	test_stale_root_right_left_zigzag();
	test_validate_empty_tree();
	test_validate_single_insert();
	test_validate_after_rotations();
	test_validate_ascending_run();
	test_foreach_empty();
	test_foreach_inorder();
	test_foreach_values();
	printf("All tests passed\n");
	return 0;
}
