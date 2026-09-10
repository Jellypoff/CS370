Red Black Tree
-Every node is a seperatly allocated heap object attatched to its neighbors by pointers


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