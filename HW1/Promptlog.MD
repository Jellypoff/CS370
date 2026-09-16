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

Prompt: Start working on insert_fixup + mirrors and validate checks black-height. insert_fixup was already implemented but update it with mirrors now. Build on what is in rbtree.c and in the tests directory.

Notes: I was a little confused on what mirror meant but Claude explained this as handling mirror branches when a parent is a left or right child. Claude caught this and asked me if I just wanted to work on rb_validate and I said yes. After it finished its plan with rb_validate, I asked Claude to walk me through its implementation so I could understand how its actually validating in C. I wanted to be thorough with rb_validate and I had in my notes that rb_validate needed to maintain parent pointers and make sure that the rotation keeps track of t-> root. This was a little bit of a backtrack but it wasn't too costly this time. Claude then explained what the spec meant by maintain and I understood this better. rb_validate is just a diagnostic pass and returns 0 of every invariant holds, and 1 if not or if there is a violation. It also confirmed that rotation already correctly updates t->root because when the node that is being rotate has no parent, it is the root.

Episode 3
Prompt: Lets start working on the last bit for M1. Build on rbtree.c and the tests directory. Complete the rb_foreach method + in-order tests;fuzzer (inserts/finds) vs model. Walk me through your implementation.

Notes:I sent this a bit too fast. I should've asked it to walk me through the fuzzer because I'm still fuzzy on its implementation. Claude gave good clarification on the fuzzer and that was sufficient. This was the end of M1 and all the tests passed. I asked Claude to run the tests and to show me output after the end of this, and it did. I have also been running the tests throughout each commit to make sure it works on my end as well.

Episode 4
Prompt: Write the failing table-driven tests for rb_delete first: red leaf, black leaf with red sibling, node with two children, root deletion, a black node with exactly one (red) child, and the mirrored variants. Each case asserts rb_validate and rb_size afterward. Don’t touch rbtree.c yet Run make test and show me the failures.

Notes: This prompt was out of the spec. Some key things it told me was that rbnode is fully opaque outside and that there was no way for test code to inspect node color or shape directly so hitting a specific strucutral case meant picking the right insertion order for rb_insert. It gave me some steps and started with step 1 which was to verify the mirror tree shape empirically.

episode 5
Prompt: Lets start working on the last parts of m2. Walk me through your implementation and how it connects to the other steps of m2. 

Notes: I wanted to see if there were any glaring issues that I wasn't seeing or any edge cases that weren't being covered. I looked through my code and I didn't see anything wrong.

episode 6
prompt: Lets start working on M3: fuzzer with deletes, 1e5 ops, asan+memcheck clean. I want you to add focused, deterministic tests for these three M3 edge cases:
1. Empty tree
2. Single-node tree
3. Overwriting the only key
Use the existing test conventions and helper functions in the repository. Do not weaken or remove existing tests, and do not change production code yet. First show me your proposed test cases and explain what each assertion is checking. Then implement only these M3 edge-case tests. Run the relevant test command and show me the actual output.

Notes: Throughout this whole process of learning prompts, I've really only done a couple of sentences because I thought that was efficient and saved tokens. For this last stretch of hw1 I wanted to make very detailed prompts and make sure that Claude wasn't fixing things that didn't need to be fixed or making extra unnessecary code. Claude made tests to test_rbtree.c that followed the style of the other ones and gave me what I wanted. After the tests were implemented, I asked Claude to review the tests and explain what behavior its checking and why that specific behavior is required by the spec.

PROMPTLOG.md: 4–6 annotated episodes, each with the prompt, what came back, and your
judgment (accepted, rejected, or pushed back on, and why). Must include one plan you revised(done)
(Step 3), one rejected/oversized diff(done) (Step 4), one tool-output debugging loop (Step 5), and one review finding you triaged (Step 7). Raw transcript dumps here score nothing; annotation is the
deliverable.