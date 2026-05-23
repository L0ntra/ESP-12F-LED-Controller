# Guidelines — General

Rules that apply regardless of language or task scope.

## Tests

- **Small unit tests over integration/roundtrip tests.** Each test exercises one
  scenario with one or two explicit assertions. Do not write tests that bundle
  multiple operations into a roundtrip check.
- **If a function's only callers are tests, remove both.** Tests must not be the
  sole reason a function survives in the codebase.
- **Prefer established, community-maintained testing libraries** over custom
  test frameworks or assertion macros.

## Process

- **Systematic passes.** When searching for dead code, unused imports, or other
  cross-cutting concerns, examine every relevant file before removing anything,
  then re-check after each removal for cascading dead items.
- **Show evidence.** When asked "why is X needed?", cite the specific file, line
  number, and call site — not a general explanation.
- **Take action.** Analysis is expected, but the default is to implement the
  change unless told otherwise.
- **Clean commits.** Descriptive commit messages, logical grouping, no
  half-done work pushed.
