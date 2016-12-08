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

Code-example
------------
Example for classical order: first options, second non-options,
```c
#include <parse_opts.h>
#include <stdlib.h>

struct MY_DATA
{
  // My data which shall be modified by the options...
  // If you'll avoid global variables.
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
      {
         .optFunction = optOneOfMyCallbackFunction,
         .hasArg      = NO_ARG,
         .shortOpt    = 'o',
         .longOpt     = "myoption",
         .helpText    = "bla bla bla :-)"
      },
      // Further options...
      OPTION_BLOCKLIST_END_MARKER
   };

   struct MY_DATA myData = { /* initialize */ };

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
      {
         .optFunction = optOneOfMyCallbackFunction,
         .hasArg      = NO_ARG,
         .shortOpt    = 'o',
         .longOpt     = "myoption",
         .helpText    = "bla bla bla :-)"
      },
      // Further options...
      OPTION_BLOCKLIST_END_MARKER
   };

   struct MY_DATA myData = { /* initialize */ };

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
