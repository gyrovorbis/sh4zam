# Contributing

So you want to contribute in some way to SH4ZAM? Sweet. We're pretty casual and easy-going. Nothing super formal is really required. Just a bunch of nerds trying to give the Dreamcast and SH4 communities some fast math is all. :)

## Desired Contributions
Pretty much anything that could be seen as adding value to the library, examples, documentation, or anything tangentially related to it is something we'll happily accept. We don't care how large or small it is. 
Don't be shy!

This is not an exhaustive list, but here are some examples of useful contributions:
- Adding new API routines
- Optimizing existing API routine implementations
- Adding new examples
- Improving unit test coverage
- Improving documentation in any way
- Adding or fixing code comments (including fixing typos)

## Opening a PR
Just try to be as descriptive as you can in terms of summarizing your changes, as that will make it easier to review. We don't have any formal requirement on PR format or anything like that, so don't sweat it too much.
Just get us some gainz and help make the lib kick ass!

Oh, if you're unsure about something or want some early feedback on something before its ready for a full PR, feel free to simply open the PR as draft!

## Coding Style
We do make an effort to keep the coding style consistent throughout the codebase; however, we aren't going to turn down some juicy optimizations over nit-picking some little coding-style inconsistencies... Falco can fix it up as part of the PR or a later commit.

## Doxygen Comments
Everything that is a part of the public API should be fully Doxygen commented; however, again, if this is too much of a PITA, if English isn't your first langauge, if you're allergic to writing, or if you simply don't have time,
just mention it in the PR, and we should be able to cover you.

## Unit Testing
We are striving hard to hit 100% unit test coverage, but we understand that, especially at this early stage, not everyone contributing is going to have time to fully unit test everything, which is fine. Just make sure to run the currently implemented unit
tests to ensure you haven't broken anything that DOES have test coverage... and Falco will get around to adding new test coverage eventually.

## C++ Bindings
We strive to keep the C++ bindings in-sync with our C API; however, for obvious reasons (like C++ is insane), SH4 and C code contributors are not going to be expected to provide idiomatic modern C++ bindings for all contributions... Falco will handle it.
