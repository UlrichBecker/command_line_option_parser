# Command-line-option-parser
A command line option parser for C/C++ projects.
An alternative to getopt() and getopt_long() by using of callback functions in the shape of
classical function-pointers or as anonymous "lambda" functions in C.

# Advantages:

- Avoiding of global variables like "optind" or "optarg".
- Using of a callback function for each option instead of (confusing huge) switch-case statements. (Avoiding of spaghetti-code.)
- Use of anonymous callback functions (lambda functions) possible.
  That means implementation of the function body directly in the option-list initializer possible.
- Object oriented flavor: Short option-name (if given), long option-name (if given) and help-text (if given)
  are bound together in a single object by a corresponding callback function.
- Avoiding of redundance: If you'll change a option name, so you have to do this on one place in your source-code only.
- Commands by mixed order of options and non-options are possible (see example e2).
- Scalable, if a serten kind of option-type won't used for your project just define the corresponding compiler-switch (CONFIG_CLOP_NO_...) in your Makefile respectively build-system to reduce memory, provided you'll link the source-code directly to your project rather than the shared-library. 

If you intend to write your project in C++11 just take a look in the following link:

https://github.com/UlrichBecker/command_line_option_parser_cpp11

Excuses my Eric Allman coding-style and my bad English ;-)

Code-example
------------
You can also find a practical example of using this module in this repository: 
https://github.com/UlrichBecker/ioctl4bash

Example for classical order: first options, second non-options,
```c
#include <parse_opts.h>
#include <stdlib.h>

struct MY_DATA
{
  // My data which shall be modified by the options...
  // If you'll avoid global variables.
  bool verbose;
  //...
};

static int optOneOfMyCallbackFunction( struct BLOCK_FUNCTION_ARG_T* pArg )
{
   // Callback function for option '-o' and "--myoption"
   // Do something...
   return 0;
}

int main( int argc, char** ppArgv )
{
   struct OPTION_BLOCK_T blockList[] =
   {
      { // Using of classical function-pointer as callback function.
         .optFunction = optOneOfMyCallbackFunction,
         .hasArg      = NO_ARG,
         .shortOpt    = 'o',
         .longOpt     = "myoption",
         .helpText    = "bla bla bla :-)"
      },
      { // Using of anonymous callback function which can be implemented directly in the initializer.
         OPT_LAMBDA( pArg, { ((struct MY_DATA*)pArg->pUser)->verbose = true; return 0; }),
         .hasArg      = NO_ARG,
         .shortOpt    = 'v',
         .longOpt     = "verbose",
         .helpText    = "Be verbose"
      },
      // Further options...
      OPTION_BLOCKLIST_END_MARKER
   };

   struct MY_DATA myData = { .verbose = false; };

   int i = parseCommandLineOptions( argc, ppArgv, blockList, &myData );
   if( i < 0 )
      return EXIT_FAILURE;
   for(; i < argc; i++ )
   {
      printf( "Non option argument: %s\n", ppArgv[i] );
   }

   // Do something others...

   return EXIT_SUCCESS;
}
```

Example for mixed order:

```c
#include <parse_opts.h>
#include <stdlib.h>

struct MY_DATA
{
  // My data which shall be modified by the options...
  // If you'll avoid global variables.
  bool verbose;
};

static int optOneOfMyCallbackFunction( struct BLOCK_FUNCTION_ARG_T* pArg )
{
   // Callback function for option '-o' and "--myoption"
   // Do something...
   return 0;
}

int main( int argc, char** ppArgv )
{
   int i;
   struct OPTION_BLOCK_T blockList[] =
   {
      { // Using of classical function-pointer as callback function.
         .optFunction = optOneOfMyCallbackFunction,
         .hasArg      = NO_ARG,
         .shortOpt    = 'o',
         .longOpt     = "myoption",
         .helpText    = "bla bla bla :-)"
      },
      { // Using of anonymous callback function which can be implemented directly in the initializer.
         OPT_LAMBDA( pArg, { ((struct MY_DATA*)pArg->pUser)->verbose = true; return 0; }),
         .hasArg      = NO_ARG,
         .shortOpt    = 'v',
         .longOpt     = "verbose",
         .helpText    = "Be verbose"
      },
      // Further options...
      OPTION_BLOCKLIST_END_MARKER
   };

   struct MY_DATA myData = { .verbose = false; };

   for( i = 1; i < argc; i++ )
   {
      i = parseCommandLineOptionsAt( i, argc, ppArgv, blockList, &myData );
      if( i < 0 )
         return EXIT_FAILURE;
      if( i < argc )
         printf( "Non option argument in ppArgv[%d]: \"%s\"\n", i, ppArgv[i] );
   }

   // Do something others...

   return EXIT_SUCCESS;
}
```


## CMake integration

```cmake
include(FetchContent)
FetchContent_Declare(
    command_line_option_parser
    GIT_REPOSITORY https://github.com/UlrichBecker/command_line_option_parser.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    FIND_PACKAGE_ARGS NAMES command_line_option_parser)
FetchContent_MakeAvailable(command_line_option_parser)

target_link_libraries(app command_line_option_parser::command_line_option_parser)

```