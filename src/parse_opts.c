/*****************************************************************************/
/*                                                                           */
/*!             @brief Redundancy free command-line option parser            */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    parse_opts.c                                                    */
/*! @see     parse_opts.h                                                    */
/*! @author  Ulrich Becker                                                   */
/*! @date    25.10.2016                                                      */
/*  Updates:                                                                 */
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

#include <parse_opts.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#if defined( CONFIG_CLOP_NO_NO_ARG )          \
    && defined( CONFIG_CLOP_NO_REQUIRED_ARG ) \
    && defined( CONFIG_CLOP_NO_OPTIONAL_ARG )
 #error Not allowed to define all three macros!
#endif

#ifndef CONFIG_CLOP_NO_OPTIONAL_ARG
/*-----------------------------------------------------------------------------
*/
static void longOptionalOptionErrorMessage( const char* prgName,
                                            const char* optionName )
{
   fprintf( stderr, "%s: missing argument after '=' of long option --%s\n",
            prgName, optionName );
}

/*-----------------------------------------------------------------------------
*/
static void shortOptionalOptionErrorMessage( const char* prgName,
                                             char optionName )
{
   fprintf( stderr, "%s: missing argument after '=' of short option -%c\n",
            prgName, optionName );
}
#endif /* ifndef CONFIG_CLOP_NO_OPTIONAL_ARG */

/*!----------------------------------------------------------------------------
@see parse_opts.h
*/

#define _RETURN_HANDLING( f ) \
   ret = (f);                 \
   if( ret < 0 )              \
      return ret;             \
   if( ret > 0 )              \
      error = true;           \

