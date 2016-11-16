# Command-line-option-parser
A command line option parser for C/C++ projects.
An alternative to getopt() and getopt_long() by using of callback functions.

# Advantages:

- Avoiding of global variables like "optind" or "optarg".
- Using of a callback function for each option instead of (confusing huge) switch-case statements. (Avoiding of spaghetti-code.)
- Object oriented flavor: Short option-name (if given), long option-name (if given) and help-text (if given)
  are bound together in a single object by a corresponding callback function.
- Avoiding of redundance: If you'll change a option name, so you have to do this on one place in your source-code only.
- Commands by mixed order of options and non-options are possible (see example e2).

Excuses my coding-style and my bad english ;-)
