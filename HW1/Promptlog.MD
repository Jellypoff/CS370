Episode 0: 
Finished M0 and familiarized myself with Claude. Need to study red-black trees a little more to deepen my understanding. These are some notes I took on questions that I had about red-black trees. 
Prompt: Read include /rbtree.h and CLAUDE.md. Walk me through a left rotation, one pointer assignment at a time. For each pointer, explain what it pointed to before and after.
Notes: 
Walkthrough was for the standard CLRS-style layout the assignment implies
Sentinel: Asked what the sentinel was, still fuzzy on its meaning. Sentinel is "one real, allocated node that stands in for "nothing" It is essentially a placeholder node. Used instead of Null to mark the abscence of a child, every missing pointer in the tree points to the sentinel.
Read through the explanation and then asked the agent to teach me what a red-black tree was as simply as it possibly could.
red-black tree doesn't ever get too lopsided, its "balanced enough"
The longest root-to-leaf path is at most twice the shortest one
Idea of pointer surgery through recoloring or rotations all made locally.
Agent also gave a recap of all the prompts.
I thought that the responses were very thorough and followed my CLAUDE.md file well.

Episode 1:
prompt: Lets start on the redblack tree from scratch. Let's first start with the node structure, create/insert(no fixup)/find + unit tests. Look in rbtree.h and rbtree.c.
notes: Started with a small slice to see what Claude would do.  Claude also asked to make a trivial stub main to fuzz.c so I could actually run my code, and skeletons of rb_destroy and rb_size so test/asan/memcheck pass. I should've also asked it to make one of these so I could incrementally test my code. Claude returned a skeleton for the things I asked it to and explained how it made sure that all allocation was routed through src directory for hw2. It also explicitly explained how insert with no fixup would work and how it would be developed later in other milestones. I thought that everything was explained well and I followed with the plan. After accepting the plan Claude wanted to run commands that I wasn't familiar with and after asking it what it was trying to run, I realized that I didn't have the gcc toolchain downloaded. My computer crashed shortly after tis and my most recent Claude log with all of this deleted! I also had to fix the gcc toolchain and that wasted a lot of tokens when I just had to remote to test make file, make asan, and make memcheck.

Episode 2:
M1- rotations, stale-root regression testing, and insert_fixup.
Prompt:  Implement the rotations own t->root and the stale-root regression test. Build on rbtree.c, fuzz.c, and test_rbtree.c. Walk me through your implementation.
Notes:
Claude asked me how I wanted to do tests. It asked me if I wanted to use whitebox testing, but I was more familiar with blackbox testing. This was an oversight on my part.  I also told Claude to pull insert_fixup forward into this part of milestone 1. This made more sense to me because it kept everything black-box. This made everything more simple at this time so no new files or significant changes were made, other than the code of course. Claude explained the tests it was going to add but I wanted more explanation as to why it chose the tests and how they correlated to stale-root regression.