int parseCommandLineOptionsAt( int offset,
                               int argc,
                               char* const ppAgv[],
                               struct OPTION_BLOCK_T optBlockList[],
                               void* pUser
                             )
{
   struct BLOCK_FUNCTION_ARG_T arg =
   {
      .argc          = argc,
      .ppAgv         = ppAgv,
      .pOptBlockList = optBlockList,
      .pUser         = pUser
   };

   bool error = false;
   int ret;
   size_t tl;
   char* pCurrent;

   for( arg.argvIndex = offset; arg.argvIndex < argc; arg.argvIndex++ )
   {
      if( ppAgv[arg.argvIndex][0] != '-' )
         break; /* No (further) option present. */

      pCurrent = &ppAgv[arg.argvIndex][1];
      if( *pCurrent == '\0' )
      {
         fprintf( stderr, "%s: missing option -?\n", ppAgv[0] );
         return -1;
      }

#ifndef CONFIG_NO_NEGATIVE_NUMBERS_IN_NON_OPTION_ARGUMENTS
      /* In the case the first non-option argument is a negative number. */
      if( (*pCurrent >= '0') && (*pCurrent <= '9') )
         break; /* Argument is not a option but a negative number. */
#endif

      arg.optArg = NULL;

      if( *pCurrent == '-' ) /* Long option? */
      { /* Yes */
         pCurrent++;
         if( *pCurrent == '\0' )
         {
            fprintf( stderr, "%s: missing long option --???\n", ppAgv[0] );
            return -1;
         }

         tl = 0;
         while( (pCurrent[tl] != '\0') && (pCurrent[tl] != '=') )
            tl++;

         FOR_EACH_OPTION_BLOCK( arg.pCurrentBlock, optBlockList )
         {
            if( arg.pCurrentBlock->longOpt == NULL )
            {  /* In this case at least shortOpt must be defined! */
               assert( arg.pCurrentBlock->shortOpt != 0 );
               continue;
            }
            if( tl != strlen( arg.pCurrentBlock->longOpt ) )
               continue;
            if( strncmp( arg.pCurrentBlock->longOpt, pCurrent, tl ) != 0 )
               continue;
            break;
         }
         if( arg.pCurrentBlock->optFunction == NULL )
         {
            error = true;
            fprintf( stderr, "%s: unrecognized long option --%s\n",
                     ppAgv[0], pCurrent );
            continue; /* Of: for( arg.argvIndex = 1; arg.argvIndex < argc; arg.argvIndex++ ) */
         }

         switch( arg.pCurrentBlock->hasArg )
         {
         #ifndef CONFIG_CLOP_NO_NO_ARG
            case NO_ARG:
            {
               _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
               break;
            }
         #endif /* ifndef CONFIG_CLOP_NO_NO_ARG */
         #ifndef CONFIG_CLOP_NO_REQUIRED_ARG
            case REQUIRED_ARG:
            {
               if( (arg.argvIndex+1) == argc )
               {
                  fprintf( stderr,
                           "%s: missing argument of long option --%s\n",
                           ppAgv[0], arg.pCurrentBlock->longOpt );
                  return -1;
               }
               arg.argvIndex++;
               arg.optArg = ppAgv[arg.argvIndex];
               _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
               break;
            }
         #endif /* ifndef CONFIG_CLOP_NO_REQUIRED_ARG */
         #ifndef CONFIG_CLOP_NO_OPTIONAL_ARG
            case OPTIONAL_ARG:
            {
               if( pCurrent[tl] == '\0' )
               {
                  if( ((arg.argvIndex+1) == argc) || (ppAgv[arg.argvIndex+1][0] != '=') )
                  {  /* No argument */
                     _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
                     break;
                  }
                  if( ppAgv[arg.argvIndex+1][0] == '=' )
                  {
                     arg.argvIndex++;
                     if( ppAgv[arg.argvIndex][1] != '\0' )
                     {  /* "--OPTION =ARGUMENT" */
                        arg.optArg = &ppAgv[arg.argvIndex][1];
                        _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
                        break;
                     }
                     if( (arg.argvIndex+1) == argc )
                     {
                        longOptionalOptionErrorMessage( ppAgv[0],
                                                        arg.pCurrentBlock->longOpt );
                        return -1;
                     }
                     /* "--OPTION = ARGUMENT" */
                     arg.argvIndex++;
                     arg.optArg = ppAgv[arg.argvIndex];
                     _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
                  }
                  break;
               } /* if( pCurrent[tl] == '\0' ) */
               if( pCurrent[tl+1] != '\0' )
               {  /* "--OPTION=ARGUMENT" */
                  arg.optArg = &pCurrent[tl+1];
                  _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
                  break;
               }
               if( (arg.argvIndex+1) == argc )
               {
                  longOptionalOptionErrorMessage( ppAgv[0],
                                                  arg.pCurrentBlock->longOpt );
                  return -1;
               }
               /* "--OPTION= ARGUMENT" */
               arg.argvIndex++;
               arg.optArg = ppAgv[arg.argvIndex];
               _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
               break;
            } /* End of case OPTIONAL_ARG: */
         #endif /* ifndef CONFIG_CLOP_NO_OPTIONAL_ARG */
            default: assert( false ); break;
         } /* End of switch( arg.pCurrentBlock->hasArg ) */
         continue; /* for( i = arg.argvIndex; arg.argvIndex < argc; arg.argvIndex++ ) */
      } /* if( *pCurrent == '-' ) End of long-option parsing. */

      while( *pCurrent != '\0' ) /* short option */
      {
         FOR_EACH_OPTION_BLOCK( arg.pCurrentBlock, optBlockList )
         {
            if( arg.pCurrentBlock->shortOpt == 0 )
            {  /*In this case at least longOpt musr be defined! */
               assert( arg.pCurrentBlock->longOpt != NULL );
               continue;
            }
            if( arg.pCurrentBlock->shortOpt != *pCurrent )
               continue;
            break;
         }
         if( arg.pCurrentBlock->optFunction == NULL )
         {
            error = true;
            fprintf( stderr,
                     "%s: unrecognized option -%c\n",
                     ppAgv[0], *pCurrent );
            pCurrent++;
            continue;
         }

         switch( arg.pCurrentBlock->hasArg )
         {
         #ifndef CONFIG_CLOP_NO_NO_ARG
            case NO_ARG:
            {
               _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
               break;
            }
         #endif    
         #ifndef CONFIG_CLOP_NO_REQUIRED_ARG
            case REQUIRED_ARG:
            {
               if( (pCurrent[1] == '\0') && ((arg.argvIndex+1) == argc) )
               {
                  fprintf( stderr,
                           "%s: missing argument for option \'%c\'\n",
                           ppAgv[0], *pCurrent );
                  return -1;
               }
               if( pCurrent[1] != '\0' )
               {
                  arg.optArg = &pCurrent[1];
                  _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )

                  do
                     pCurrent++;
                  while( pCurrent[1] != '\0' );
               }
               else
               {
                  arg.argvIndex++;
                  arg.optArg = ppAgv[arg.argvIndex];
                  _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )
               }
               break;
            }
         #endif /* ifndef CONFIG_CLOP_NO_REQUIRED_ARG */
         #ifndef CONFIG_CLOP_NO_OPTIONAL_ARG
            case OPTIONAL_ARG:
            {
               if( pCurrent[1] == '=' )
               {
                  pCurrent++;
                  if( pCurrent[1] != '\0' )
                     arg.optArg = &pCurrent[1]; /* "-O=ARGUMENT" */
                  else if( (arg.argvIndex+1) < argc )
                  {
                     arg.argvIndex++; /* "-O= ARGUMENT" */
                     arg.optArg = ppAgv[arg.argvIndex];
                  }
                  else
                  {
                     shortOptionalOptionErrorMessage( ppAgv[0],
                                                      arg.pCurrentBlock->shortOpt );
                     return -1;
                  }
               }
               else if( (pCurrent[1] == '\0') && ((arg.argvIndex+1) < argc) )
               {
                  if( ppAgv[arg.argvIndex+1][0] == '=' )
                  {
                     arg.argvIndex++;
                     if( ppAgv[arg.argvIndex][1] != '\0' )
                        arg.optArg = &ppAgv[arg.argvIndex][1]; /* "-O =ARGUMENT" */
                     else
                     {
                        if( (arg.argvIndex+1) < argc )
                        {  /* "-O = ARGUMENT" */
                           arg.argvIndex++;
                           arg.optArg = ppAgv[arg.argvIndex];
                        }
                        else
                        {
                           shortOptionalOptionErrorMessage( ppAgv[0],
                                                            arg.pCurrentBlock->shortOpt );
                           return -1;
                        }
                     }
                  }
               }

               _RETURN_HANDLING( arg.pCurrentBlock->optFunction( &arg ) )

               if( arg.optArg == NULL )
                  break;

               while( pCurrent[1] != '\0' )
                  pCurrent++;
               break;
            } /* End of case OPTIONAL_ARG */
         #endif /* ifndef CONFIG_CLOP_NO_OPTIONAL_ARG */
            default: assert( false ); break;
         } /* End of switch( arg.pCurrentBlock->hasArg ) */

         pCurrent++;
      } /* while( *pCurrent != '\0' ) */
   } /* for( arg.argvIndex = 1; arg.argvIndex < argc; arg.argvIndex++ ) */
   return error? -1 : arg.argvIndex;
}

