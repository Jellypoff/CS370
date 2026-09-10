Red Black Tree
-Every node is a seperatly allocated heap object attatched to its neighbors by pointers

Required Functions
rb_create
rb_insert
rb_find
rb_delete
rb_size
rb_foreach
rb_validate
rb_destroy

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
