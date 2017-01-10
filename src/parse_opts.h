/*****************************************************************************/
/*                                                                           */
/*!        @brief Redundancy free command-line option parser                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    parse_opts.h                                                    */
/*! @see     parse_opts.c                                                    */
/*! @author  Ulrich Becker                                                   */
/*! @date    25.10.2016                                                      */
/*  Updates:                                                                 */
/*! @example cmd_opt_ex1.c Example for classical parsing.                    */
/*! @example cmd_opt_ex2.c Example for mixed parsing.                        */
/*! @example cmd_opt_ex3.c Example for using anonymous (lambda) functions as */
/*                         callback function.                                */
/*****************************************************************************/
/*
 * MIT License
 *
 * Copyright (c) 2016 Ulrich Becker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _PARSE_ARGS_H
#define _PARSE_ARGS_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Pre-declaration resolves the chicken-egg problem.
struct OPTION_BLOCK_T;

/*!
 * @brief Argument-type of the option callback function.
 */
struct BLOCK_FUNCTION_ARG_T
{
   const int argc;      //!<@brief Forwarding of the first argument of
                        //! your main() function.

   char* const* ppAgv;  //!<@brief Forwarding of the second argument of
                        //! your main() function.

   int argvIndex;       //!<@brief Current index value of the argument vector
                        //! ppArgv

   char const* optArg;  //!<@brief Pointer to a possible option-argument. \n
                        //! If no argument given, so this value becomes
                        //! initialized by NULL.

   struct OPTION_BLOCK_T* pOptBlockList; //!<@brief Forwarding of the third
                        //! argument of the parser parseCommandLineOptions(). \n
                        //! The start-pointer to your option block-list.

   const struct OPTION_BLOCK_T* pCurrentBlock; //!<@brief Pointer to the actual
                        //! option block which has invoked the corresponding
                        //! callback function.

   void* pUser;         //!<@brief Forwarding of the last argument pUser of
                        //! the parser parseCommandLineOptions() and
                        //! parseCommandLineOptionsAt(). \n
                        //! The "tunnel" of your private data. \n
                        //! In this way the avoiding of global variables
                        //! becomes possible.
};

/*!
 * @brief Signature of the option callback functions.
 * @param pArg Pointer to the info-block of type BLOCK_FUNCTION_ARG_T.
 * @retval ==0 Function was successful.
 * @retval <0  Function was not successful. \n
 *             In this case the parser terminates immediately
 *             its work by the return-value of this function.
 * @retval >0  Function was not successful. \n
 *             In this case the parser continues its work but
 *             the return value of the parser will be -1.
 */
typedef int (*OPT_BLOCK_F)( struct BLOCK_FUNCTION_ARG_T* pArg );

/*!
 * @brief Helper-macro for OPT_LAMBDA
 */
#define _LAMBDA_(L_) ({ L_ _;})

/*!
 * @brief Macro for implementing anonymous functions directly in the initializer
 *        of the option list.
 *
 * This "nice to have" macro is useful when the call-back function is very small,
 * e.g. for setting or resetting flags.
 * Example:
 * @code
 * struct OPTION_BLOCK_T blockList[] =
 * {
 *    {
 *       OPT_LAMBDA( pArg, { ((struct MY_DATA*)pArg->pUser)->verbose = true; return 0; }),
 *       .hasArg      = NO_ARG,
 *       .shortOpt    = 'v',
 *       .longOpt     = "verbose",
 *       .helpText    = "Be verbose"
 *    },
 *    // Further options...
 *    OPTION_BLOCKLIST_END_MARKER
 * };
 * @endcode
 * @param argName Name of the argument variable which is
 *                the pointer to the info-block of type struct BLOCK_FUNCTION_ARG_T*.
 * @param body    Function body in {}. @see OPT_BLOCK_F
 */
#define OPT_LAMBDA( argName, body ) \
   .optFunction = _LAMBDA_( int _( struct BLOCK_FUNCTION_ARG_T* argName ) body )

/*!
 * @brief Definition of the option-type.
 */
typedef enum
{
   NO_ARG       = 0, //!<@brief Option has no argument.

   REQUIRED_ARG = 1, //!<@brief Option needs a argument.

   OPTIONAL_ARG = 2  //!<@brief Option can have a argument but must not. \n
                     //!<@note  If a argument for this option type is given, \n
                     //! so the usage of the '=' character in the command line
                     //! becomes necessary.
} ARG_REQUIRE_T;

/*!
 * @brief Definition of the option-block.
 */
struct OPTION_BLOCK_T
{
   OPT_BLOCK_F   optFunction; //!<@brief Mandatory pointer to your corresponding callback-function.