/*!-----------------------------------------------------------------------------
@see parse_opts.h
*/
void printOption( FILE* pStream, const struct OPTION_BLOCK_T* pOptionBlock )
{
   const char* pParam = "";
   assert( (pOptionBlock->shortOpt != 0) || (pOptionBlock->longOpt != NULL) );

   switch( pOptionBlock->hasArg )
   {
   #ifndef CONFIG_CLOP_NO_NO_ARG
      case NO_ARG:                             break;
   #endif
   #ifndef CONFIG_CLOP_NO_REQUIRED_ARG
      case REQUIRED_ARG: pParam = " PARAM";    break;
   #endif
   #ifndef CONFIG_CLOP_NO_OPTIONAL_ARG
      case OPTIONAL_ARG: pParam = " [=PARAM]"; break;
   #endif
      default: assert( false ); break;
   }

   if( pOptionBlock->shortOpt != 0 )
   {
      fprintf( pStream, "-%c%s", pOptionBlock->shortOpt, pParam );
      if( pOptionBlock->longOpt != NULL )
         fprintf( pStream, ", " );
   }
   if( pOptionBlock->longOpt != NULL )
      fprintf( pStream, "--%s%s", pOptionBlock->longOpt, pParam );
}

/*!----------------------------------------------------------------------------
@see parse_opts.h
*/
void printOptionList( FILE* pStream, struct OPTION_BLOCK_T optBlockList[] )
{
   const char* pC;
   BLK_LIST_ITERATOR_T pBlock;

   FOR_EACH_OPTION_BLOCK( pBlock, optBlockList )
   {
      assert( (pBlock->shortOpt != 0) || (pBlock->longOpt != NULL) );
      fprintf( pStream, "  " );
      printOption( pStream, pBlock );
      fprintf( pStream, "\n\t" );
      if( pBlock->helpText != NULL )
      {
         pC = pBlock->helpText;
         while( *pC != '\0' )
         {
            if( *pC == '\n' )
               fprintf( pStream, "\n\t" );
            else
               fprintf( pStream, "%c", *pC );
            pC++;
         }
      }
      fprintf( pStream, "\n\n" );
   }
}

/*================================== EOF ====================================*/
