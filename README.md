# Error Reporting and Handling: My Learning and Thinking

(Status: work in progress)

## 1. What is a caller and what is a callee?

A caller is the code that calls a function.

A callee is the function that is called by the caller. I also use "called function" to refer to a callee.

## 2. What is an error?

I highly recommend [1] which defines "errors" clearly:

> Within a function **f**, a failure is an error **if and only** if it prevents **f** from meeting any of its callees' _preconditions_, achieving any of **f**'s own _postconditions_, or reestablishing any _invariant_ that **f** shares responsibility for maintaining.

The _preconditions_ are the starting states when a function is called. The **caller** is responsible for satisfying them, and reporting errors if they can't be satisfied.

The _postconditions_ are the goals that the called function wants to achieve. The **function** is responsible for satisfying them, and reporting errors if they can't be satisfied.

The _invariants_ are the valid states that the program (or an object in the program) must maintain. The called **function** is responsible for maintaining them, and reporting errors if they can't be maintained.

## 3. Error categories: abstract machine corruption; programming bug; recoverable error

[7a] puts the errors into three categories:

- **Abstract machine corruption**, such as stack exhaustion.
- **Programming bug**, mostly errors in satisfying the function's preconditions.
- **Recoverable error**, such as file not found, host not reachable.

## 4. Error categories: recoverable and non-recoverable

The Rust programming language [9] puts errors into two categories:
- recoverable
- non-recoverable

Abstract machine corruption and programming bug are actually considered non-recoverable errors.

## 5. Error categories: fatal; boneheaded; vexing; exogenous

(TODO)

## Error categories: No hard line

There isn't always a hard and clear line to draw to say "hey, this kind of error is definitely non-recoverable".

For example, out-of-memory error. The caller may not have a way to handle it and should probably just quit. But C++ provides `set_new_handler` to set a handler function to be called when the `new` operator fails, and one of the purposes of this `new` handler is to make more memory available (while the other two are to terminate the program, if needed, or to throw the `std::bad_alloc` exception, if either is needed).

## Error reporting and error handling

Most of the time, people use "error handling" to talk about both error reporting and error handling. I am totally OK with that. In fact, I also use "error handling" to refer to both in many occasions. But in this article, I want to distinguish them:

- **Error reporting** is all about "telling the caller about the details of an error". This is usually (not always, see the _precondition errors_ above) done by the callee in various ways:
  - Indicating an error condition with particular return codes.
  - Setting a global variable to indicate errors (e.g., `errno`).
  - Throwing an exception.
- **Error handling** is all about "catching and dealing with the error". This is usually done by the caller.

## Standard of good error reporting and handling

In order to evaluate if an error reporting and handling mechanism is good, we need to define the standard of "good". I'm proposing the following standard.

1. S1: Error reporting should provide sufficient details for the caller to handle the error properly.
2. S2: Error handling should keep the program in valid states all the time.

I'll discuss them a bit more.

[2] points out one problem of bad error reporting:

> ... important information was lost: The state of the database creation. The code that catches the exception doesn't know which step in database creation failed. Does it need to delete the indexes? Does it need to delete the tables? Does it need to delete the physical database? It doesn't know.

This problem can happen when the error reporting tells "something is wrong" but does not provide enough details for the caller to figure out "in **what** context **what** exactly is wrong". These two _whats_ are important: The first _what_ tells the caller what has been done and what has not been done yet (e.g., which step in database creation failed); the second _what_ tells the caller what the possible cause of the error is (e.g., how to deal with it: deleting the indexes, tables, or physical database).

## Good error reporting and handling

When choosing the mechanism of error reporting and handling, I suggest to consider the following aspects:

1. In terms of reporting the details of the error, it doesn't matter how they are reported. In other words, returning error codes or throwing exceptions essentially do the same job. It matters what are reported. See standard S1.
2. Programming languages have built-in restrictions on the choices. For example, you can't throw exceptions in C, and you can't return error codes in C++'s constructors.
3. If throwing an exception can potentially cause performance penalty, as [3] describes:

>  Because actually handling an exception is likely to be significantly slower than executing mainline code, you should also ask: "Can I afford stack unwinding here?"

When implementing error reporting and handling, I'm proposing the following implementation.

1. A function should report all possible errors it can encounter. If a function fails to report all the possible errors, it is a bug and should be fixed in the next version.
2. When calling a function, the caller should handle almost all (see the exceptions below) the possible errors of the called function. It's OK that the caller may not be able to fix the errors and have to report them to higher caller, but when reporting to the higher caller, the current caller must not pass on the callee's error directly. Instead, the current caller should report the error in the current context, so the higher caller will be able to understand the error in the right context.
  - Imagine you are the CEO of a large company. Typically, the comapny is organized in levels of hierarchy and you being the top level only talk to the immediately next level of managers most of the time. The company may have established special occasions or channels for employees on the lower levels to speak up directly to the higher levels, but on a daily basis, work or issues are reported level by level. One important reason is the higher level managers may not have all the details to understand the lower level issues because their work contexts are so different. The further the two levels are, the more difficult to understand. Allowing the lower level APIs to throw an exception all the way up through the calling hierarchy is similar to allowing an entry-level employee to rush into the CEO's office and complain a particular column of numbers on the data sheets he was dealing with didn't have enough precision. You being the CEO may be willing to help to solve the problem but you may not have the details to know what caused the bad number precision and where you should look into to fix it.
  - An exception is: If the error from the lower callee makes sense to the higher caller, it is OK to simply rethrow it or just let it propagate automatically. For example, an error called `NetworkDisconnected` may make pretty much sense and the program probably cannot fix it at all. In this case, if the lowest callee detects this issue, it is OK to throw it all the way to the top caller because this error makes sense to all the levels. However, an error of `FileNotFound` may make sense to some lower level callers but may not make sense to the higher level callers if the higher level callers have no knowledge about the low-level storage details. In this case, the error `FileNotFound` should be caught before it is thrown to the caller that doesn't know anything about storage details.
