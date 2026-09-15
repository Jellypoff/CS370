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
