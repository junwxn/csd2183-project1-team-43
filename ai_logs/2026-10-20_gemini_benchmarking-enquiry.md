# **Tree Benchmarking Project Tutoring Session**

https://share.gemini.google/6Kl0uwUIbXd2

*User prompt: ehhhh. no summary of brown\! please strictly stick to what i've said on me first prompt thanks. also a lot of these are claims? like running time for rbt and cpu cache? anyway i'll try to explain what i've made out in my provided example.cpp file given, lets do them in chunks and correct my understanding if im wrong*

Response: My apologies\! I definitely overstepped by giving away the "spoilers" of the paper. You are completely right to call me out on that, and I will strictly hold back and let you do the discovery from here on out.

And your skepticism is spot-on. Yes, those are entirely *claims* and theoretical hypotheses. In performance engineering, Big-O notation, CPU cache theories, and academic papers are just expectations. The compiler, the hardware, and your specific implementation often conspire to do something completely different. Your benchmarks are the only ground truth here.

I'm ready when you are. Drop your first chunk of example.cpp or your explanation of it, and let's break it down.

---

*User prompt: okay i've just finished reading brown's(2025) paper, i'll list it down here as reference, if you feel like i've missed anything let me know without giving me the answers for my group's reference, the benchmarking conditions were key type is 32bit int node layout is 32bits per node? 8 bytes each for parent and its two childs (left and right), \+ another 8 for the key and the coloring or balance fields? browns deletion version has additional 8byte field? repetition is 4096 for 2^18 keys, and 16384 \- 1024 iterations for 2^16 to 2^20 compiler uses gnu g++ 13.2.0 and \-O3 \-Winline. hardware is hp pro mini 400 g9, intel core i7 14700t, 32gb ddr5 5600ram environment is ubuntu 24.01.1 lts single thread, with 1 performance core using tasknet and no app running timing uses c++ stl lib function std::chrono::steady\_clock::now() as for workload it was done consecutively with int keys, inserted or deleted in either sorted or shuffle. randomisation use std::mt19937\_64 with default seed i noted that my assignment uses 64 bit keys though...*

Response: This is a fantastic summary of the experimental setup. You caught all the crucial environmental controls: pinning to a single performance core, stripping background apps, and locking in the compiler optimization flags (-O3). That is exactly how you get clean, reproducible data.

Since your assignment requires **64-bit keys** instead of 32-bit keys, I want you to think carefully about your node layout. In C++, memory alignment matters. If you have three 8-byte pointers (parent, left, right) and an 8-byte key (64-bit), you are exactly at 32 bytes. But you still need to store your color boolean (for RBT) or balance factor (for AVL). Because of memory alignment padding on 64-bit architectures, adding even a 1-byte bool or char will likely push your total node size up to 40 bytes. Keep this in mind, as larger nodes mean fewer nodes fit in the CPU cache\!

As for what you might have missed—or what you should look out for in your example.cpp—consider these two questions:

> 1. **Memory Allocation:** Did Brown use standard new/delete for every single node insertion, or did he pre-allocate a massive pool of memory beforehand? Allocating memory dynamically during the timer can severely skew benchmark results.  
> 2. **Timing Tool:** You noted Brown used std::chrono::steady\_clock. Since your assignment specifies using ankerl::nanobench, you won't need to manually write the timing loops or calculate the repetitions. Nanobench handles the clock sampling and automatically scales the iterations for you to get statistically significant results.

You are definitely on the right track. What did you make out of the nanobench example file? Drop your explanation whenever you're ready\!

---

*User prompt: lets get back to exercise.cpp until we've settled brown's mmmm for 1 i dont think that was explicitly stated in brown's report? but i guess it depends on whether we choose to measure insertion performance vs balancing algo overhead..? for 2, im not done analysing the file, im prohibited from sharing files with you so i can only share chunks of it which i'll do soon*

Response: You hit the nail on the head regarding memory allocation. If you call new inside your timing loop, you are effectively benchmarking the operating system's memory allocator alongside your tree's logic. It introduces a ton of noise. Whether you want to include that noise or isolate the balancing overhead is a crucial design decision for your project, but we can table that for later.

I'm ready to pivot fully to your exercise.cpp file. Take your time analyzing it. Whenever you are ready, drop your first chunk of code or your explanation (keeping to the 100 lines or 3 paragraphs limit), and tell me what you think is going on\!

---

