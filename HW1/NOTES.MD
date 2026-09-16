Red Black Tree
-Every node is a seperatly allocated heap object attatched to its neighbors by pointers
-In order sequence, 1,a,1,b,2 would be kept on left or right rotations
-A new key always enters the tree as a red node at a leaf position, leaves the black-height count unchanged
-recoloring and rotations are the whole toolkit
-insert fixup is a loop
-Be careful with the pointer that points to root, keep this updated when using rotations and recolors.
-Removing a red tree costs nothing
-Keep track of deleted black nodes, where do pointers go?
-Corner case- deleting a black node with one child
-Systems goal: Build a red-black tree in C and own every byte of it: correct rebalancing above,
airtight allocation and teardown below. A correct tree that leaks, double-frees, or reads freed
memory fails, even if every lookup returns the right answer.
-Tree copies keys on insert and owns the copies

M0: repo skeleton, frozen header, Makefile, .gitignore
M0: CLAUDE.md project law (from /init, then edited)
M1: node struct, create/insert(no fixup)/find + unit tests
M1: rotations own t->root; stale-root regression test
M1: insert_fixup + mirrors, validate checks black-height
M1: rb_foreach + in-order test; fuzzer (inserts/finds) vs model
M2: table-driven delete tests (failing, on purpose)
M2: two-children reduction + one-child splice, half the table green
M2: delete_fixup all cases + mirrors, table green
M2: fuzzer with deletes, 1e5 ops, asan+memcheck clean
M3: edge cases: empty tree, single node, overwrite-only-key
M3: post-review fixes from adversarial pass (1 real, 1 rejected)
docs: PROMPTLOG episodes 1-5 annotated, REFLECTION drafted
Check that the tests cover:
   - empty tree
   - single-node tree
   - deleting the only node and returning to an empty tree
   - overwriting the only existing key
   - rb_size remaining correct
   - rb_find returning the expected value
   - rb_validate remaining valid
   - rb_destroy working on an empty tree
   - the old value being freed exactly once during overwrite
   - the replacement value being freed exactly once during destruction
Check that the ownership/free-count helper is test-local and does not modify src/rbtree.c.Run the relevant test suite. Show me the actual test output, including any failures. If all tests pass, do not change production code yet. Explain whether these tests are capable of catching an incorrect implementation of the M3 edge cases.

Required Functions
M1
Node struct
- rb_create
- rb_insert(without fixup)
- rb_find(with full deletion-fixup cases)
- rb_validates ordering check 
- comfortable recursive rb_destroy


- rb_delete(with full deletion-fixup cases)
- rb_size
- rb_foreach
- rb_validate

Compile with:
-std=c23
-Wall -Wextra -Werror

Required verification:
make test
make asan
make memcheck


Learning objectives 
• Read and write enough C to build a pointer-linked data structure, and know at every moment
which piece of code owns which allocation.
• Implement a red-black tree in C and reason precisely about node ownership, lifetimes, and the
constant-time pointer surgery in rotation and fixup.
• Verify memory-management correctness with sanitizers and valgrind, treating a single leaked
byte or invalid access as a failure, not a warning.
• Design and defend a C module against a frozen public API: struct layout, ownership contract,
and the error-code discipline the grading harness depends on.
• Direct an AI coding agent (specifically, Claude Code) the way a senior engineer directs a capable
but overconfident junior: give it context, make it plan, keep changes small, verify everything
with tools, and merge nothing you cannot explain.


A red-black tree is a binary search tree that keeps itself approximately balanced by attaching one
bit of bookkeeping, a color, to every node. Five rules govern the colors:
1. Every node is red or black.
2. The root is black.
3. Conceptually, every missing child ends at a black sentinel leaf, written NIL.
4. A red node has only black children, so reds never stack.
5. Every path from a given node down to any NIL leaf crosses the same number of black nodes.
This count is called the black-height.

Goals 
1. Systems goal: Build a red-black tree in C and own every byte of it: correct rebalancing above,
airtight allocation and teardown below. A correct tree that leaks, double-frees, or reads freed
memory fails, even if every lookup returns the right answer.
2. Process-Engineering-with-AI goal: Learn to direct an AI agent the way a senior engineer
directs a capable but overconfident junior: give it context, make it plan, keep changes small,
verify everything with tools, and never merge code you can’t explain.
The anti-goal: pasting this assignment into Claude and typing “solve this.” That approach
will actually hurt you here, for three reasons: (a) the targeted deletion tests and the grading harness
punish code nobody reviewed, (b) part of your grade comes from your process artifacts and a live
walkthrough, and (c) you will be asked to explain arbitrary lines of your submission (live), and to
extend it on parameters you have never seen.


Before each milestone commit, get a review from a context that did not write the code, because
a reviewer that shares the author’s assumptions inherits its blind spots. Either run the bundled
/code-review skill, or /clear and prompt:
Review the diff for Milestone 2 as a hostile kernel reviewer. Hunt specifically for: a use-afterfree in the successor splice, a key copy leaked on the overwrite path, allocations whose NULL
return is never checked, and any path where rb_destroy can miss a subtree. Report findings with
file:line; do not fix anything.

Confusions list:
Read include /rbtree.h and CLAUDE.md. Walk me through a left rotation, one pointer assignment at a time. For each pointer, explain what it pointed to before and after.
