/*****************************************************************************/
/*                                                                           */
/*        Example program for using the command line option parser           */
/*                                                                           */
/*     Demonstration of using anonymous functions (lambda-functions)         +/
/*     in the option-list.                                                   */
/*     Handling of mixed order of option and non-option arguments.           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    cmd_opt_ex3.c                                                   */
/*! @author  Ulrich Becker                                                   */
/*! @date    20.12.2016                                                      */
/*! @see     parse_opts.h                                                    */
/*****************************************************************************/

/*
 * NOTE:
 * If you have been made and installed the shared-library via the makefile in
 * /src/makefile just compile this example by the following line:
 *
 * gcc -o e3 -lParseOptsC cmd_opt_ex3.c
 *
 * otherwise invoke the makefile in this directory.
 */

#include <parse_opts.h>
#include <stdlib.h>
#include <assert.h>

/*!----------------------------------------------------------------------------
 * @brief The program variables which shall be modified by the options in our
 *        Example.
 */
struct MY_DATA
{
   const char* logfileName;
   unsigned int flags;
   int integer;
};

/*!----------------------------------------------------------------------------
 * @brief Option callback function for printing the help of our example.
 */
static int optPrintHelp( struct BLOCK_FUNCTION_ARG_T* pArg )
{
   assert( pArg->optArg == NULL );

   printf( "Usage: %s [options] [argunents]\nOptions:\n", pArg->ppAgv[0] );
   printOptionList( stdout, pArg->pOptBlockList );
   exit( EXIT_SUCCESS );
   return 0;
}

/*!---------------------------------------------------------------------------
 * @brief Shared callback function for more then one options to demonstrate
 *        the usage of the option identifier OPTION_BLOCK_T::id.
 */
static int optSharedSetFalg( struct BLOCK_FUNCTION_ARG_T* pArg )
{
   assert( pArg->optArg == NULL );
   assert( pArg->pCurrentBlock->id < (sizeof( unsigned int ) * 8) );

   ((struct MY_DATA*)pArg->pUser)->flags |= (1 << pArg->pCurrentBlock->id);

   return 0;
}

/*!----------------------------------------------------------------------------
 * @brief Option callback function to demonstrate the usage of options with
 *        a mandatory argument.
 */
static int optReadInteger( struct BLOCK_FUNCTION_ARG_T* pArg )
{
   #define IS_DIGIT( d ) (((d) >= '0') && ((d) <= '9'))

   char* pEnd;

   assert( pArg->optArg != NULL );

   if( (*pArg->optArg != '-' && *pArg->optArg != '+' && !IS_DIGIT( *pArg->optArg )) ||
       ((*pArg->optArg == '-' || *pArg->optArg == '+') && !IS_DIGIT( pArg->optArg[1] ))
     )
   {
      fprintf( stderr, "%s: Argument no: %d option: \"", pArg->ppAgv[0], pArg->argvIndex );
      printOption( stderr, pArg->pCurrentBlock );
      fprintf( stderr, "\" expects a decimal number and not: \"%s\"\n",
               pArg->optArg );
      return -1;
   }

   ((struct MY_DATA*)pArg->pUser)->integer = strtol( pArg->optArg, &pEnd, 10 );

   if( *pEnd == '\0' )
      return 0;

   fprintf( stderr, "%s: Argument no: %d invalid format of integer number \"%s\" for option: \"",
            pArg->ppAgv[0], pArg->argvIndex, pArg->optArg );
   printOption( stderr, pArg->pCurrentBlock );
   fprintf( stderr, "\"\n" );
   return -1;
}

/*!============================================================================
 * @brief The main function of our example.
 */
int main( int argc, char** ppArgv )
{
   /*
    * Initializing of our option block list.
    * You can also initialize this list outside from this function
    * as global variable.
    */
   struct OPTION_BLOCK_T blockList[] =
   {
      {
         OPT_LAMBDA( pArg,
         {
            assert( pArg->optArg == NULL );

            printf( "Usage: %s [options] [argunents]\nOptions:\n", pArg->ppAgv[0] );
            printOptionList( stdout, pArg->pOptBlockList );
            exit( EXIT_SUCCESS );
         }),
         .shortOpt    = 'h',
         .longOpt     = "help",
         .helpText    = "Print this help and exit"
      },
      {
         OPT_LAMBDA( pArg,
         {
             if( pArg->optArg == NULL ) // No argument given?
                ((struct MY_DATA*)pArg->pUser)->logfileName = "/var/log/myDefaultLogfile";
             else
                ((struct MY_DATA*)pArg->pUser)->logfileName = pArg->optArg;
             return 0;
         }),
         .hasArg      = OPTIONAL_ARG,
         .shortOpt    = 'l',
         .longOpt     = "logfile",
         .helpText    = "Logfile. If set logging is enabled.\n"
                        "You can name a explicit logfile in PARAM"
      },
      {
         .optFunction = optSharedSetFalg,
         .hasArg      = NO_ARG,
         .shortOpt    = 'a',
         .id          = 0,
         .helpText    = "Set flag 'a'"
      },
      {
         .optFunction = optSharedSetFalg,
         .hasArg      = NO_ARG,
         .shortOpt    = 'b',
         .id          = 1,
         .helpText    = "Set flag 'b'"
      },
      {
         .optFunction = optSharedSetFalg,
         .hasArg      = NO_ARG,
         .shortOpt    = 'c',
         .id          = 2,
         .helpText    = "Set flag 'c'"
      },
      {
         .optFunction = optReadInteger,
         .hasArg      = REQUIRED_ARG,
         .shortOpt    = 'i',
         .longOpt     = "integer",
         .helpText    = "Read a integer number in PARAM"
      },
      OPTION_BLOCKLIST_END_MARKER /* Please don't forget this!!! */ 
   };

   /*
    * Pre-initializing of our program variables which shall
    * be modified by the options.
    */
   struct MY_DATA myData =
   {
      .logfileName = NULL,
      .flags = 0,
      .integer = 0
   };
   int i;
   printf("\n");
   /*
    * Parsing-loop of the whole commend line.
    */
   for( i = 1; i < argc; i++ )
   {
      i = parseCommandLineOptionsAt( i, argc, ppArgv, blockList, &myData );
      if( i < 0 )
         return EXIT_FAILURE;
      if( i < argc )
         printf( "Non option argument in ppArgv[%d]: \"%s\"\n", i, ppArgv[i] );
   }
/*
 * Of course it's also possible to use different block-lists by different
 * option-sets and/or different private-data pointer (here &myData) for
 * each loop-step depending on the index "i" or by using
 * a FSM (finite state machine).
 */

   if( myData.logfileName != NULL )
      printf( "Logfile will used: %s\n", myData.logfileName );
   else
      printf( "No logfile will used\n" );

   printf( "Flags:\n" );
   for( i = 0; i < 3; i++ )
      printf( "Flag %c is %s\n",
              'a' + i,
              ((myData.flags & (1 << i)) != 0)? "set" : "not set" );

   printf( "Integer value = %d\n\n", myData.integer );

   return EXIT_SUCCESS;
}
/*================================== EOF ====================================*/