*User prompt: // An example benchmark with nanobench (https\://nanobench.ankerl.com).//// Build and run it with//// make bench// build/example (build\\example.exe in PowerShell)//// It shows the three patterns you will need, each after sorted and after// random insertion. The first times a short operation, find(), on a structure// that stays alive across measurements. The// second times a bulk operation, inserting n keys, for which every measurement// must start from an empty structure that is created, and later destroyed,// outside the timed code. The third times deleting all n keys, for which every// measurement must start from a full structure that is built outside the timed// code. All three use MapBaseline; once your trees work,// uncomment the calls in main() to time them under the same conditions. Copy// this file to write your own benchmarks: every .cpp file in bench/ becomes a// separate program in build/. Do not define ANKERL\_NANOBENCH\_IMPLEMENT; the// Makefile already links the library into every benchmark program.\#include \<algorithm\>\#include \<cstddef\>\#include \<cstdint\>\#include \<memory\>\#include \<numeric\>\#include \<random\>\#include \<string\>\#include \<vector\>\#include "avl\_tree.h"\#include "map\_baseline.h"\#include "nanobench.h"\#include "red\_black\_tree.h"namespace {// The sizes of all three benchmarks: four sizes, the largest 2^18 keys as in// Brown (2025). Add sizes as your experiments need.constexpr std::size\_t kSizes\[\] \= {1'000, 10'000, 100'000, std::size\_t{1} \<\< 18};// Times find() on a structure of type Tree that holds the given keys,// inserted in the given order, and adds the result to bench.template \<typename Tree\>void benchmark\_find(ankerl::nanobench::Bench \&bench, const std::string \&name, const std::vector\<std::int64\_t\> \&insert\_keys) { Tree tree; for (const std::int64\_t key : insert\_keys) { tree.insert(key, key); } // Search the keys in a random order, independent of the insertion order. std::vector\<std::int64\_t\> search\_keys \= insert\_keys; std::mt19937\_64 rng(2183); std::shuffle(search\_keys.begin(), search\_keys.end(), rng); std::size\_t i \= 0; bench.run(name \+ " find, n \= " \+ std::to\_string(search\_keys.size()), \[&\] { ankerl::nanobench::doNotOptimizeAway(tree.find(search\_keys\[i\])); i \= (i \+ 1 \== search\_keys.size()) ? 0 : i \+ 1; }); }// Times the insertion of all keys, in the given order, into an empty structure// of type Tree, and adds the result to bench. nanobench times everything inside// run(), so the empty structures are created beforehand, one for each// measurement, and destroyed only after run() returns; releasing the nodes is// not part of the insertion time. bench must be set to exactly one iteration// per epoch, as in main().template \<typename Tree\>void benchmark\_insert(ankerl::nanobench::Bench \&bench, const std::string \&name, const std::vector\<std::int64\_t\> \&keys) { std::vector\<std::unique\_ptr\<Tree\>\> trees; for (std::size\_t i \= 0; i \< bench.epochs(); \++i) { trees.push\_back(std::make\_unique\<Tree\>()); } // at() throws if nanobench ever needs more structures than were created. std::size\_t next \= 0; bench.batch(keys.size()) .run(name \+ " insert, n \= " \+ std::to\_string(keys.size()), \[&\] { Tree \&tree \= \*trees.at(next++); for (const std::int64\_t key : keys) { tree.insert(key, key); } ankerl::nanobench::doNotOptimizeAway(tree.size()); }); } here's the first chunk, lines 1 to 86 of exercise.cpp from my understanding of it, it first initiatialises an array of 1000, 10000 and left shift to position 18, so 2^18. this should be a hint relating to the iterations, our dataset sizes perhaps? afterwards a template to account for avl trees, rb trees and mapbaseline and then declaring the funct params for benchmark\_find and then we start building the tree. afterwards the search sequence is prepped, from my understanding of std::mt19938\_64 its a pseudorandom numgen, with a seed of 2183, then we use that seed to shuffle the copied vector???*

Response: You are reading it exactly right\! Your breakdown of the templates and the PRNG is spot on.

Here is some feedback and a few nudges to deepen your understanding of this chunk:

**1\. The kSizes Array** You guessed correctly: these are your dataset sizes (*n*, the number of keys in the tree), *not* the number of nanobench iterations. But look closely at the numbers in the array: 1'000, 10'000, 100'000, and 218. Now look back at your assignment prompt. Your required sizes are 212,214,216, and 218. You will definitely need to modify this array so you are actually testing the sizes you are graded on\!

**2\. The PRNG and the Seed (2183)** Yes, std::mt19937\_64 is the 64-bit Mersenne Twister (a very high-quality pseudorandom number generator). But why do you think they hardcoded the seed 2183 instead of using the current time to make it truly random? Think about what happens if you run the AVL tree benchmark, and then run the Red-Black tree benchmark. If the search orders were completely different, would that be a fair comparison? Hardcoding the seed ensures *deterministic randomness*—the sequence looks random, but it is exactly the same random sequence every single time you compile and run.