   ARG_REQUIRE_T hasArg;      //!<@brief Determines whether the option needs a additional argument. \n
                              //!        If this is omitted in your initializer,
                              //!        no argument will assumed like NO_ARG.

   const int     id;          //!<@brief Optional option identifier to distinguish options
                              //!        with the same callback-function. \n 
                              //!       (Nice to have.)

   const char    shortOpt;    //!<@brief Character for your short-option "-o". \n
                              //!        Mandatory when longOpt is omitted.

   const char*   longOpt;     //!<@brief String for your long-option "--option". \n
                              //!        Mandatory when shortOpt is omitted.

   const char*   helpText;    //!<@brief Additional help-text for your option. \n
                              //!        You can omit this in your initializer,
                              //!        but that's not a good idea. ;-)
};

/*!
 * @brief Data type can be used as iterator-type for the loop-macro
 *        FOR_EACH_OPTION_BLOCK
 */
typedef  struct OPTION_BLOCK_T* BLK_LIST_ITERATOR_T;

/*!
 * @brief Helper loop-macro for browsing your whole option-block list.
 * @see OPTION_BLOCK_T
 * @param i Pointer to the current option-block within the loop "iterator"
 * @param l Pointer of your option-block list.
 * @note Don't forget to put the macro OPTION_BLOCKLIST_END_MARKER in your
 *       Block-list as last element.
 */
#define FOR_EACH_OPTION_BLOCK( i, l ) \
   for( i = l; i->optFunction != NULL; i++ )

/*!
 * @brief End-marker for your block-list initializer.
 *
 * This is absolutely mandatory to put this as last element
 * in your block-list initializer. \n
 * In this manner the parser recognized the end of your
 * declared block-list.
 *
 * @note Don't forget it!
 */
#define OPTION_BLOCKLIST_END_MARKER { .optFunction = NULL }

/*!
 * @brief The option command line parser for the mixed order of
 *        non-option arguments and option arguments.
 *
 * Once the parser has recognized a (long or short) option defined in your
 * option block-list, the corresponding callback function becomes invoked. \n
 * If a option in the command line is not defined in your option block list,
 * so a corresponding error-message will written in the standard error pipe (stderr),
 * and the parser returns by -1.
 *
 * Example:
 * @code
 * #include <parse_opts.h>
 * #include <stdlib.h>
 * ...
 * struct MY_DATA
 * {
 *   // My data which shall be modified by the options...
 *   // If you'll avoid global variables.
 * };
 *
 * static int optOneOfMyCallbackFunction( struct BLOCK_FUNCTION_ARG_T* pArg )
 * {
 *    // Callback function for option '-o' and "--myoption"
 *    // Do something...
 *    return 0;
 * }
 *
 * int main( int argc, char** ppArgv )
 * {
 *    int i;
 *    struct OPTION_BLOCK_T blockList[] =
 *    {
 *       {
 *          .optFunction = optOneOfMyCallbackFunction,
 *          .hasArg      = NO_ARG,
 *          .shortOpt    = 'o',
 *          .longOpt     = "myoption",
 *          .helpText    = "bla bla bla :-)"
 *       },
 *       // Further options...
 *       OPTION_BLOCKLIST_END_MARKER
 *    };
 *
 *    struct MY_DATA myData = { // initialize };
 *
 *    for( i = 1; i < argc; i++ )
 *    {
 *       i = parseCommandLineOptionsAt( i, argc, ppArgv, blockList, &myData );
 *       if( i < 0 )
 *          return EXIT_FAILURE;
 *       if( i < argc )
 *          printf( "Non option argument in ppArgv[%d]: \"%s\"\n", i, ppArgv[i] );
 *    }
 *
 *    // Do something others...
 *
 *    return EXIT_SUCCESS;
 * }
 * @endcode
 *
 * Of course it's also possible to use different block-lists by different
 * option sets and/or different private-data pointer (here &myData) for
 * each loop-step depending on the index "i" or by using
 * a FSM (finite state machine).
 *
 * @param offset Start-offset, initializer of the index for ppArgv[].
 * @param argc The first argument of your main() function.
 * @param ppArgv Argument vector the second argument of your main() function.
 * @param optBlockList Pointer to your defined option-block-list.
 * @param pUser Optional pointer to your data structure.
 *              This will forwarded in the element pUser of
 *              the argument-block of BLOCK_FUNCTION_ARG_T for the
 *              callback functions. In this way the callback function
 *              can access to your program data without the using of
 *              global variables.
 * @retval <0 Parsing was not successful.
 * @retval >0 Index-value to the first non-option argument after
 *            possible options if present.
 */
int parseCommandLineOptionsAt( int offset,
                               int argc,
                               char* const ppAgv[],
                               struct OPTION_BLOCK_T optBlockList[],
                               void* pUser
                             );