3. It's OK to catch generic exceptions such as `std::exception` or `...` in C++ or similar constructs in other programming languages. But this should be treated as **truly unexpected** situations that **nobody knows** how to deal with them so the only valid handling is to terminate the program, ask the user to report a bug, so the developers can investigate further. In other words, during the testing phase, if somebody knows some condition may happen (even rarely) but does not catch it with a specific exception, it should be considered as a bug.

## Others

[1] quotes the words by Stroustrup:

> "It is not a good idea to try to make every function a fire-wall. The best error-handling strategies are those in which only designated major interfaces are concerned with non-local error handling issues." —[Stroustrup94] 16.8

I disagree (so far). When you feel it wrong "to make every function a fire-wall", I guess it's probably because you don't add context when you report an error. If you simple catch an error and rethrow it without contributing more contextual details, then catching in every function does look pretty dull and "wrong".

## Error: Out of range

Out-of-range error is a kind of programming bug that should be fixed, unless it is used as a way to check if the given index is in the range.

On the surface, it looks like that "out of range" is an expected error: what if the index is entered by the user?

But the point is: the correct programming is to check the index before actually using it to access the array. If you don't check it and get out-of-range error, it is a programming bug.

## TODOs

- [ ] What does "recoverable error" mean?
  - [What does mean by recoverable and un-recoverable exception or error](https://stackoverflow.com/q/36099102/630364)
- [ ] Finish studying [this issue](https://github.com/yaobinwen/robin_on_rails/issues/90)
- [ ] [Destructors that throw](https://akrzemi1.wordpress.com/2011/09/21/destructors-that-throw/)

## Examples

Work in progress: I want to write example code to demonstrate my ideas.

### Demo: vector

This demo implements a very simple vector. The input parameters are checked using `assert` instead of throwing exceptions. The purposes of this demo are:
1. Learn how `assert` works for pre-condition checks.
2. Learn how to handle the errors appropriately in library code.

1. `assert` is more logically clear because it states the violation of pre-conditions (i.e., mistakes by the caller). However, it may make writing test code harder because `assert` crashes the program.
2. `assert` may also require the caller do sufficient tests to make sure all the calls of the function are covered, otherwise, there may be run-time crashes.

### Demo: File

(TODO)

The file operation is an example of "reporting error via `errno` but nobody knows what values `errno` could be in different cases" unless the developers read POSIX interface document. But the library developers have the responsibility to encapsulate the details and present the errors in a better way.

If you tell the caller that the detailed error is indicated by `errno` without listing what specific error numbers can be, you are essentially encouraging them to only display the `errno`. The caller can still figure out all the possible `errno`, but usually people are lazy.

## Other issues

1. Many times, the warnings only describe what the warnings are but do not tell why and how to possibly solve them. As a result, only the developers who wrote the warnings understand what they are and how to deal with them. For maintainers or future developers, they need to spend more time to learn about them.

## References

- [1] [Herb Sutter: When and How to Use Exceptions](https://drdobbs.com/when-and-how-to-use-exceptions/184401836)
- [2] [Raymond Chen: Cleaner, more elegant, and wrong](https://devblogs.microsoft.com/oldnewthing/20040422-00/?p=39683)
- [3] [Error and Exception Handling](https://www.boost.org/community/error_handling.html)
- [4] [Raul Jordan: Why Go's Error Handling is Awesome](https://rauljordan.com/2020/07/06/why-go-error-handling-is-awesome.html)
- [5] [Rob Pike: Errors are values](https://go.dev/blog/errors-are-values)
- [6] [Hacker News: Go proposal: leave "if err != nil" alone?](https://news.ycombinator.com/item?id=20303468)
  - [6a] [Go issue #32825: proposal: leave "if err != nil" alone?](https://github.com/golang/go/issues/32825)
- [7] [De-fragmenting C++: Making Exceptions and RTTI More Affordable and Usable - Herb Sutter CppCon 2019](https://youtu.be/ARYP83yNAWk)
  - [7a] [Herb Sutter: De-fragmenting C++: Making exceptions more affordable and usable](https://accu.org/conf-docs/PDFs_2019/herb_sutter_-_de-fragmenting_cpp__making_exceptions_more_affordable_and_usable.pdf)
- [8] [Eric Lippert: Vexing exceptions](https://learn.microsoft.com/en-us/archive/blogs/ericlippert/vexing-exceptions)
- [9] [The Rust Programming Language: Error Handling](https://doc.rust-lang.org/book/ch09-00-error-handling.html)
- [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html)
- [Herb Sutter: Questions About Exception Specifications](https://herbsutter.com/2007/01/24/questions-about-exception-specifications/)
- [Herb Sutter: Zero-overhead deterministic exceptions: Throwing values](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0709r4.pdf)
- [Using Exceptions in C++ a practical guide](https://cpp.mimuw.edu.pl/files/Using%20Exceptions%20in%20C++.pdf)
- [Arthur O'Dwyer: Thoughts on Herb Sutter’s CppCon keynote](https://quuxplusone.github.io/blog/2019/09/24/thoughts-on-eh/)
- [Exceptions and Error Handling](https://isocpp.org/wiki/faq/exceptions)
- [cppreference.com: Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)