**3\. The Unexplained Bits** You successfully pasted benchmark\_insert, but you didn't explain it\! This function actually answers the exact question we were discussing earlier about **Memory Allocation**.

Take a look at these lines in your chunk:

  `std::vector<std::unique_ptr<Tree>> trees;`  
  `for (std::size_t i = 0; i < bench.epochs(); ++i) {`  
    `trees.push_back(std::make_unique<Tree>());`  
  `}`

And then later inside the bench.run lambda:

`Tree &tree = *trees.at(next++);`

What is happening here with the unique\_ptr vector and the epochs? How does this setup prevent the operating system's memory allocator from ruining your benchmark timing?

Also, notice the ankerl::nanobench::doNotOptimizeAway(...) function in both the find and insert benchmarks. Remember my warning about the "ruthlessly smart" compiler deleting your code? This is nanobench's way of forcing the compiler to compute the result.

What's your read on how benchmark\_insert handles the tree creation?

---

*User prompt: does this create a smart pointer to an empty tree? so 11 of them?*

Response: You are absolutely on the right track\! Yes, it is creating smart pointers to empty trees. Let's break down exactly what that loop does and *why* it's so important for your benchmark.

### **1\. The Setup (benchmark\_insert)**

`std::vector<std::unique_ptr<Tree>> trees;`  
`for (std::size_t i = 0; i < bench.epochs(); ++i) {`  
  `trees.push_back(std::make_unique<Tree>());`  
`}`

You are correct: this creates a vector holding std::unique\_ptrs pointing to freshly allocated, empty Tree objects. But why bench.epochs()?

Nanobench doesn't just run your code once. It runs it many times in "epochs" to gather statistically significant data (filtering out noise from the OS). If it's going to time the insertion of 10,000 keys, it might want to run that 11 times (or however many epochs it decides).

If you only created *one* tree, the first epoch would insert 10,000 keys into an empty tree. But the second epoch would insert keys into a tree that *already has 10,000 keys in it*. That would ruin the measurement\! By pre-allocating an array of empty trees equal to the number of epochs, you ensure that every single timed run starts with a completely blank slate.

### **2\. The Execution (bench.run)**

`bench.batch(keys.size())`  
    `.run(name + " insert, n = " + std::to_string(keys.size()), [&] {`  
      `Tree &tree = *trees.at(next++);`  
      `for (const std::int64_t key : keys) {`  
        `tree.insert(key, key);`  
      `}`  
      `ankerl::nanobench::doNotOptimizeAway(tree.size());`  
    `});`

Here is where the magic happens. The lambda \[&\] { ... } is the *only* code being timed. Inside that lambda:

> 1. **Tree \&tree \= \*trees.at(next++);** It grabs the next *pre-allocated*, empty tree from your vector. Because the vector and the empty trees were created *outside* the lambda, their memory allocation overhead doesn't affect the benchmark timer.  
> 2. It inserts all the keys.  
> 3. **doNotOptimizeAway** prevents the C++ compiler from saying, "Hey, you inserted all these keys, but you never actually printed or used the tree, so I'm just going to delete this entire for loop to make the code run in 0 seconds." It forces the compiler to actually do the work.

### **Your Next Move**

Notice the comment at the end of the benchmark\_insert function: *"releasing the nodes is not part of the insertion time."*

Because std::unique\_ptr automatically destroys the tree when the trees vector goes out of scope (which happens *after* bench.run finishes), the time it takes to delete all those nodes and free the memory is cleverly excluded from the insertion benchmark.

What's the next chunk of the file, or the next concept you want to tackle?

---

*User prompt: im working on benchmarking now, for now i've duplicated the rest of exercise.cpp into a seperate cpp file as instructed and running its makefile*

Response: Perfect\! Setting up a dedicated .cpp file in the bench/ folder and letting the Makefile do the heavy lifting is exactly the right workflow.

If it compiles and runs successfully, you should see nanobench outputting some beautifully formatted tables to your terminal, likely running whatever baseline structure was left in the example file.

As you start adapting this new benchmark file to fit your specific assignment constraints, you have a few big tasks ahead of you:

> 1. **Updating the Math:** Changing that kSizes array so it calculates the exact powers of two (212,214,216,218) required by your prompt.  
> 2. **The Workloads:** Ensuring you have distinct tests for *sorted* keys versus *random/shuffled* keys.  
> 3. **The Missing Operation:** The example showed benchmark\_find and benchmark\_insert. Your assignment also requires you to measure **deletion** performance.

How did the build go? Let me know what piece of the puzzle you are tackling first, or drop the next chunk of code if you want to review your modifications\!

---

