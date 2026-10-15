---
title: Contributing
description: How to contribute to SH4ZAM, and to this documentation site
---

So you want to contribute in some way to SH4ZAM? Sweet. The project is pretty casual and easy-going; nothing super formal is required. Just a bunch of nerds trying to give the Dreamcast and SH4 communities some fast math.

## Desired contributions

Pretty much anything that could be seen as adding value to the library, examples, documentation, or anything tangentially related to it will happily be accepted, no matter how large or small. Some examples:

- Adding new API routines
- Optimizing existing API routine implementations
- Adding new examples
- Improving unit test coverage
- Improving documentation in any way
- Adding or fixing code comments (including fixing typos)

## Opening a PR

Try to be as descriptive as you can in summarizing your changes, as that makes them easier to review. There is no formal requirement on PR format, so don't sweat it too much. Just get some gainz and help make the lib kick ass.

If you're unsure about something or want early feedback before it's ready for a full PR, open the PR as a draft.

## Coding style

The project makes an effort to keep the coding style consistent throughout the codebase, but it isn't going to turn down some juicy optimizations over nit-picking little style inconsistencies. Falco can fix it up as part of the PR or a later commit.

## Doxygen comments

Everything that is part of the public API should be fully Doxygen commented. If this is too much of a PITA, if English isn't your first language, if you're allergic to writing, or if you simply don't have time, mention it in the PR and you'll be covered. Those header comments are also the source for the [API reference](/api/) on this site.

## Unit testing

The project is striving hard to hit 100% unit test coverage, but not everyone contributing is going to have time to fully unit test everything, which is fine. Make sure to run the currently implemented unit tests so you haven't broken anything that does have coverage. How: [Testing and Contributing](/guides/testing-and-contributing/).

## C++ bindings

The C++ bindings are kept in sync with the C API; however, SH4 and C contributors are not expected to provide idiomatic C++ bindings for their contributions. Falco will handle it.

## This documentation site

This site lives in `doc/site/` of the SH4ZAM repo, linked as "Site source" in the footer of every page. The API pages are generated from the SH4ZAM headers, so fixes to API descriptions belong in the header comments. Guides and concept pages are plain Markdown in `doc/site/src/content/docs/`; every handwritten page has an edit link at the bottom.

## Related

- [Testing and Contributing](/guides/testing-and-contributing/)
- [Community](/community/)
- [Changelog](/changelog/)
