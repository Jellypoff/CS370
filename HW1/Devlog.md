2026-9-10 ~2 hours
This was my first experience with Claude Code and fully AI driven development. I want to improve on my prompts and be better with contracts with claude.
2026-9-15 ~3 hours
This is evening 2 and Claude automatically answered the parent-pointer question with this "I'm including parent now even though today's insert has no fixup, since rotations/fixup in a later milestone need it and retrofitting it later would touch every node-creation site again." It also explained everything well specifically with the functions. I was a little confused on what Claude was doing for rb_insert. There was a note in the spec document that said routing all allocation in src/rbtree.c would help for hw2 and thats why it made an internal dup_key helper. The big problem with this session was fixing my gcc toolchain and getting all the test checks to work. I used way too many tokens to fix that but I eventually got everything fixed and got my first commit for m1 done.
2026-9-15
Evening 3 ~2 and a half hours
This is the equivalent of evening 3. I forgot to run the predict-then-scroll-drill so I did it after I implemented rb_validate. I looked at figures 3 and 4 in the spec. For figure 3, I saw that 5 was red, its parent and uncle were also red which means that we could just repaint 9 and 23 black and then repaint their parent 17, red. The algorithim for this sort of makes sense but the why is still a bit mysterious for me. I also improved on my prompts but was a bit inconsiderate because I didn't tell Claude where to look at times and I let it fill in more blanks than I wanted to.
2026-9-15
Evening 4 ~3hours
I started this session with clearing the session and starting with a new session for M2. I gave it a little too much control and let it do multiple steps in one session. There are 4 parts to m1 and m2 and I wanted to make each prompt do one part of a milestone. This session/prompt got up to 3 parts of m2. This doesn't mean that I wasn't asking Claude questions though and I was actively green lighting it. I also asked it to clarify roughly how many parts of M2 we were through and then what it had done and to walk me through all of it. At this point in my code I am almost done with m2 and I going to start M3 with fuzzer driving delete, 1e5 ops, and asan+memcheck clean.
2026-9-16
Evening 5