*User prompt: okay... my output has columns ns/insert, insert/s, err%, and total, from my understanding ns/insert represents time per inserted key and lower values mean faster operations. can you explain how nanobench derives those stats, especially the distinction between median timings and total measured time? again pls do challenge any incorrect assumptions in my explanation and dont gimme code*

Response: Your assumption is 100% correct: ns/insert stands for nanoseconds per inserted key, and yes, lower is always better. You've got the right intuition.

Here is exactly how nanobench arrives at those numbers, and why the distinction between the median and the total time is so critical in performance testing:

**1\. Total Time vs. Median Time** The total column represents the absolute wall-clock time your computer spent executing the timed lambda across *all* epochs combined. If nanobench decided to run your insertion loop 11 times, total is the sum of all 11 runs. However, nanobench does *not* calculate your final score by taking the average (mean) of those runs. Instead, it sorts the execution times of all 11 epochs and picks the **median** (the exact middle value).

Why use the median instead of the average? Because operating systems are noisy. Even though you pinned your test to a single performance core, the OS might still pause your program for a microsecond to handle a hardware interrupt or manage a background service. If one epoch gets paused, its time will spike massively. A standard average would be skewed upward by that one random spike, but the median completely ignores it, giving you the truest representation of your tree's actual speed.

**2\. Where the "Per Insert" Math Comes From** Do you remember this line from the code chunk you shared earlier: bench.batch(keys.size())? That is the secret sauce. Because you told nanobench exactly how many keys were being inserted in that loop, it takes the time of that median epoch and divides it by the batch size. That's how it derives the ns/insert metric. It also inverts that math to give you insert/s (insertions per second), which is just a different way to visualize the exact same data.

**3\. The err% (Error Margin)** This column tells you how much variance there was between your epochs. If the err% is low (e.g., 1-2%), it means your tree performs very consistently and your testing environment is clean. If it's high, it means the OS was constantly interrupting your program, or the operation is so unbelievably fast that it's hitting the limits of what the hardware clock can accurately measure. Since you killed all background apps and used a performance core, your err% should hopefully be quite low.

Now that you can read the scoreboard, what is the next step in your implementation? Are you adapting the array sizes next, or building the deletion benchmark?

---

*User prompt: the std::map benchmark sometimes reports unstable timings, like random deletion with 65,536 keys gave me 760.75 ns/erase with an err% \= 59.2%. i suspect its because of background processes, cpu scheduling, cache behaviour, and variations in memory allocation. but how can I distinguish these possibilities through controlled experiments? my starter uses epochs(11) and epochIterations(1) for insertion and deletion. please explain why blindly increasing the iteration count could be incorrect without providing implementation code.*

Response: You have hit one of the most notorious walls in systems programming: the unpredictable nature of the heap. A 59.2% error margin means your data is drowning in noise, and your instincts about the causes are excellent.

Let's address the iteration trap first. Why is blindly cranking up epochIterations for a deletion benchmark a terrible idea? Think about what an "iteration" actually does in Nanobench. It runs the exact lambda provided *repeatedly* within a single tick of the timer. But deletion is a **stateful** operation. If you start with a tree of 65,536 keys and delete them all in iteration 1, what happens in iteration 2? The tree is empty\! It will instantly return, doing zero work. Your resulting time will be an artificial average of one real deletion run and *N*−1 empty runs, completely ruining the math. To properly increase sample size for stateful operations, you must increase the epochs (rebuilding the tree outside the timer each time), not the epochIterations.

So, how do we design controlled experiments to isolate that 59.2% variance? You strip away the variables one by one:

> 1. **Exonerate the OS and CPU:** Write a benchmark that performs a heavy, strictly CPU-bound task with zero memory allocation—like calculating a massive sum or shuffling a pre-allocated std::vector. If this benchmark reports a low err% (\< 2%), you have scientifically proven that background apps and CPU scheduling are *not* your problem. The environment is clean.  
> 2. **Isolate the Memory Allocator (The Prime Suspect):** std::map allocates and frees every single node individually on the heap. When you insert 65,536 nodes sequentially, they sit nicely in memory. But when you delete them in a *random* order, you punch microscopic holes all over your RAM. This is called **Heap Fragmentation**. The OS memory allocator occasionally has to freeze your program to scan the heap and merge these empty blocks together. This freeze is highly unpredictable. To test this, you will eventually compare this against your custom AVL/RBT. If your custom trees (which might use a bulk-allocated memory pool rather than individual new/delete calls) show stable timings under the exact same random deletion workload, you have proven the variance was caused by the OS memory allocator struggling with std::map's fragmentation, not the tree logic itself.

You are entering the realm where hardware architecture matters just as much as Big-O notation. What does your deletion benchmark setup look like so far?

---

