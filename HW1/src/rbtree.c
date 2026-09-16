#include "rbtree.h"

#include <stdlib.h>
#include <string.h>

typedef enum { RB_RED, RB_BLACK } rb_color_t;

typedef struct rbnode {
	char *key;
	void *value;
	rb_color_t color;
	struct rbnode *left;
	struct rbnode *right;
	struct rbnode *parent;
} rbnode_t;

struct rbtree {
	rbnode_t *root;
	rbnode_t nil;
	size_t size;
	rb_value_free_fn value_free;
};

static char *dup_key(const char *key)
{
	size_t len = strlen(key) + 1;
	char *copy = malloc(len);
	if (copy == NULL) {
		return NULL;
	}
	memcpy(copy, key, len);
	return copy;
}

rbtree_t *rb_create(rb_value_free_fn value_free)
{
	rbtree_t *t = malloc(sizeof *t);
	if (t == NULL) {
		return NULL;
	}

	t->nil.key = NULL;
	t->nil.value = NULL;
	t->nil.color = RB_BLACK;
	t->nil.left = &t->nil;
	t->nil.right = &t->nil;
	t->nil.parent = &t->nil;

	t->root = &t->nil;
	t->size = 0;
	t->value_free = value_free;
	return t;
}

static void rb_rotate_left(rbtree_t *t, rbnode_t *x)
{
	rbnode_t *y = x->right;

	x->right = y->left;
	if (y->left != &t->nil) {
		y->left->parent = x;
	}

	y->parent = x->parent;
	if (x->parent == &t->nil) {
		t->root = y;
	} else if (x == x->parent->left) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

static void rb_rotate_right(rbtree_t *t, rbnode_t *y)
{
	rbnode_t *x = y->left;

	y->left = x->right;
	if (x->right != &t->nil) {
		x->right->parent = y;
	}

	x->parent = y->parent;
	if (y->parent == &t->nil) {
		t->root = x;
	} else if (y == y->parent->right) {
		y->parent->right = x;
	} else {
		y->parent->left = x;
	}

	x->right = y;
	y->parent = x;
}

static void rb_insert_fixup(rbtree_t *t, rbnode_t *z)
{
	/* invariant: z is red, and z is the only red-red violation in the tree */
	while (z->parent->color == RB_RED) {
		if (z->parent == z->parent->parent->left) {
			rbnode_t *y = z->parent->parent->right;
			if (y->color == RB_RED) {
				z->parent->color = RB_BLACK;
				y->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->right) {
					z = z->parent;
					rb_rotate_left(t, z);
				}
				z->parent->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				rb_rotate_right(t, z->parent->parent);
			}
		} else {
			rbnode_t *y = z->parent->parent->left;
			if (y->color == RB_RED) {
				z->parent->color = RB_BLACK;
				y->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				z = z->parent->parent;
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					rb_rotate_right(t, z);
				}
				z->parent->color = RB_BLACK;
				z->parent->parent->color = RB_RED;
				rb_rotate_left(t, z->parent->parent);
			}
		}
	}
	t->root->color = RB_BLACK;
}

int rb_insert(rbtree_t *t, const char *key, void *value)
{
	rbnode_t *parent = &t->nil;
	rbnode_t *cur = t->root;
	int cmp = 0;
	rbnode_t *node;

	/* invariant: cur is the still-unsearched subtree; parent trails one step behind cur */
	while (cur != &t->nil) {
		cmp = strcmp(key, cur->key);
		if (cmp == 0) {
			if (t->value_free != NULL && cur->value != NULL) {
				t->value_free(cur->value);
			}
			cur->value = value;
			return 0;
		}
		parent = cur;
		cur = (cmp < 0) ? cur->left : cur->right;
	}

	node = malloc(sizeof *node);
	if (node == NULL) {
		goto fail;
	}

	node->key = dup_key(key);
	if (node->key == NULL) {
		goto fail_node;
	}

	node->value = value;
	node->color = RB_RED;
	node->left = &t->nil;
	node->right = &t->nil;
	node->parent = parent;

	if (parent == &t->nil) {
		t->root = node;
	} else if (cmp < 0) {
		parent->left = node;
	} else {
		parent->right = node;
	}

	rb_insert_fixup(t, node);
	t->size++;
	return 0;

fail_node:
	free(node);
fail:
	return -1;
}

void *rb_find(const rbtree_t *t, const char *key)
{
	const rbnode_t *cur = t->root;

	/* invariant: cur is the still-unsearched subtree that may contain key */
	while (cur != &t->nil) {
		int cmp = strcmp(key, cur->key);
		if (cmp == 0) {
			return cur->value;
		}
		cur = (cmp < 0) ? cur->left : cur->right;
	}
	return NULL;
}

size_t rb_size(const rbtree_t *t)
{
	return t->size;
}

/* invariant: subtree at node is a valid RB-tree with all keys in (lo, hi),
 * else return 0; on success *black_height is the subtree's black-height */
static int rb_validate_rec(const rbtree_t *t, const rbnode_t *node,
			    const char *lo, const char *hi, int *black_height)
{
	if (node == &t->nil) {
		*black_height = 0;
		return 1;
	}
	if ((lo != NULL && strcmp(node->key, lo) <= 0) ||
	    (hi != NULL && strcmp(node->key, hi) >= 0)) {
		return 0;
	}
	if (node->color == RB_RED &&
	    (node->left->color == RB_RED || node->right->color == RB_RED)) {
		return 0;
	}
	if ((node->left != &t->nil && node->left->parent != node) ||
	    (node->right != &t->nil && node->right->parent != node)) {
		return 0;
	}

	int bh_left, bh_right;
	if (!rb_validate_rec(t, node->left, lo, node->key, &bh_left)) {
		return 0;
	}
	if (!rb_validate_rec(t, node->right, node->key, hi, &bh_right)) {
		return 0;
	}
	if (bh_left != bh_right) {
		return 0;
	}

	*black_height = bh_left + (node->color == RB_BLACK ? 1 : 0);
	return 1;
}

int rb_validate(const rbtree_t *t)
{
	if (t->root != &t->nil && t->root->color != RB_BLACK) {
		return 1;
	}
	int black_height;
	return rb_validate_rec(t, t->root, NULL, NULL, &black_height) ? 0 : 1;
}

static void rb_foreach_rec(const rbtree_t *t, const rbnode_t *node,
			    void (*fn)(const char *key, void *value, void *ctx),
			    void *ctx)
{
	if (node == &t->nil) {
		return;
	}
	rb_foreach_rec(t, node->left, fn, ctx);
	fn(node->key, node->value, ctx);
	rb_foreach_rec(t, node->right, fn, ctx);
}

void rb_foreach(const rbtree_t *t, void (*fn)(const char *key, void *value, void *ctx),
		 void *ctx)
{
	rb_foreach_rec(t, t->root, fn, ctx);
}

static void free_subtree(rbtree_t *t, rbnode_t *node)
{
	if (node == &t->nil) {
		return;
	}
	free_subtree(t, node->left);
	free_subtree(t, node->right);
	if (t->value_free != NULL && node->value != NULL) {
		t->value_free(node->value);
	}
	free(node->key);
	free(node);
}

void rb_destroy(rbtree_t *t)
{
	if (t == NULL) {
		return;
	}
	free_subtree(t, t->root);
	free(t);
}