/*!
 * @brief The option command line parser for the classical order: \n
 *        first: option-arguments; second: non-option arguments.
 *
 * Once the parser has recognized a (long or short) option defined in your
 * option block-list, the corresponding callback function becomes invoked. \n
 * If a option in the command line is not defined in your option block list,
 * so a corresponding error-message will written in the standard error pipe (stderr),
 * and the parser returns by -1.
 *
 * Example:
 * @code
 * #include <parse_opts.h>
 * #include <stdlib.h>
 *
 * struct MY_DATA
 * {
 *   // My data which shall be modified by the options...
 *   // If you'll avoid global variables.
 * };
 *
 * static int optOneOfMyCallbackFunction( struct BLOCK_FUNCTION_ARG_T* pArg )
 * {
 *    // Callback function for option '-o' and "--myoption"
 *    // Do something...
 *    return 0;
 * }
 *
 * int main( int argc, char** ppArgv )
 * {
 *    struct OPTION_BLOCK_T blockList[] =
 *    {
 *       {
 *          .optFunction = optOneOfMyCallbackFunction,
 *          .hasArg      = NO_ARG,
 *          .shortOpt    = 'o',
 *          .longOpt     = "myoption",
 *          .helpText    = "bla bla bla :-)"
 *       },
 *       // Further options...
 *       OPTION_BLOCKLIST_END_MARKER
 *    };
 *
 *    struct MY_DATA myData = { // initialize };
 *
 *    int i = parseCommandLineOptions( argc, ppArgv, blockList, &myData );
 *    if( i < 0 )
 *       return EXIT_FAILURE;
 *    for(; i < argc; i++ )
 *    {
 *       printf( "Non option argument: %s\n", ppArgv[i] );
 *    }
 *
 *    // Do something others...
 *
 *    return EXIT_SUCCESS;
 * }
 * @endcode
 *
 * @param argc The first argument of your main() function.
 * @param ppArgv Argument vector the second argument of your main() function.
 * @param optBlockList Pointer to your defined option-block-list.
 * @param pUser Optional pointer to your data structure.
 *              This will forwarded in the element pUser of
 *              the argument-block of BLOCK_FUNCTION_ARG_T for the
 *              callback functions. In this way the callback function
 *              can access to your program data without the using of
 *              global variables.
 * @retval <0 Parsing was not successful.
 * @retval >0 Index-value to the first non-option argument if present.
 */
static inline int parseCommandLineOptions( int argc,
                                           char* const ppAgv[],
                                           struct OPTION_BLOCK_T optBlockList[],
                                           void* pUser
                                         )
{
   return parseCommandLineOptionsAt( 1, argc, ppAgv, optBlockList, pUser );
}

/*!
 * @brief Prints the schort option (if present) and long option
 *        (if present) of a option-block in the file pStream.
 *
 * Helper-function can be used to simplify e.g. your print-help-function
 * and/or error messages in your callback-functions. \n
 * Example:
 * @code
 * static int optOneOfMyCallbackFunctions( struct BLOCK_FUNCTION_ARG_T* pArg )
 * {
 *   //... do something ...
 *   if( something_was_wrong )
 *   {
 *      fprintf( stderr, "Error in option: \"" );
 *      printOption( stderr, pArg->pCurrentBlock );
 *      fprintf( stderr "\" bla bla bla :-(\n" );
 *      return -1;
 *   }
 *   return 0;
 * }
 * @endcode
 * @param pStream Output-file e.g. stdout or stderr.
 * @param pOptionBlock Pointer to the option-block.
 */
void printOption( FILE* pStream, const struct OPTION_BLOCK_T* pOptionBlock );

/*!
 * @brief Prints the formatted content of each element of your option-block-list
 *        in the file pStream.
 *
 * Helper-function can be used to simplify your print-help-function. \n
 * Example:
 * @code
 * static int optPrintMyHelp( struct BLOCK_FUNCTION_ARG_T* pArg )
 * {
 *    printf( "Bla bla bla ...\n" );
 *    printf( "Usage: %s [options] bla bla bla...\n". pArg->ppAgv[0] );
 *    printf( "Options:\n" );
 *    printOptionList( stdout, pArg->pOptBlockList );
 *    exit( EXIT_SUCCESS );
 *    return 0;
 * }
 * @endcode
 * @param pStream Output-file e.g. stdout or stderr.
 * @param optBlockList Start-pointer to your option-block-list.
 */
void printOptionList( FILE* pStream, struct OPTION_BLOCK_T optBlockList[] );

#ifdef __cplusplus
}
#endif
#endif /* ifndef _PARSE_ARGS_H */
/*================================== EOF ====================================*/
