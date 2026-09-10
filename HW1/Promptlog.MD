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
