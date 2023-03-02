# re-java-2-times-faster-than-c

Someone brought me to this repository https://github.com/xemantic/java-2-times-faster-than-c and because just the title alone annoyed me, I modified the authors c code to demostrate that his claim is false. As he says by himself he has "limited C experience" so I can see where he is coming from.

Java is designed and optimized to write code in a certain style. When you translate every statement of a Java code into C it might not be optimal.

What the author demonstrated is that the memory management system of Java is more time efficient than using malloc and free of c in exactly the same way.
An actual fair comparison between Java and C would have been if the C code used the same memory management algorithm as the JVM.

According to his benchmark the Java code is 1.5x faster than the C code (8.5 s vs 12.8 s).
The modified C code here is 2.8x faster than the original C code (3.4 s vs 9.6 s) thus in comparison much faster than the Java code (almost 2x).

The code was compiled and run on an Apple silicon mac using clang and -O3 as the only compiler flag.

Here is how the dialog should actually go:
```
"Your code running on a virtual machine will be ALWAYS slower than equivalent native code."

"Why?"

"Because of automatic memory management."

"Why is it so?"

"Things like automatic memory management ALWAYS add additional overhead to execution."

"Hmm, let me try, here is a code in Java, and direct equivalent in C, the first one is almost 2 times faster."

"It's because you are doing things wrong. You should not write C code just like Java code."

"Why?"

"Because you need to properly manage your memory for efficiency."

"How do you do it?"

"There is no one solution fits all. But it is often about preallocation and memory reusage."

"Ok, so did you just make contradictory statements?"

"No, just add these few lines to your code."

"Do you think it's still the same algorithm afterwards?"

"It is the same algorithm just like the original Java and C versions are the same. They use different memory managment."
```