/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   cubeparserlex

/* First part of user declarations.  */

/* Line 279 of lalr1.cc  */
#line 40 "../../build-backend/../src/cube/src/syntax/Cube4Parser.cpp"


#include "Cube4Parser.h"

/* User implementation prologue.  */
/* Line 285 of lalr1.cc  */
#line 240 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"




#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeSystemTreeNode.h"
#include "CubeMetric.h"
#include "CubeCnode.h"
#include "CubeRegion.h"

#include "CubeDriver.h"
#include "Cube4Scanner.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <string>


#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <string>
#include <vector>
#include "CubeServices.h"
#include "CubeParseContext.h"
#include "CubeMetric.h"
#include "Cube.h"
#include "CubeCartesian.h"

using namespace std;

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex Cube4Lexer.lex

// Workaround for Sun Studio C++ compilers on Solaris
#if defined(__SVR4) &&  defined(__SUNPRO_CC)
  #include <ieeefp.h>

  #define isinf(x)  (fpclass(x) == FP_NINF || fpclass(x) == FP_PINF)
  #define isnan(x)  isnand(x)
#endif


cubeparser::Cube4Parser::location_type cubeparserloc;



/* Line 285 of lalr1.cc  */
#line 104 "../../build-backend/../src/cube/src/syntax/Cube4Parser.cpp"


# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if CUBEPARSERDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !CUBEPARSERDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location) YYUSE(Type)
# define YY_REDUCE_PRINT(Rule)        static_cast<void>(0)
# define YY_STACK_PRINT()             static_cast<void>(0)

#endif /* !CUBEPARSERDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace cubeparser {
/* Line 353 of lalr1.cc  */
#line 199 "../../build-backend/../src/cube/src/syntax/Cube4Parser.cpp"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Cube4Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Cube4Parser::Cube4Parser (class Driver& driver_yyarg, class ParseContext& parseContext_yyarg, class Cube4Scanner& Cube4Lexer_yyarg, class cube::Cube& cube_yyarg, bool& clustering_on_yyarg)
    :
#if CUBEPARSERDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg),
      parseContext (parseContext_yyarg),
      Cube4Lexer (Cube4Lexer_yyarg),
      cube (cube_yyarg),
      clustering_on (clustering_on_yyarg)
  {
  }

  Cube4Parser::~Cube4Parser ()
  {
  }

#if CUBEPARSERDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Cube4Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    std::ostream& yyo = debug_stream ();
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Cube4Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Cube4Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Cube4Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if CUBEPARSERDEBUG
  std::ostream&
  Cube4Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Cube4Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Cube4Parser::debug_level_type
  Cube4Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Cube4Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  Cube4Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Cube4Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Cube4Parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    // State.
    int yyn;
    int yylen = 0;
    int yystate = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    static semantic_type yyval_default;
    semantic_type yylval = yyval_default;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


/* User initialization code.  */
/* Line 545 of lalr1.cc  */
#line 75 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
{
    // initialize the initial location object
    yylloc.begin.filename = yylloc.end.filename = &driver.streamname;
    clustering_on = false;
}
/* Line 545 of lalr1.cc  */
#line 405 "../../build-backend/../src/cube/src/syntax/Cube4Parser.cpp"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
        YYCDEBUG << "Reading a token: ";
        yychar = yylex (&yylval, &yylloc);
      }

    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    // Compute the default @$.
    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
          case 2:
/* Line 670 of lalr1.cc  */
#line 305 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.versionSeen)
	  error(cubeparserloc,"Multiple version attributes defines!"); 
	else parseContext.versionSeen = true;
	parseContext.version = parseContext.str.str(); 
}
    break;

  case 3:
/* Line 670 of lalr1.cc  */
#line 312 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.encodingSeen)
	  error(cubeparserloc,"Multiple encoding attributes defines!"); 
	else parseContext.encodingSeen = true;
	parseContext.encoding = parseContext.str.str(); 
}
    break;

  case 4:
/* Line 670 of lalr1.cc  */
#line 319 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.keySeen)
	  error(cubeparserloc,"Multiple key attributes defines!"); 
	else parseContext.keySeen = true;
	parseContext.key = parseContext.str.str(); 
}
    break;

  case 5:
/* Line 670 of lalr1.cc  */
#line 326 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.valueSeen)
	  error(cubeparserloc,"Multiple value attributes defines!"); 
	else parseContext.valueSeen = true;	
	parseContext.value = parseContext.str.str(); 
}
    break;

  case 6:
/* Line 670 of lalr1.cc  */
#line 333 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
	if (parseContext.modSeen)
	  error(cubeparserloc,"Multiple module attributes defines!"); 
	else parseContext.modSeen = true;
	parseContext.mod = parseContext.str.str(); }
    break;

  case 7:
/* Line 670 of lalr1.cc  */
#line 340 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.metricTypeSeen)
      error(cubeparserloc,"Multiple metric type attributes defines!"); 
    else parseContext.metricTypeSeen = true;
    parseContext.metricType = parseContext.str.str(); }
    break;

  case 8:
/* Line 670 of lalr1.cc  */
#line 348 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.metricVizTypeSeen)
      error(cubeparserloc,"Multiple metric visibilty type attributes defines!"); 
    else parseContext.metricVizTypeSeen = true;
    parseContext.metricVizType = parseContext.str.str(); }
    break;

  case 9:
/* Line 670 of lalr1.cc  */
#line 356 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.cnodeParTypeSeen)
      error(cubeparserloc,"Multiple cnode parameter type attributes defines!"); 
    else parseContext.cnodeParTypeSeen = true;
    parseContext.cnode_parameter_type = parseContext.str.str(); 
}
    break;

  case 10:
/* Line 670 of lalr1.cc  */
#line 365 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.cnodeParKeySeen)
      error(cubeparserloc,"Multiple cnode parameter keys defines!"); 
    else parseContext.cnodeParKeySeen = true;
    parseContext.cnode_parameter_key = parseContext.str.str(); 
}
    break;

  case 11:
/* Line 670 of lalr1.cc  */
#line 374 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.cnodeParValueSeen)
      error(cubeparserloc,"Multiple cnode parameter values defines!"); 
    else parseContext.cnodeParValueSeen = true;
    parseContext.cnode_parameter_value = parseContext.str.str(); 
}
    break;

  case 12:
/* Line 670 of lalr1.cc  */
#line 383 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.cartNameSeen)
      error(cubeparserloc,"Multiple topology names  defines!"); 
    else parseContext.cartNameSeen = true;
    parseContext.cartName = parseContext.str.str(); 
}
    break;

  case 13:
/* Line 670 of lalr1.cc  */
#line 393 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.dimNameSeen) 
	  error(cubeparserloc,"Multiple names for dimension attributes defines!"); 
	else parseContext.dimNameSeen = true;
	parseContext.dimNamesCount++;
	parseContext.dimName = cube::services::escapeFromXML(parseContext.str.str()); 
}
    break;

  case 15:
/* Line 670 of lalr1.cc  */
#line 403 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.metricsTitleSeen)
      error(cubeparserloc,"Multiple metrics titles defined!"); 
    else parseContext.metricsTitleSeen = true;
    parseContext.metricsTitle = parseContext.str.str(); 
}
    break;

  case 17:
/* Line 670 of lalr1.cc  */
#line 412 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.calltreeTitleSeen)
      error(cubeparserloc,"Multiple calltree titles defined!"); 
    else parseContext.calltreeTitleSeen = true;
    parseContext.calltreeTitle = parseContext.str.str(); 
}
    break;

  case 19:
/* Line 670 of lalr1.cc  */
#line 422 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.systemtreeTitleSeen)
      error(cubeparserloc,"Multiple system tree titles defined!"); 
    else parseContext.systemtreeTitleSeen = true;
    parseContext.systemtreeTitle = parseContext.str.str(); 
}
    break;

  case 21:
/* Line 670 of lalr1.cc  */
#line 432 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.cubeplTypeSeen)
      error(cubeparserloc,"Multiple cubepl type attributes defines!"); 
    else parseContext.cubeplTypeSeen = true;
    parseContext.rowwise = false; 
    
}
    break;

  case 24:
/* Line 670 of lalr1.cc  */
#line 448 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.expressionAggrPlusSeen)
      error(cubeparserloc,"Multiple cubepl plus-aggregation attributes defines!"); 
    else parseContext.expressionAggrPlusSeen = true;
    parseContext.cubeplAggrType = ParseContext::CUBEPL_AGGR_PLUS; 
    
}
    break;

  case 25:
/* Line 670 of lalr1.cc  */
#line 458 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {  
    if (parseContext.expressionAggrMinusSeen)
      error(cubeparserloc,"Multiple cubepl type attributes defines!"); 
    else parseContext.expressionAggrMinusSeen = true;
    parseContext.cubeplAggrType = ParseContext::CUBEPL_AGGR_MINUS; 
    
}
    break;

  case 26:
/* Line 670 of lalr1.cc  */
#line 472 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.idSeen) 
	  error(cubeparserloc,"Multiple id attributes defines!"); 
	else parseContext.idSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Ids must be non-negative!"); 
	parseContext.id = (int)parseContext.longAttribute; 
}
    break;

  case 27:
/* Line 670 of lalr1.cc  */
#line 481 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.calleeidSeen) 
	  error(cubeparserloc,"Multiple callee id attributes defines!"); 
	else parseContext.calleeidSeen = true;
	if (parseContext.longAttribute<0) 
	  error(cubeparserloc,"Callee ids of regions must be non-negative!"); 
	parseContext.calleeid = (int)parseContext.longAttribute; 
}
    break;

  case 28:
/* Line 670 of lalr1.cc  */
#line 493 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.thrdidSeen) 
	  error(cubeparserloc,"Multiple location id attributes defines!"); 
	else parseContext.thrdidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Location ids must be non-negative!"); 
	parseContext.locid = (int)parseContext.longAttribute; 
}
    break;

  case 29:
/* Line 670 of lalr1.cc  */
#line 502 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.procidSeen) 
	  error(cubeparserloc,"Multiple location group id attributes defines!"); 
	else parseContext.procidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Location group ids must be non-negative!"); 
	parseContext.lgid = (int)parseContext.longAttribute; 
}
    break;

  case 30:
/* Line 670 of lalr1.cc  */
#line 511 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.nodeidSeen) 
	  error(cubeparserloc,"Multiple system tree node id attributes defines!"); 
	else parseContext.nodeidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"System tree node ids must be non-negative!"); 
	parseContext.stnid = (int)parseContext.longAttribute; 
}
    break;

  case 31:
/* Line 670 of lalr1.cc  */
#line 521 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.thrdidSeen) 
	  error(cubeparserloc,"Multiple thread id attributes defines!"); 
	else parseContext.thrdidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Thread ids must be non-negative!"); 
	parseContext.thrdid = (int)parseContext.longAttribute; 
}
    break;

  case 32:
/* Line 670 of lalr1.cc  */
#line 530 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.procidSeen) 
	  error(cubeparserloc,"Multiple process id attributes defines!"); 
	else parseContext.procidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Process ids must be non-negative!"); 
	parseContext.procid = (int)parseContext.longAttribute; 
}
    break;

  case 33:
/* Line 670 of lalr1.cc  */
#line 539 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.nodeidSeen) 
	  error(cubeparserloc,"Multiple node id attributes defines!"); 
	else parseContext.nodeidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Node ids must be non-negative!"); 
	parseContext.nodeid = (int)parseContext.longAttribute; 
}
    break;

  case 34:
/* Line 670 of lalr1.cc  */
#line 548 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.machidSeen) 
	  error(cubeparserloc,"Multiple machine id attributes defines!"); 
	else parseContext.machidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Machine ids must be non-negative!"); 
	parseContext.machid = (int)parseContext.longAttribute; 
}
    break;

  case 35:
/* Line 670 of lalr1.cc  */
#line 557 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.metricidSeen) 
	  error(cubeparserloc,"Multiple metric id attributes defines!"); 
	else parseContext.metricidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Metric ids must be non-negative!"); 
	parseContext.metricid = parseContext.longAttribute; 
}
    break;

  case 36:
/* Line 670 of lalr1.cc  */
#line 566 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.cnodeidSeen) 
	  error(cubeparserloc,"Multiple cnode id attributes defines!"); 
	else parseContext.cnodeidSeen = true;
	if (parseContext.longAttribute<0) error(cubeparserloc,"Cnode ids must be non-negative!"); 
	parseContext.cnodeid = (int)parseContext.longAttribute; 
}
    break;

  case 37:
/* Line 670 of lalr1.cc  */
#line 577 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.beginSeen) 
	  error(cubeparserloc,"Multiple begin attributes defines!"); 
	else parseContext.beginSeen = true;
	parseContext.beginln = parseContext.longAttribute; }
    break;

  case 38:
/* Line 670 of lalr1.cc  */
#line 585 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.endSeen) 
	  error(cubeparserloc,"Multiple end attributes defines!"); 
	else parseContext.endSeen = true;
	parseContext.endln = parseContext.longAttribute; }
    break;

  case 39:
/* Line 670 of lalr1.cc  */
#line 593 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.lineSeen) 
	  error(cubeparserloc,"Multiple line attributes defines!"); 
	else parseContext.lineSeen = true;
	parseContext.line = parseContext.longAttribute; }
    break;

  case 40:
/* Line 670 of lalr1.cc  */
#line 601 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.ndimsSeen) 
	  error(cubeparserloc,"Multiple ndims attributes defines!"); 
	else parseContext.ndimsSeen = true;
	if (parseContext.longAttribute<=0)
	   error(cubeparserloc,"Topology dimensions must be positive numbers!");
	parseContext.ndims = parseContext.longAttribute;
}
    break;

  case 41:
/* Line 670 of lalr1.cc  */
#line 611 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.sizeSeen) 
	  error(cubeparserloc,"Multiple size attributes defines!"); 
	else parseContext.sizeSeen = true;
	if (parseContext.longAttribute<=0)
	  error(cubeparserloc,"Dimension sizes must be positive numbers!");
	parseContext.dimVec.push_back(parseContext.longAttribute);
}
    break;

  case 42:
/* Line 670 of lalr1.cc  */
#line 623 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.periodicSeen) 
	  error(cubeparserloc,"Multiple periodic attributes defines!"); 
	else parseContext.periodicSeen = true;
	parseContext.periodicVec.push_back(false);
}
    break;

  case 43:
/* Line 670 of lalr1.cc  */
#line 629 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.periodicSeen) 
	  error(cubeparserloc,"Multiple periodic attributes defined!");
	else parseContext.periodicSeen = true;
	parseContext.periodicVec.push_back(true);
}
    break;

  case 44:
/* Line 670 of lalr1.cc  */
#line 640 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.murlSeen) 
	  error(cubeparserloc,"Multiple murl tags defined!"); 
	else parseContext.murlSeen = true;
	parseContext.murl = parseContext.str.str(); }
    break;

  case 45:
/* Line 670 of lalr1.cc  */
#line 650 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.dispnameSeen) 
	  error(cubeparserloc,"Multiple disp_name tags defined!"); 
	else parseContext.dispnameSeen = true;
	parseContext.disp_name = parseContext.str.str(); }
    break;

  case 46:
/* Line 670 of lalr1.cc  */
#line 657 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.uniqnameSeen) 
	  error(cubeparserloc,"Multiple uniq_name tags defined!"); 
	else parseContext.uniqnameSeen = true;
	parseContext.uniq_name = parseContext.str.str(); }
    break;

  case 47:
/* Line 670 of lalr1.cc  */
#line 663 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    { 
	if (parseContext.dtypeSeen) 
	  error(cubeparserloc,"Multiple dtype tags defined!"); 
	else parseContext.dtypeSeen = true;
	parseContext.dtype = parseContext.str.str(); }
    break;

  case 48:
/* Line 670 of lalr1.cc  */
#line 670 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.uomSeen) 
	  error(cubeparserloc,"Multiple uom tags defined!"); 
	else parseContext.uomSeen = true;
	parseContext.uom = parseContext.str.str(); }
    break;

  case 49:
/* Line 670 of lalr1.cc  */
#line 677 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.valSeen) 
	  error(cubeparserloc,"Multiple val tags defined!"); 
	else parseContext.valSeen = true;
	parseContext.val = parseContext.str.str(); }
    break;

  case 50:
/* Line 670 of lalr1.cc  */
#line 684 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.urlSeen) 
	  error(cubeparserloc,"Multiple url tags defined!");
	else parseContext.urlSeen = true;
	parseContext.url = parseContext.str.str(); }
    break;

  case 53:
/* Line 670 of lalr1.cc  */
#line 696 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.expressionSeen) 
	  error(cubeparserloc,"Multiple expressions defined!"); 
	else parseContext.expressionSeen = true;
	parseContext.expression = cube::services::escapeFromXML(parseContext.str.str()); 
	}
    break;

  case 54:
/* Line 670 of lalr1.cc  */
#line 704 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    if (parseContext.expressionInitSeen) 
      error(cubeparserloc,"Multiple expression initializations defined!"); 
    else parseContext.expressionInitSeen = true;
    parseContext.expressionInit = cube::services::escapeFromXML(parseContext.str.str()); 
    }
    break;

  case 55:
/* Line 670 of lalr1.cc  */
#line 712 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if ( parseContext.cubeplAggrType == ParseContext::CUBEPL_AGGR_PLUS)
	    parseContext.expressionAggrPlus = cube::services::escapeFromXML(parseContext.str.str()); 
	else
	if ( parseContext.cubeplAggrType == ParseContext::CUBEPL_AGGR_MINUS)
	    parseContext.expressionAggrMinus = cube::services::escapeFromXML(parseContext.str.str()); 
	}
    break;

  case 56:
/* Line 670 of lalr1.cc  */
#line 720 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.descrSeen) 
	  error(cubeparserloc,"Multiple descr tags defined!");
	else parseContext.descrSeen = true;
	parseContext.descr = parseContext.str.str(); }
    break;

  case 57:
/* Line 670 of lalr1.cc  */
#line 727 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.nameSeen) 
	  error(cubeparserloc,"Multiple name tags defined!"); 
	else parseContext.nameSeen = true;
	parseContext.name = parseContext.str.str(); 
	parseContext.mangled_name = parseContext.name; 
}
    break;

  case 58:
/* Line 670 of lalr1.cc  */
#line 737 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.mangledNameSeen) 
	  error(cubeparserloc,"Multiple mangled name tags defined!"); 
	else parseContext.mangledNameSeen = true;
	parseContext.mangled_name = parseContext.str.str(); }
    break;

  case 59:
/* Line 670 of lalr1.cc  */
#line 745 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    if (parseContext.paradigmSeen) 
      error(cubeparserloc,"Multiple paradigm tags defined!"); 
    else parseContext.paradigmSeen = true;
    parseContext.paradigm = parseContext.str.str(); }
    break;

  case 60:
/* Line 670 of lalr1.cc  */
#line 753 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    if (parseContext.roleSeen) 
      error(cubeparserloc,"Multiple role tags defined!"); 
    else parseContext.roleSeen = true;
    parseContext.role = parseContext.str.str(); }
    break;

  case 61:
/* Line 670 of lalr1.cc  */
#line 762 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.classSeen) 
	  error(cubeparserloc,"Multiple class tags defined!"); 
	else parseContext.classSeen = true;
	parseContext.stn_class = parseContext.str.str(); }
    break;

  case 62:
/* Line 670 of lalr1.cc  */
#line 771 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.typeSeen) 
	  error(cubeparserloc,"Multiple type tags defined!"); 
	else parseContext.typeSeen = true;
	parseContext.type = parseContext.str.str(); }
    break;

  case 63:
/* Line 670 of lalr1.cc  */
#line 778 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.rankSeen) 
	  error(cubeparserloc,"Multiple rank tags defined!"); 
	else parseContext.rankSeen = true;
	if (parseContext.longValues.size()==0)
	  error(cubeparserloc,"No rank is given in a rank tag!");
	if (parseContext.longValues.size()>1)
	  error(cubeparserloc,"Multiple ranks are given in a rank tag!");
	int rank = (int)parseContext.longValues[0];
	if (rank<0) 
	  error(cubeparserloc,"Ranks must be non-negative!");
	parseContext.rank = rank; 
}
    break;

  case 64:
/* Line 670 of lalr1.cc  */
#line 795 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    unsigned nparameters = 0;
    if (parseContext.cnode_parameter_type == "numeric")
    {
        if (parseContext.n_cnode_num_parameters.size() != 0 || true )
        {
            nparameters = parseContext.n_cnode_num_parameters.top();
            parseContext.n_cnode_num_parameters.pop();
        }
        nparameters++;
        parseContext.n_cnode_num_parameters.push(nparameters);
        std::pair<std::string, std::string > _key ;
        _key.first  =  cube::services::escapeFromXML(parseContext.cnode_parameter_key);
        _key.second =  cube::services::escapeFromXML(parseContext.cnode_parameter_value) ;
        parseContext.cnode_num_parameters.push( _key ); 

    }
    if (parseContext.cnode_parameter_type == "string")
    {
        if (parseContext.n_cnode_str_parameters.size() != 0 ||  true)
        {
            nparameters = parseContext.n_cnode_str_parameters.top();
            parseContext.n_cnode_str_parameters.pop();
        }
        nparameters++;
        parseContext.n_cnode_str_parameters.push(nparameters);
    
        std::pair<std::string, std::string > _key ;
        _key.first  =  cube::services::escapeFromXML(parseContext.cnode_parameter_key);
        _key.second =  cube::services::escapeFromXML(parseContext.cnode_parameter_value) ;
        parseContext.cnode_str_parameters.push( _key ); 
    }
    parseContext.stringContent = false;
    parseContext.cnodeParTypeSeen= false;
    parseContext.cnodeParKeySeen= false;
    parseContext.cnodeParValueSeen= false;
}
    break;

  case 66:
/* Line 670 of lalr1.cc  */
#line 846 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	if (!parseContext.versionSeen)
 	  error(cubeparserloc,"Missing version attribute!");
	else parseContext.versionSeen = false;
	if (!parseContext.encodingSeen)
	  error(cubeparserloc,"Missing encoding attribute!");
	else parseContext.encodingSeen = false;
	int valid = strcmp(parseContext.version.c_str(),"1.0");
	if (valid!=0) error(cubeparserloc,"XML version is expected to be 1.0!");
	valid = strcmp(parseContext.encoding.c_str(),"UTF-8");
	if (valid!=0) error(cubeparserloc,"XML encoding is expected to be UTF-8!");
}
    break;

  case 72:
/* Line 670 of lalr1.cc  */
#line 878 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	parseContext.cubeVersion = 4;
	cube.def_attr( "Cube anchor.xml syntax version", "4" );
}
    break;

  case 73:
/* Line 670 of lalr1.cc  */
#line 882 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    parseContext.cubeVersion = 4.1;
    cube.def_attr( "Cube anchor.xml syntax version", "4.1" );
}
    break;

  case 74:
/* Line 670 of lalr1.cc  */
#line 886 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    parseContext.cubeVersion = 4.2;
    cube.def_attr( "Cube anchor.xml syntax version", "4.2" );
}
    break;

  case 75:
/* Line 670 of lalr1.cc  */
#line 890 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    parseContext.cubeVersion = 4.3;
    cube.def_attr( "Cube anchor.xml syntax version", "4.3" );
}
    break;

  case 76:
/* Line 670 of lalr1.cc  */
#line 894 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    parseContext.cubeVersion = 4.4;
    cube.def_attr( "Cube anchor.xml syntax version", "4.4" );
}
    break;

  case 77:
/* Line 670 of lalr1.cc  */
#line 898 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    parseContext.cubeVersion = 3;
    cube.def_attr( "Cube syntax version", "3" );
}
    break;

  case 78:
/* Line 670 of lalr1.cc  */
#line 902 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
     throw cube::NotSupportedVersionError(parseContext.str.str());
}
    break;

  case 85:
/* Line 670 of lalr1.cc  */
#line 922 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	if (!parseContext.keySeen)
	  error(cubeparserloc,"Missing key attribute!"); 
	else parseContext.keySeen = false;
	if (!parseContext.valueSeen)
	  error(cubeparserloc,"Missing value attribute!"); 
	else parseContext.valueSeen = false;
	string attrkey = cube::services::escapeFromXML(parseContext.key);
	string attrvalue = cube::services::escapeFromXML(parseContext.value);
	cube.def_attr( attrkey, attrvalue );
	
	if (attrkey.compare(CUBEPL_VERSION_KEY) == 0)
	{
        cube.select_cubepl_engine( attrvalue );
	}
}
    break;

  case 96:
/* Line 670 of lalr1.cc  */
#line 969 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {parseContext.murlSeen = false; cube.def_mirror(cube::services::escapeFromXML(parseContext.murl));}
    break;

  case 97:
/* Line 670 of lalr1.cc  */
#line 970 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {parseContext.murlSeen = false; cube.def_mirror(cube::services::escapeFromXML(parseContext.murl));}
    break;

  case 98:
/* Line 670 of lalr1.cc  */
#line 979 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
  clustering_on = false;
  if ( cube.is_clustering_on() )
  {
    clustering_on = true;
    std::map<uint64_t, std::map<uint64_t, uint64_t> > clusters_counter;
    
    const std::map<std::string, std::string> & attrs = cube.get_attrs();
    std::map<std::string, std::string>::const_iterator _iter = attrs.find("CLUSTERING");
    if ( _iter  != attrs.end() && (*_iter).second.compare("ON") == 0)
    {
      std::map<std::string, std::string>::const_iterator _iter = attrs.find("CLUSTER ITERATION COUNT");
      if (_iter != attrs.end() )
      {
        parseContext.number_of_iterations = cube::services::string2int((*_iter).second);
        std::map<std::string, std::string>::const_iterator _iter = attrs.find("CLUSTER ROOT CNODE ID");
        if (_iter != attrs.end() )
        {
            parseContext.clustering_root_cnode_id = cube::services::string2int((*_iter).second);  
            for (uint64_t iteration = 0; iteration < parseContext.number_of_iterations; iteration ++)
            {
                std::string iteration_number = cube::services::numeric2string(iteration);
                std::string key = "CLUSTER MAPPING " + iteration_number;
                std::map<std::string, std::string>::const_iterator _iter = attrs.find(key);
                if (_iter != attrs.end())
                {
                    std::string value = (*_iter).second;
                    uint64_t iteration_key = iteration;
                    std::vector<uint64_t> _cluster_id = cube::services::parse_clustering_value( value );
                    std::vector<uint64_t> _collapsed_cluster_id = cube::services::sort_and_collapse_clusters( _cluster_id );
                    parseContext.cluster_mapping[ iteration_key] = _cluster_id;
                    parseContext.cluster_positions[ iteration_key ] = cube::services::get_cluster_positions( _cluster_id );
                    parseContext.collapsed_cluster_mapping[ iteration_key ] = _collapsed_cluster_id; 
                    uint64_t process_rank = 0;
                    for (std::vector<uint64_t>::iterator iter = _cluster_id.begin(); iter!= _cluster_id.end(); iter++, process_rank++)
                    {
                        uint64_t _id = *iter;
                        clusters_counter[_id][process_rank]++;
                    }
                }
                else 
                {
                cerr << "Clustering mapping is not continuous." << endl;
                clustering_on = false;
                cube.enable_clustering(false);
                break;
                }
            }
                cube.set_clusters_count(clusters_counter);
            }
            else 
            {
                cerr << "Cannot find cluster root cnode" << endl;
                clustering_on = false;
                cube.enable_clustering(false);	  
            }
        }
        else 
        {
            cerr << "Cannot find number of clustered iterations" << endl;
            clustering_on = false;
            cube.enable_clustering(false);
        }
    }
    else
    {
        clustering_on = false;
        cube.enable_clustering(false);
    }
  
  }
}
    break;

  case 99:
/* Line 670 of lalr1.cc  */
#line 1052 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    if (parseContext.metricsTitle != "")
        cube.set_metrics_title(cube::services::escapeFromXML(parseContext.metricsTitle));
}
    break;

  case 103:
/* Line 670 of lalr1.cc  */
#line 1068 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	    parseContext.idSeen = false;
	//check for tags
	if (parseContext.metricTypeSeen) parseContext.metricTypeSeen = false;
	else parseContext.metricType = "";
	if (parseContext.metricVizTypeSeen) parseContext.metricVizTypeSeen = false;
	else parseContext.metricVizType = "";
	if (parseContext.dispnameSeen) parseContext.dispnameSeen = false;
	else parseContext.disp_name = "";
	if (parseContext.uniqnameSeen) parseContext.uniqnameSeen = false;
	else parseContext.uniq_name = "";
	if (parseContext.dtypeSeen) parseContext.dtypeSeen = false;
	else parseContext.dtype = "";
	if (parseContext.uomSeen) parseContext.uomSeen = false;
	else parseContext.uom = "";
	if (parseContext.valSeen) parseContext.valSeen = false;
	else parseContext.val = "";
	if (parseContext.urlSeen) parseContext.urlSeen = false;
	else parseContext.url = "";
	if (parseContext.descrSeen) parseContext.descrSeen = false;
	else parseContext.descr = "";
	if (parseContext.expressionSeen) parseContext.expressionSeen = false;
	else parseContext.expression = "";
	if (parseContext.expressionInitSeen) parseContext.expressionInitSeen = false;
	else parseContext.expressionInit = "";
	if (parseContext.expressionAggrPlusSeen) parseContext.expressionAggrPlusSeen = false;
	else parseContext.expressionAggrPlus = "";
	if (parseContext.expressionAggrMinusSeen) parseContext.expressionAggrMinusSeen = false;
	else parseContext.expressionAggrMinusSeen = "";

	while ((int)parseContext.metricVec.size()<=parseContext.id) 
		parseContext.metricVec.push_back(NULL);
	if (parseContext.metricVec[parseContext.id]!=NULL)
	  error(cubeparserloc,"Re-declared metric!");

	    parseContext.currentMetric = 
		    cube.def_met(
			    cube::services::escapeFromXML(parseContext.disp_name),
			    cube::services::escapeFromXML(parseContext.uniq_name),
			    cube::services::escapeFromXML(parseContext.dtype),
			    cube::services::escapeFromXML(parseContext.uom),
			    cube::services::escapeFromXML(parseContext.val),
			    cube::services::escapeFromXML(parseContext.url),
			    cube::services::escapeFromXML(parseContext.descr),
			    parseContext.currentMetric,
			    cube::Metric::get_type_of_metric(parseContext.metricType),
			    cube::services::escapeFromXML(parseContext.expression),
			    cube::services::escapeFromXML(parseContext.expressionInit),
			    cube::services::escapeFromXML(parseContext.expressionAggrPlus),
			    cube::services::escapeFromXML(parseContext.expressionAggrMinus),
			    true,
			    cube::Metric::get_viz_type_of_metric(parseContext.metricVizType));
        if (parseContext.currentMetric != NULL)
        {
	    parseContext.currentMetric->set_filed_id( parseContext.id );
            if (parseContext.n_attributes.size() > 0)
            {
                unsigned nattributes = parseContext.n_attributes.top(); 
                parseContext.n_attributes.pop();
                
                if (nattributes > parseContext.attributes.size()) 
                {
                error(cubeparserloc, "Number of saved attributes for metric " + parseContext.uniq_name + " is more, than number of actual saved parameters.");
                }
                for (unsigned i=0; i< nattributes; i++) 
                {
                    std::pair< std::string, std::string>  attr = parseContext.attributes.top();
                    parseContext.attributes.pop();
                    parseContext.currentMetric->def_attr(attr.first, attr.second);
                }
            }
        parseContext.metricVec[parseContext.id] = parseContext.currentMetric;
        }
        else
        {
            std::string _error = "Cannot create metric  " + parseContext.uniq_name + ". Ignore it.";
            cerr << _error << endl;
/*             error(cubeparserloc,_error.c_str()); */
        }
}
    break;

  case 125:
/* Line 670 of lalr1.cc  */
#line 1189 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    //check for attributes
    if (!parseContext.keySeen)
      error(cubeparserloc,"Missing key attribute!"); 
    else parseContext.keySeen = false;
    if (!parseContext.valueSeen)
      error(cubeparserloc,"Missing value attribute!"); 
    else parseContext.valueSeen = false;
    
    unsigned nattributes = 0;
    if (parseContext.n_attributes.size() != 0  )
    {
        nattributes = parseContext.n_attributes.top();
        parseContext.n_attributes.pop();
    }
    nattributes++;
    parseContext.n_attributes.push(nattributes);
    std::pair<std::string, std::string > _key ;
    _key.first  =  cube::services::escapeFromXML(parseContext.key);
    _key.second =  cube::services::escapeFromXML(parseContext.value) ;
    parseContext.attributes.push( _key );  
}
    break;

  case 126:
/* Line 670 of lalr1.cc  */
#line 1217 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (parseContext.currentMetric!=NULL)
        parseContext.currentMetric = 
            (parseContext.currentMetric)->get_parent();
}
    break;

  case 127:
/* Line 670 of lalr1.cc  */
#line 1229 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    if (parseContext.calltreeTitle != "")
        cube.set_calltree_title(cube::services::escapeFromXML(parseContext.calltreeTitle));
        
    
  if (clustering_on) // here look for region template  and create subtrees for iterations
  {
    // after parsing, whole stored calltree is stored not within cube, but separatelly, in parseContext.oroginal_tree_roots. 
    // here we create first a copy of the original tree without clustering subtrees
    cube::Cnode * clusters_root = NULL;
    cube.set_original_root_cnodev( parseContext.original_tree_roots );
    for (std::vector<cube::Cnode*>::iterator citer= parseContext.original_tree_roots.begin(); citer!=parseContext.original_tree_roots.end(); ++citer )
    {
        cube::Cnode* cnode = *citer;
        cube::Cnode * _clusters_root = NULL;
        cube::services::copy_tree( cnode, _clusters_root, parseContext.clustering_root_cnode_id,  NULL, &cube);
        if (_clusters_root != NULL )
            clusters_root = _clusters_root;
    }   
    parseContext.clustering_root_cnode = clusters_root;
    
    if (cube::services::get_children( parseContext.original_tree_roots,
        parseContext.clustering_root_cnode_id,
        parseContext.clusters_trees))
    {
        int i = 0;
        for (std::vector<cube::Cnode*>::iterator citer= parseContext.clusters_trees.begin(); citer!=parseContext.clusters_trees.end(); ++citer, ++i )
        {
            parseContext.id2cluster[ (*citer)->get_id() ] = i;
        }   
          
    }else
    {
        clustering_on = false;
        break;
    }
			      
    

     // here we are going througs registered sofar regions and look for first cluster to take its region as a template for iterations
     for (std::vector<cube::Region*>::const_iterator riter= cube.get_regv().begin(); riter!=cube.get_regv().end(); ++riter )
     {
      cube::Region* region = *riter;
      if (region->get_name().compare("instance=1")==0)
      {
        parseContext.iterationRegionTemplate = region;
        break;
      }
     }
     // if we didn't found any regions, but clustering was "on" -> fatal error appeared. 
     if (parseContext.iterationRegionTemplate == NULL ) // we didnt find any template for iteration subtree, bad cube layout
      throw cube::CubeClusteringLayoutError("Cannot find a template for iteration calltree.");
 
    // here we merge different combinations of clusters.
    // one can reduce amount of merges if one finds ont set of different collapsed rows - improvements for future.  
 
      // here we create regions for iterations
     std::map<uint64_t,   std::map<uint64_t, uint64_t> > cluster_counter = cube.get_clusters_counter();
      for (unsigned i=0; i< parseContext.number_of_iterations; i++)
      {
        std::stringstream sstr;
        std::string iteration_name;
        sstr << i;
        sstr >> iteration_name;
        // we create a regions
        cube::Region * region =  cube.def_region( 
                            "iteration="+iteration_name,
			    "iteration="+iteration_name,
                            parseContext.iterationRegionTemplate->get_paradigm(),
                            parseContext.iterationRegionTemplate->get_role(),
                            parseContext.iterationRegionTemplate->get_begn_ln(),
                            parseContext.iterationRegionTemplate->get_end_ln(), 
                            parseContext.iterationRegionTemplate->get_url(),
                            parseContext.iterationRegionTemplate->get_descr(),
                            parseContext.iterationRegionTemplate->get_mod() 
                            );
        parseContext.iteration_regions.push_back( region );
        // create its cnode
        cube::Cnode* iteration_cnode = cube.def_cnode(
                            region,
                            region->get_mod(), 
                            region->get_begn_ln(), 
                            parseContext.clustering_root_cnode
                            );							  	

        // here we merge all subtrees of all clusters to this callpath.
        std::vector<cube::Cnode*> subtrees;
        for ( std::vector< uint64_t>::iterator iter= parseContext.collapsed_cluster_mapping[i].begin(); iter != parseContext.collapsed_cluster_mapping[i].end(); ++iter  )
        {   
            uint64_t _cluster_id = *iter;
            std::map<uint64_t, uint64_t> _cluster_normalization = cluster_counter[ _cluster_id ];
            uint64_t _cluster_position = parseContext.id2cluster[_cluster_id];
            cube::Cnode* cluster_root = parseContext.clusters_trees[_cluster_position];
            cube.store_ghost_cnode( cluster_root ); 
            for ( size_t j = 0; j <  parseContext.cluster_positions[ i ][ _cluster_id ].size(); ++j )
            {
                iteration_cnode->set_remapping_cnode( parseContext.cluster_positions[ i ][ _cluster_id ][j], cluster_root,  _cluster_normalization[parseContext.cluster_positions[ i ][ _cluster_id ][j]] );
            }
            cube::services::gather_children( subtrees, cluster_root );
            cube::services::merge_trees(subtrees, iteration_cnode, &cube, &_cluster_normalization, &(parseContext.cluster_positions[ i ][ _cluster_id ]) );
        }  
      }
  }   
 
}
    break;

  case 130:
/* Line 670 of lalr1.cc  */
#line 1342 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for the attributes
	if (!parseContext.idSeen)
	  error(cubeparserloc,"Missing id attribute!"); 
	else parseContext.idSeen = false;
	if (!parseContext.modSeen)
		parseContext.mod = "";
	else parseContext.modSeen = false;
	if (!parseContext.beginSeen)
		parseContext.beginln = -1;
	else parseContext.beginSeen = false;
	if (!parseContext.endSeen)
		parseContext.endln = -1;
	else parseContext.endSeen = false;
    if (!parseContext.paradigmSeen)
        parseContext.paradigm = "unknown";
    else parseContext.paradigmSeen = false;
    if (!parseContext.roleSeen)
        parseContext.role = "unknown";
    else parseContext.roleSeen = false;    
	//check for tags
	if (parseContext.nameSeen) parseContext.nameSeen = false;
	else parseContext.name = "";
	if (parseContext.mangledNameSeen) parseContext.mangledNameSeen = false;
	else parseContext.mangled_name = parseContext.name;
	if (parseContext.urlSeen) parseContext.urlSeen = false;
	else parseContext.url = "";
	if (parseContext.descrSeen) parseContext.descrSeen = false;
	else parseContext.descr = "";
	
	while ((int)parseContext.regionVec.size()<=parseContext.id) 
		parseContext.regionVec.push_back(NULL);
	if (parseContext.regionVec[parseContext.id]!=NULL)
	  error(cubeparserloc,"Re-declared region!"); 
 
	cube::Region * _region =
        cube.def_region(
		 cube::services::escapeFromXML(parseContext.name),
		 cube::services::escapeFromXML(parseContext.mangled_name),
		 cube::services::escapeFromXML(parseContext.paradigm),
		 cube::services::escapeFromXML(parseContext.role),
			parseContext.beginln,
			parseContext.endln,
			parseContext.url,
		 cube::services::escapeFromXML(parseContext.descr),
		 cube::services::escapeFromXML(parseContext.mod),
			parseContext.id);
    if (parseContext.n_attributes.size() > 0)
    {
        unsigned nattributes = parseContext.n_attributes.top(); 
        parseContext.n_attributes.pop();
        
        if (nattributes > parseContext.attributes.size()) 
        {
        error(cubeparserloc, "Number of saved attributes for region " + parseContext.name + " is more, than number of actual saved parameters.");
        }
        for (unsigned i=0; i< nattributes; ++i) 
        {
            std::pair< std::string, std::string>  attr = parseContext.attributes.top();
            parseContext.attributes.pop();
            _region->def_attr(attr.first, attr.second);
        }
    }
    parseContext.regionVec[parseContext.id] = _region;
}
    break;

  case 150:
/* Line 670 of lalr1.cc  */
#line 1446 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    if (parseContext.n_attributes.size() > 0)
    {
        unsigned nattributes = parseContext.n_attributes.top(); 
        parseContext.n_attributes.pop();
        
        if (nattributes > parseContext.attributes.size()) 
        {
            error(cubeparserloc, "Number of saved attributes for cnode is more, than number of actual saved parameters.");
        }
        for (unsigned i=0; i< nattributes; ++i) 
        {
            std::pair< std::string, std::string>  attr = parseContext.attributes.top();
            parseContext.attributes.pop();
            parseContext.currentCnode->def_attr(attr.first, attr.second);
        }    
    }
    
    
    if (!clustering_on)
    {      
    
	  if (parseContext.currentCnode==NULL)
	    error(cubeparserloc,"Cnode definitions are not correctly nested!"); 


	  if (!parseContext.n_cnode_num_parameters.empty()  )
	  {
	      unsigned nparameters = parseContext.n_cnode_num_parameters.top(); 
	      parseContext.n_cnode_num_parameters.pop();
	      
	      if (nparameters > parseContext.cnode_num_parameters.size()) 
	      {
		  error(cubeparserloc, "Number of saved numeric parameters for current cnode is more, than number of actual saved parameters.");
	      }
	      for (unsigned i=0; i< nparameters; ++i) 
	      {
		  std::pair< std::string, std::string>  param = parseContext.cnode_num_parameters.top();
		  
		  parseContext.cnode_num_parameters.pop();

		  double d_value = atof(param.second.data());
		  parseContext.currentCnode->add_num_parameter(param.first, d_value);
	      }
	  }
	  if (!parseContext.n_cnode_str_parameters.empty()  )
	  {
	      unsigned nparameters = parseContext.n_cnode_str_parameters.top(); 
	      parseContext.n_cnode_str_parameters.pop();
	      
	      if (nparameters > parseContext.cnode_str_parameters.size()) 
	      {
		  error(cubeparserloc, "Number of saved string parameters for current cnode is more, than number of actual saved parameters.");
	      }
	      for (unsigned i=0; i< nparameters; ++i) 
	      {

		  std::pair< std::string, std::string>  param = parseContext.cnode_str_parameters.top();
		  parseContext.cnode_str_parameters.pop();
		  parseContext.currentCnode->add_str_parameter(param.first, param.second);
	      }
	  }	

	  parseContext.currentCnode = 
		  parseContext.currentCnode->get_parent();
	  parseContext.start_parse_clusters = false;
    }
    else
    {
	if (parseContext.clusterCurrentCnode==NULL)
	  error(cubeparserloc,"Cluster cnode definitions are not correctly nested!"); 


	if (!parseContext.n_cnode_num_parameters.empty()  )
	{
	    unsigned nparameters = parseContext.n_cnode_num_parameters.top(); 
	    parseContext.n_cnode_num_parameters.pop();
	    
	    if (nparameters > parseContext.cnode_num_parameters.size()) 
	    {
		error(cubeparserloc, "Number of saved numeric parameters for current cnode is more, than number of actual saved parameters.");
	    }
	    for (unsigned i=0; i< nparameters; ++i) 
	    {
		std::pair< std::string, std::string>  param = parseContext.cnode_num_parameters.top();
		
		parseContext.cnode_num_parameters.pop();

		double d_value = atof(param.second.data());
		parseContext.clusterCurrentCnode->add_num_parameter(param.first, d_value);
	    }
	}
	if (!parseContext.n_cnode_str_parameters.empty()  )
	{
	    unsigned nparameters = parseContext.n_cnode_str_parameters.top(); 
	    parseContext.n_cnode_str_parameters.pop();
	    
	    if (nparameters > parseContext.cnode_str_parameters.size()) 
	    {
		error(cubeparserloc, "Number of saved string parameters for current cnode is more, than number of actual saved parameters.");
	    }
	    for (unsigned i=0; i< nparameters; ++i) 
	    {
		std::pair< std::string, std::string>  param = parseContext.cnode_str_parameters.top();
		parseContext.cnode_str_parameters.pop();
		parseContext.clusterCurrentCnode->add_str_parameter(param.first, param.second);
	    }
	}

	parseContext.clusterCurrentCnode = 
		parseContext.clusterCurrentCnode->get_parent();    
	if (parseContext.clusterCurrentCnode == NULL)
	{
	  parseContext.parse_clusters = false;
	}

    }

}
    break;

  case 151:
/* Line 670 of lalr1.cc  */
#line 1568 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for the attributes 
	if (!parseContext.idSeen)
	  error(cubeparserloc,"Missing id attribute in cnode definition!"); 
	else parseContext.idSeen = false;
	if (!parseContext.calleeidSeen)
	  error(cubeparserloc,"Missing callee id attribute in cnode definition!"); 
	else parseContext.calleeidSeen = false;
	if (!parseContext.lineSeen)
	  	parseContext.line = -1;
	else parseContext.lineSeen = false;
	if (!parseContext.modSeen)
	  	parseContext.mod = "";
	else parseContext.modSeen = false;
	//check if the region is defined
	if ((int)parseContext.regionVec.size()<=parseContext.calleeid)
	  error(cubeparserloc,"Undefined region in cnode definition!"); 
	if (parseContext.regionVec[parseContext.calleeid]==NULL)
	  error(cubeparserloc,"Undefined region in cnode definition!"); 
	//extend the cnode vector if necessary
	while ((int)parseContext.cnodeVec.size()<=parseContext.id) 
		parseContext.cnodeVec.push_back(NULL);
	if (parseContext.cnodeVec[parseContext.id]!=NULL)
	  error(cubeparserloc,"Re-declared cnode!");
	//define the cnode

    parseContext.cnodeParTypeSeen= false;
    parseContext.cnodeParKeySeen= false;
    parseContext.cnodeParValueSeen= false;

    if (clustering_on)
    {
	  parseContext.clusterCurrentCnode = new cube::Cnode( parseContext.regionVec[parseContext.calleeid] ,
							    cube::services::escapeFromXML(parseContext.mod),
							    parseContext.line,
							    parseContext.clusterCurrentCnode,
							    parseContext.id );
        if (parseContext.clusterCurrentCnode->get_parent() == NULL) // store root call path for cluster in a vector.
        {
        parseContext.original_tree_roots.push_back(parseContext.clusterCurrentCnode );
        }
    }
    else
    {
      parseContext.currentCnode = cube.def_cnode(
			parseContext.regionVec[parseContext.calleeid],
		 cube::services::escapeFromXML(parseContext.mod),
			parseContext.line,
			parseContext.currentCnode,
			parseContext.id);

	parseContext.cnodeVec[parseContext.id] =
		parseContext.currentCnode;		
    }
}
    break;

  case 162:
/* Line 670 of lalr1.cc  */
#line 1653 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
    if (parseContext.systemtreeTitle != "")
        cube.set_systemtree_title(cube::services::escapeFromXML(parseContext.systemtreeTitle));
        
        // at this moment all dimensionis are loadeed -> we can initialize containers
        cube.initialize();
}
    break;

  case 167:
/* Line 670 of lalr1.cc  */
#line 1674 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.idSeen = false;
	//check for tags
	parseContext.nameSeen = false;
	if (!parseContext.classSeen )
	   error(cubeparserloc,"No class of system tree node declared."); 
	parseContext.classSeen = false;
	if (parseContext.descrSeen) parseContext.descrSeen = false;
	else parseContext.descr = "";

	cube::SystemTreeNode * _stn = cube.def_system_tree_node(
				    cube::services::escapeFromXML(parseContext.name),
				    cube::services::escapeFromXML(parseContext.descr), 
				    cube::services::escapeFromXML(parseContext.stn_class), 
				    (parseContext.currentSystemTreeNode.size() == 0)? NULL : parseContext.currentSystemTreeNode.top() ); 
    if (parseContext.n_attributes.size() > 0)
    {				    
        unsigned nattributes = parseContext.n_attributes.top(); 
        parseContext.n_attributes.pop();
        
        if (nattributes > parseContext.attributes.size()) 
        {
        error(cubeparserloc, "Number of saved attributes for metric " + parseContext.uniq_name + " is more, than number of actual saved parameters.");
        }
        for (unsigned i=0; i< nattributes; ++i) 
        {
            std::pair< std::string, std::string>  attr = parseContext.attributes.top();
            parseContext.attributes.pop();
            _stn->def_attr(attr.first, attr.second);
        }  				    
    }		    
				    
				    
	parseContext.currentSystemTreeNode.push(_stn);
 	while ((unsigned int)parseContext.stnVec.size()<=_stn->get_id()) 
 		parseContext.stnVec.push_back(NULL); 
	if (parseContext.stnVec[_stn->get_id()]!=NULL)
	  error(cubeparserloc,"Re-declared system tree node!"); 
	parseContext.stnVec[_stn->get_id()] =_stn;

}
    break;

  case 168:
/* Line 670 of lalr1.cc  */
#line 1719 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {

  parseContext.currentSystemTreeNode.pop();
	//check for attributes
  parseContext.idSeen = false;
	//check for tags
	parseContext.nameSeen = false;
  if (parseContext.descrSeen) parseContext.descrSeen = false;
      else parseContext.descr = "";
}
    break;

  case 175:
/* Line 670 of lalr1.cc  */
#line 1754 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.idSeen = false;
	//check for tags
	parseContext.nameSeen = false;
	parseContext.rankSeen = false;
	if (!parseContext.typeSeen )
	   error(cubeparserloc,"No type of location group declared."); 
	parseContext.typeSeen = false;

	if (parseContext.descrSeen) parseContext.descrSeen = false;
	  else parseContext.descr = "";

	parseContext.currentLocationGroup = cube.def_location_group(
	 cube::services::escapeFromXML(parseContext.name),
		parseContext.rank,
		cube::LocationGroup::getLocationGroupType(cube::services::escapeFromXML(parseContext.type)),
		parseContext.currentSystemTreeNode.top());
		
    if (parseContext.n_attributes.size() > 0)
    {		
        unsigned nattributes = parseContext.n_attributes.top(); 
        parseContext.n_attributes.pop();
        
        if (nattributes > parseContext.attributes.size()) 
        {
        error(cubeparserloc, "Number of saved attributes for location group " + parseContext.name + " is more, than number of actual saved parameters.");
        }
        for (unsigned i=0; i< nattributes; ++i) 
        {
            std::pair< std::string, std::string>  attr = parseContext.attributes.top();
            parseContext.attributes.pop();
            parseContext.currentLocationGroup->def_attr(attr.first, attr.second);
        }   	
    }
		
		
	while ((unsigned int)parseContext.locGroup.size()<=parseContext.currentLocationGroup->get_id())
		parseContext.locGroup.push_back(NULL);
	if (parseContext.locGroup[parseContext.currentLocationGroup->get_id()]!=NULL)
	  error(cubeparserloc,"Re-declared location group!"); 
	parseContext.locGroup[parseContext.currentLocationGroup->get_id()] = parseContext.currentLocationGroup;
}
    break;

  case 183:
/* Line 670 of lalr1.cc  */
#line 1819 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.idSeen = false;
	//check for tags
	parseContext.nameSeen = false;
	parseContext.rankSeen = false;

	if (!parseContext.typeSeen )
	   error(cubeparserloc,"No type of location group declared."); 
	parseContext.typeSeen = false;
	parseContext.currentLocation = cube.def_location(
	 cube::services::escapeFromXML(parseContext.name),
		parseContext.rank,
		cube::Location::getLocationType(cube::services::escapeFromXML(parseContext.type)),
		parseContext.currentLocationGroup,
		parseContext.id);
    if (parseContext.n_attributes.size() > 0)
    {
        unsigned nattributes = parseContext.n_attributes.top(); 
        parseContext.n_attributes.pop();
        
        if (nattributes > parseContext.attributes.size()) 
        {
        error(cubeparserloc, "Number of saved attributes for location " + parseContext.name + " is more, than number of actual saved parameters.");
        }
        for (unsigned i=0; i< nattributes; ++i) 
        {
            std::pair< std::string, std::string>  attr = parseContext.attributes.top();
            parseContext.attributes.pop();
            parseContext.currentLocation->def_attr(attr.first, attr.second);
        }  	
    }
		
	while (( unsigned int)parseContext.locVec.size()<=parseContext.currentLocation->get_id())
		parseContext.locVec.push_back(NULL);
	if (parseContext.locVec[parseContext.currentLocation->get_id()]!=NULL)
	  error(cubeparserloc,"Re-declared location!"); 
	parseContext.locVec[parseContext.currentLocation->get_id()] = parseContext.currentLocation;
}
    break;

  case 186:
/* Line 670 of lalr1.cc  */
#line 1869 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.idSeen = false;
	//check for tags
	parseContext.nameSeen = false;
	if (parseContext.descrSeen) parseContext.descrSeen = false;
	else parseContext.descr = "";

	parseContext.currentMachine = cube.def_mach(
	 cube::services::escapeFromXML(parseContext.name),
	 cube::services::escapeFromXML(parseContext.descr));
	while ((unsigned int)parseContext.machVec.size()<=parseContext.currentMachine->get_id())
		parseContext.machVec.push_back(NULL);
	if (parseContext.machVec[parseContext.currentMachine->get_id()]!=NULL)
	  error(cubeparserloc,"Re-declared machine!"); 
	parseContext.machVec[parseContext.currentMachine->get_id()] = parseContext.currentMachine;
}
    break;

  case 190:
/* Line 670 of lalr1.cc  */
#line 1894 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.idSeen = false;
	//check for tags
	parseContext.nameSeen = false;
    if (parseContext.descrSeen) parseContext.descrSeen = false;
    else parseContext.descr = "";
    
	parseContext.currentNode = cube.def_node(
	 cube::services::escapeFromXML(parseContext.name),
		parseContext.currentMachine);
	while ((unsigned int)parseContext.nodeVec.size()<=parseContext.currentNode->get_id())
		parseContext.nodeVec.push_back(NULL);
	if (parseContext.nodeVec[parseContext.currentNode->get_id()]!=NULL)
	  error(cubeparserloc,"Re-declared node!");
	parseContext.nodeVec[parseContext.currentNode->get_id()] = parseContext.currentNode;
}
    break;

  case 194:
/* Line 670 of lalr1.cc  */
#line 1919 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.idSeen = false;
	//check for tags
	if (parseContext.nameSeen) parseContext.nameSeen = false;
	else parseContext.name = "";
	if (parseContext.rankSeen) parseContext.rankSeen = false;
	else parseContext.rank = 0;

	std::ostringstream name;
	if (parseContext.name.empty()) {
	  name << "Process " << parseContext.rank;
	} else {
	  name << parseContext.name;
	}
	parseContext.currentProc = cube.def_proc(
	 cube::services::escapeFromXML(name.str()),
		parseContext.rank,
		parseContext.currentNode);
	while ((unsigned int)parseContext.procVec.size()<=parseContext.currentProc->get_id())
		parseContext.procVec.push_back(NULL);
	if (parseContext.procVec[parseContext.currentProc->get_id()]!=NULL)
	  error(cubeparserloc,"Re-declared process!");
	parseContext.procVec[parseContext.currentProc->get_id()] = parseContext.currentProc;
}
    break;

  case 202:
/* Line 670 of lalr1.cc  */
#line 1961 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.idSeen = false;
	//check for tags
	if (parseContext.nameSeen) parseContext.nameSeen = false;
	else parseContext.name = "";
	if (parseContext.rankSeen) parseContext.rankSeen = false;
	else parseContext.rank = 0;

	std::ostringstream name;
	if (parseContext.name.empty()) {
	  name << "Thread " << parseContext.rank;
	} else {
	  name << parseContext.name;
	}
	
	
	parseContext.currentThread = cube.def_thrd(
	 cube::services::escapeFromXML(name.str()),
		parseContext.rank,
		parseContext.currentProc,
		parseContext.id);
	while ((unsigned int)parseContext.threadVec.size()<=parseContext.currentThread->get_id()) 
		parseContext.threadVec.push_back(NULL);
	if (parseContext.threadVec[parseContext.currentThread->get_id()]!=NULL)
	  error(cubeparserloc,"Re-declared thread!"); 
	parseContext.threadVec[parseContext.currentThread->get_id()] = parseContext.currentThread;
}
    break;

  case 205:
/* Line 670 of lalr1.cc  */
#line 1996 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (!parseContext.checkThreadIds())
	  error(cubeparserloc,"Thread ids must cover an interval [0,n] without gap!"); 
}
    break;

  case 213:
/* Line 670 of lalr1.cc  */
#line 2017 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.ndimsSeen = false;

	if (parseContext.ndims!=(int)parseContext.dimVec.size())
	  error(cubeparserloc,"Too few or too many topology dimensions are declared!"); 
	parseContext.currentCart = cube.def_cart(
		parseContext.ndims,
		parseContext.dimVec,
		parseContext.periodicVec);

        if (parseContext.cartNameSeen)
	{
            (parseContext.currentCart)->set_name(cube::services::escapeFromXML(parseContext.cartName));
	    parseContext.cartNameSeen=false;
	}
	if(parseContext.dimNamesCount>0)
	{
	    parseContext.dimNamesCount = 0;
	    (parseContext.currentCart)->set_namedims(parseContext.dimNameVec);
	}
	parseContext.dimNameVec.clear();

}
    break;

  case 215:
/* Line 670 of lalr1.cc  */
#line 2044 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {parseContext.dimVec.clear();
	   parseContext.periodicVec.clear();
        parseContext.cartNameSeen=false;
}
    break;

  case 218:
/* Line 670 of lalr1.cc  */
#line 2056 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	if (!parseContext.sizeSeen)
	  error(cubeparserloc,"Missing size attribute!"); 
	else parseContext.sizeSeen = false;
	if (!parseContext.periodicSeen)
	  error(cubeparserloc,"Missing periodic attribute!");
	else parseContext.periodicSeen = false;
	if (parseContext.dimNameSeen )
	    parseContext.dimNameVec.push_back(parseContext.dimName); 
	else
	    parseContext.dimNameVec.push_back(""); 
	parseContext.dimNameSeen= false; 
  }
    break;

  case 233:
/* Line 670 of lalr1.cc  */
#line 2105 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.thrdidSeen = false;

	if (parseContext.dimVec.size()!=parseContext.longValues.size())
	  error(cubeparserloc,"Too few or too many dimension coordinates in coord tag!"); 
	for (unsigned i=0; i<parseContext.dimVec.size(); ++i){
	    if (parseContext.longValues[i]>=parseContext.dimVec[i])
	  error(cubeparserloc,"Topology coordinate is out of range!");
	}
	if (parseContext.locid>=(int)parseContext.locVec.size()) 
	  error(cubeparserloc,"Location of the topology coordinates wasn't declared!"); 
	if (parseContext.locVec[parseContext.locid]==NULL) 
	  error(cubeparserloc,"Location of the topology coordinates wasn't declared!"); 
	cube.def_coords(parseContext.currentCart,
                         (cube::Sysres*)(parseContext.locVec[parseContext.locid]),
                         parseContext.longValues);
}
    break;

  case 234:
/* Line 670 of lalr1.cc  */
#line 2126 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.procidSeen = false;

	if (parseContext.dimVec.size()!=parseContext.longValues.size())
	  error(cubeparserloc,"Too few or too many dimension coordinates in coord tag!"); 
	for (unsigned i=0; i<parseContext.dimVec.size(); ++i){
	    if (parseContext.longValues[i]>=parseContext.dimVec[i])
	  error(cubeparserloc,"Topology coordinate is out of range!"); 
	}
	if (parseContext.lgid>=(int)parseContext.locGroup.size()) 
	  error(cubeparserloc,"Location group of the topology coordinates wasn't declared!"); 
	if (parseContext.locGroup[parseContext.lgid]==NULL) 
	  error(cubeparserloc,"Location group of the topology coordinates wasn't declared!"); 
	cube.def_coords(parseContext.currentCart,
                         (cube::Sysres*)(parseContext.locGroup[parseContext.lgid]),
                         parseContext.longValues);
}
    break;

  case 235:
/* Line 670 of lalr1.cc  */
#line 2147 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.nodeidSeen = false;

	if (parseContext.dimVec.size()!=parseContext.longValues.size())
	  error(cubeparserloc,"Too few or too many dimension coordinates in coord tag!"); 
	for (unsigned i=0; i<parseContext.dimVec.size(); ++i){
	    if (parseContext.longValues[i]>=parseContext.dimVec[i])
	  error(cubeparserloc,"Topology coordinate is out of range!"); 
	}
	if (parseContext.stnid>=(int)parseContext.stnVec.size()) 
	  error(cubeparserloc,"System tree node of the topology coordinates wasn't declared!"); 
	if (parseContext.nodeVec[parseContext.stnid]==NULL) 
	  error(cubeparserloc,"System tree node of the topology coordinates wasn't declared!"); 
	cube.def_coords(parseContext.currentCart,
                         (cube::Sysres*)(parseContext.nodeVec[parseContext.stnid]),
                         parseContext.longValues);
}
    break;

  case 236:
/* Line 670 of lalr1.cc  */
#line 2169 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.thrdidSeen = false;

	if (parseContext.dimVec.size()!=parseContext.longValues.size())
	  error(cubeparserloc,"Too few or too many dimension coordinates in coord tag!"); 
	for (unsigned i=0; i<parseContext.dimVec.size(); ++i){
	    if (parseContext.longValues[i]>=parseContext.dimVec[i])
	  error(cubeparserloc,"Topology coordinate is out of range!");
	}
	if (parseContext.thrdid>=(int)parseContext.threadVec.size()) 
	  error(cubeparserloc,"Thread of the topology coordinates wasn't declared!"); 
	if (parseContext.threadVec[parseContext.thrdid]==NULL) 
	  error(cubeparserloc,"Thread of the topology coordinates wasn't declared!"); 
	cube.def_coords(parseContext.currentCart,
                         (cube::Sysres*)(parseContext.threadVec[parseContext.thrdid]),
                         parseContext.longValues);
}
    break;

  case 237:
/* Line 670 of lalr1.cc  */
#line 2190 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.procidSeen = false;

	if (parseContext.dimVec.size()!=parseContext.longValues.size())
	  error(cubeparserloc,"Too few or too many dimension coordinates in coord tag!"); 
	for (unsigned i=0; i<parseContext.dimVec.size(); ++i){
	    if (parseContext.longValues[i]>=parseContext.dimVec[i])
	  error(cubeparserloc,"Topology coordinate is out of range!"); 
	}
	if (parseContext.procid>=(int)parseContext.locGroup.size()) 
	  error(cubeparserloc,"Process of the topology coordinates wasn't declared!"); 
	if (parseContext.locGroup[parseContext.procid]==NULL) 
	  error(cubeparserloc,"Process of the topology coordinates wasn't declared!"); 
	cube.def_coords(parseContext.currentCart,
                         (cube::Sysres*)(parseContext.locGroup[parseContext.procid]),
                         parseContext.longValues);
}
    break;

  case 238:
/* Line 670 of lalr1.cc  */
#line 2211 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.nodeidSeen = false;

	if (parseContext.dimVec.size()!=parseContext.longValues.size())
	  error(cubeparserloc,"Too few or too many dimension coordinates in coord tag!"); 
	for (unsigned i=0; i<parseContext.dimVec.size(); ++i){
	    if (parseContext.longValues[i]>=parseContext.dimVec[i])
	  error(cubeparserloc,"Topology coordinate is out of range!"); 
	}
	
	if (parseContext.nodeid >= (int)cube.get_non_root_stnv().size()) 
	  error(cubeparserloc,"Node of the topology coordinates wasn't declared!"); 
	if (cube.get_non_root_stnv().at(parseContext.nodeid)==NULL) 
	  error(cubeparserloc,"Node of the topology coordinates wasn't declared!"); 
	cube.def_coords(parseContext.currentCart,
                         (cube::Sysres*)(cube.get_non_root_stnv().at(parseContext.nodeid)),
                         parseContext.longValues);
}
    break;

  case 239:
/* Line 670 of lalr1.cc  */
#line 2233 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.machidSeen = false;

	if (parseContext.dimVec.size()!=parseContext.longValues.size())
	  error(cubeparserloc,"Too few or too many dimension coordinates in coord tag!"); 
	for (unsigned i=0; i<parseContext.dimVec.size(); ++i){
	    if (parseContext.longValues[i]>=parseContext.dimVec[i])
	  error(cubeparserloc,"Topology coordinate is out of range!"); 
	}
	if (parseContext.machid>=(int)cube.get_root_stnv().size()) 
	  error(cubeparserloc,"Machine of the topology coordinates wasn't declared!"); 
	if (cube.get_root_stnv().at(parseContext.nodeid)==NULL) 
	  error(cubeparserloc,"Machine of the topology coordinates wasn't declared!"); 
	cube.def_coords(parseContext.currentCart,
                         (cube::Sysres*)(cube.get_root_stnv().at(parseContext.machid)),
                         parseContext.longValues);
}
    break;

  case 242:
/* Line 670 of lalr1.cc  */
#line 2260 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {if (parseContext.dynamicMetricLoading) return 0;}
    break;

  case 246:
/* Line 670 of lalr1.cc  */
#line 2270 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.metricidSeen = false;

	if ((int)parseContext.metricVec.size()<=parseContext.metricid)
	  error(cubeparserloc,"Metric of the severity matrix wasn't declared!"); 
	if (parseContext.metricVec[parseContext.metricid]==NULL)
	  error(cubeparserloc,"Metric of the severity matrix wasn't declared!");
	parseContext.currentMetric = 
		parseContext.metricVec[parseContext.metricid];

	parseContext.ignoreMetric = false;
        cube::Metric* metric = parseContext.currentMetric;
        while (metric!=NULL) {
    		if (metric->get_val() == "VOID") {
      		  parseContext.ignoreMetric = true;
      		  break;
    		}
    		metric = metric->get_parent();
  	}
}
    break;

  case 252:
/* Line 670 of lalr1.cc  */
#line 2303 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	//check for attributes
	parseContext.cnodeidSeen = false;

	if (!parseContext.ignoreMetric){
		if ((int)parseContext.cnodeVec.size()<=parseContext.cnodeid)
		  error(cubeparserloc,"Cnode of the severity row wasn't declared!");
		if (parseContext.cnodeVec[parseContext.cnodeid]==NULL)
		  error(cubeparserloc,"Cnode of the severity row wasn't declared!"); 
		parseContext.currentCnode = 
			parseContext.cnodeVec[parseContext.cnodeid];
	}
}
    break;

  case 253:
/* Line 670 of lalr1.cc  */
#line 2315 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
    {
	if (!parseContext.ignoreMetric)
    {
	  	std::vector<double> & v = parseContext.realValues;
		std::vector<cube::Thread*> & t = parseContext.threadVec;
        if (v.size()>t.size())	
                error(cubeparserloc,"Too many values in severity row!"); 
        std::vector<cube::Thread*> t_sorted = t;
        sort(t_sorted.begin(), t_sorted.end(), &cube::IdentObject::id_compare);

    
		cube::Metric* metric = parseContext.currentMetric;
		cube::Cnode * cnode = parseContext.currentCnode;
        for (unsigned i=0; i<v.size(); ++i)
        {
            cube.set_sev(metric,
                            cnode,
                            t_sorted[i],
                            v[i]);
        }
	}
}
    break;


/* Line 670 of lalr1.cc  */
#line 2436 "../../build-backend/../src/cube/src/syntax/Cube4Parser.cpp"
      default:
        break;
      }

    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */
        if (yychar <= yyeof_)
          {
            /* Return failure if at end of input.  */
            if (yychar == yyeof_)
              YYABORT;
          }
        else
          {
            yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
            yychar = yyempty_;
          }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	  YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystate_stack_.height ())
      {
        yydestruct_ ("Cleanup: popping",
                     yystos_[yystate_stack_[0]],
                     &yysemantic_stack_[0],
                     &yylocation_stack_[0]);
        yypop_ ();
      }

    return yyresult;
    }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (yychar != yyempty_)
          {
            /* Make sure we have latest lookahead translation.  See
               comments at user semantic actions for why this is
               necessary.  */
            yytoken = yytranslate_ (yychar);
            yydestruct_ (YY_NULL, yytoken, &yylval, &yylloc);
          }

        while (1 < yystate_stack_.height ())
          {
            yydestruct_ (YY_NULL,
                         yystos_[yystate_stack_[0]],
                         &yysemantic_stack_[0],
                         &yylocation_stack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  // Generate an error message.
  std::string
  Cube4Parser::yysyntax_error_ (int yystate, int yytoken)
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yychar.
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULL;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const short int Cube4Parser::yypact_ninf_ = -385;
  const short int
  Cube4Parser::yypact_[] =
  {
         6,     1,    32,   106,    33,    40,  -385,  -385,    -8,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,  -385,    31,  -385,    -1,
    -385,  -385,  -385,  -385,  -385,   -81,    16,  -385,    56,    -1,
    -385,    18,    13,    83,   101,  -385,  -385,    -5,  -385,    47,
      72,  -385,    35,  -385,  -385,    18,    13,    13,    37,    44,
    -385,  -385,  -385,  -385,    77,  -385,    81,    47,  -385,   144,
     159,    13,    44,    44,   164,   161,    61,    73,  -385,  -385,
    -385,  -385,   148,    44,    73,    73,  -385,   141,   170,   167,
    -385,  -385,  -385,    64,     2,  -385,    73,  -385,  -385,    15,
      38,  -385,  -385,     3,  -385,   173,  -385,   -22,  -385,  -385,
    -385,  -385,  -385,   174,   175,   176,  -385,  -385,  -385,  -385,
       4,  -385,   -24,  -385,   -21,  -385,    64,    64,    84,   145,
    -385,   143,  -385,   -39,  -385,   179,   182,  -385,  -385,   180,
     -22,  -385,  -385,  -385,    -3,  -385,   183,   185,  -385,  -385,
    -385,  -385,  -385,    -2,    85,  -385,   156,  -385,   184,   186,
    -385,    98,  -385,  -385,  -385,    67,  -385,  -385,  -385,  -385,
      71,  -385,    93,   119,   120,   117,   121,   113,   112,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,   171,    -3,  -385,  -385,
    -385,  -385,  -385,   198,    99,  -385,     7,   122,   122,   153,
    -385,   205,   203,    80,   187,    86,   146,   147,   149,   150,
     139,  -385,  -385,  -385,  -385,  -385,  -385,  -385,  -385,  -385,
    -385,  -385,    71,  -385,  -385,   107,  -385,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,   213,   114,  -385,  -385,  -385,
     132,   151,  -385,   125,   153,  -385,   -50,  -385,  -385,   223,
     218,  -385,   127,   219,  -385,  -385,  -385,  -385,  -385,  -385,
    -385,  -385,   221,  -385,   224,   222,   142,   151,  -385,  -385,
    -385,  -385,   228,   230,   126,  -385,   227,   188,  -385,   210,
    -385,  -385,   207,  -385,  -385,  -385,  -385,  -385,   194,  -385,
    -385,  -385,   234,   108,   195,   188,  -385,  -385,  -385,   237,
      64,    92,  -385,   -72,     5,  -385,   245,   242,  -385,  -385,
    -385,  -385,   243,  -385,  -385,   248,   249,   155,  -385,  -385,
    -385,    -7,  -385,  -385,  -385,  -385,  -385,    36,   122,  -385,
    -385,  -385,  -385,  -385,  -385,    29,   201,    64,   145,   220,
    -385,   151,   251,   252,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   104,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,   266,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,   206,   208,   209,   225,
     226,   229,   231,  -385,    29,   122,   232,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,   168,  -385,  -385,    64,   116,
    -385,   178,   192,   236,   269,  -385,  -385,  -385,   196,   237,
      64,   124,  -385,   -16,  -385,   237,  -385,   271,  -385,  -385,
    -385,  -385,   -16,  -385,  -385,   122,   239,  -385,   244,    64,
     118,  -385,  -385,   272,  -385,  -385,   -16,   -30,  -385
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Cube4Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,    69,    70,     0,    67,
       1,    72,    73,    74,    75,    76,    77,     0,    65,     0,
       2,     3,    66,    68,    78,     0,    91,    98,     0,     0,
      83,     0,     0,     0,     0,    88,    89,     0,    86,    94,
       0,    92,    14,    71,    84,     0,     0,     0,    16,     0,
       4,     5,    85,    87,     0,    96,     0,    95,    90,     0,
       0,     0,     0,     0,     0,     0,    18,   240,    44,    93,
      97,    15,     0,     0,   240,   240,    17,     0,     0,     0,
     242,    82,   241,   104,     0,   100,   240,    80,    81,     0,
       0,   128,    19,     0,   244,     0,   105,   106,   126,    99,
     101,   102,    79,     0,     0,     0,   145,   144,   146,   147,
       0,   142,     0,   129,   152,   148,     0,     0,   203,   164,
     165,   163,   184,     0,    26,     0,     0,   108,   109,     0,
     106,     6,    37,    38,   131,   143,     0,     0,   158,   156,
     159,   157,   160,     0,     0,   127,   152,   154,     0,     0,
     205,     0,   204,   166,   185,     0,   243,   245,     7,     8,
     110,   107,     0,     0,     0,     0,     0,     0,     0,   139,
     140,   135,   136,   137,   138,   141,     0,   132,   133,    39,
      27,   151,   161,     0,     0,   153,     0,     0,     0,   207,
     162,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   118,   119,   121,   122,   123,
     120,   103,   111,   112,   124,     0,    50,    56,    59,    60,
      57,    58,   130,   134,     9,     0,     0,   150,   155,   149,
       0,    51,   215,     0,   208,   209,     0,    35,   246,     0,
       0,    54,     0,     0,    22,    23,    45,    46,    47,    48,
      49,   113,     0,    10,     0,     0,     0,    51,   186,    52,
     206,   210,     0,     0,     0,   211,     0,   248,    21,     0,
      24,    25,     0,   125,    11,    64,    61,   169,     0,    12,
      40,   212,     0,     0,     0,   249,   250,    53,    55,   167,
       0,     0,   188,     0,     0,   216,     0,     0,   247,   251,
     170,   173,     0,   187,   189,     0,     0,     0,   221,   222,
     223,     0,   219,   213,   217,    36,   252,   171,     0,    13,
      41,    42,    43,   218,   220,     0,     0,     0,   172,     0,
     174,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   230,   231,
     232,   226,   227,   228,   229,   253,     0,   168,   190,    28,
      29,    30,    31,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,   214,     0,     0,     0,   233,   234,   235,
     236,   237,   238,   239,   225,     0,   175,   177,     0,     0,
     192,     0,     0,     0,     0,   191,   193,    63,     0,   179,
       0,     0,   181,     0,    62,   179,   178,     0,   176,   182,
     198,   199,   194,   196,   180,     0,     0,   197,     0,     0,
       0,   200,   183,     0,   195,   201,     0,     0,   202
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Cube4Parser::yypgoto_[] =
  {
      -385,  -385,  -385,   129,    78,   -96,  -385,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,  -385,  -385,  -385,  -385,
     -83,  -385,  -385,  -385,  -385,  -385,  -385,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,    28,  -385,  -385,   238,  -385,  -385,
    -385,  -385,  -385,  -146,  -246,  -385,  -385,  -385,  -125,  -133,
    -385,  -385,  -385,  -385,  -385,   -91,  -385,  -385,  -385,  -385,
     288,  -385,  -385,  -385,  -385,   268,  -385,   267,   270,  -385,
    -385,  -385,  -385,    -6,  -385,  -385,   214,  -385,   172,  -385,
    -385,  -385,    88,  -156,  -385,   -25,  -385,   211,  -385,  -385,
     130,  -385,   193,  -385,   123,   160,  -385,   165,  -385,   -12,
    -385,    -4,  -117,  -385,  -385,  -385,  -385,  -385,  -385,  -105,
    -385,   -94,  -385,   -89,  -385,   197,  -385,  -385,    23,  -385,
    -385,   -74,  -385,  -110,  -384,  -385,  -101,  -385,  -385,  -385,
    -385,  -385,  -385,    87,  -385,  -385,  -385,    26,  -385,    11,
    -385,   -51,  -385,  -385,  -385,  -385,  -385,  -385,  -385,    17,
    -385,  -385,  -385,  -385,  -385,  -385,  -385,    39,  -385
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Cube4Parser::yydefgoto_[] =
  {
        -1,     6,     7,    35,    36,   106,   127,   128,   184,   226,
     255,   264,   308,    60,    65,    79,   240,   243,   244,   245,
     107,   140,   339,   340,   341,   342,   343,   344,   345,   192,
     297,   108,   109,   141,   265,   309,   310,    55,   201,   202,
     203,   204,   205,   169,   258,   207,   208,   209,   259,   410,
     172,   173,   174,   257,   399,   411,   146,     2,     3,     8,
       9,    18,    19,    28,    29,    30,    37,    38,    31,    40,
      41,    56,    57,    32,    42,    84,    85,    97,   129,   130,
     211,   212,   213,   175,   101,    49,    90,    91,   176,   177,
     178,   110,   111,   114,   115,   147,   186,   142,   143,    67,
     118,   119,   120,   301,   289,   329,   317,   330,   393,   386,
     387,   406,   401,   402,   121,   122,   278,   291,   292,   376,
     389,   390,   416,   412,   413,   420,   421,   151,   152,   189,
     233,   234,   266,   235,   325,   236,   294,   295,   311,   312,
     346,   347,   348,   349,   350,   351,   352,   353,   354,    81,
      82,    94,   123,   157,   267,   284,   285,   286,   326
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Cube4Parser::yytable_ninf_ = -1;
  const unsigned short int
  Cube4Parser::yytable_[] =
  {
        96,   171,   153,   323,   214,    52,    22,   181,   162,   170,
      25,   277,   155,   134,   206,   144,   138,   293,   162,     1,
     428,    62,    63,    46,    33,    47,    83,    98,   417,   139,
     228,    34,    10,   148,   149,   210,    73,   305,    20,    61,
     116,   112,   227,   417,   171,    21,   117,   138,   306,   307,
      74,    75,   170,    24,   230,   231,   214,   167,    26,   262,
     139,    86,   313,   156,    27,   145,   206,   391,    99,   263,
      89,   167,   112,   116,   163,   327,   164,    39,   165,    43,
     166,   391,   162,    27,   167,   358,   168,   210,    50,    95,
     103,    87,    88,   136,   137,     4,     5,   193,    48,   194,
      33,   195,   305,   102,     4,     5,    51,    34,   125,   126,
      54,    95,   103,   306,   307,   136,   137,    95,   103,   104,
     105,    11,    12,    13,    14,    15,    16,    17,    95,   103,
     104,   105,    58,   300,   270,   271,   303,   290,   196,    66,
     197,    68,   198,    69,   199,    59,   200,    64,   163,    71,
     164,   332,   333,   334,   335,   336,   337,   338,   321,   322,
     373,   374,   395,   388,   408,   400,   424,   419,    72,    76,
      77,    78,    83,    89,    80,    92,    93,    95,   124,   131,
     132,   133,   116,   150,   158,   331,   117,   159,   179,   160,
     180,   183,   144,   187,   190,   188,   191,   216,    33,   218,
     217,   220,   221,   224,   222,   219,   225,   302,   232,   167,
     237,   153,   238,   239,   246,   250,   241,   247,   253,    34,
     242,   248,   254,   256,   249,   260,   268,   269,   272,   274,
     164,   273,   275,   279,   276,   280,   282,   287,   288,   290,
     296,   283,   385,   405,   356,   263,   293,   298,   162,   405,
     315,   316,   318,   319,   320,   355,   359,   360,   357,   361,
     362,   363,   364,   365,   377,   391,   378,   379,   366,   367,
     368,   369,   370,   371,   372,   375,   397,   400,   403,   388,
     415,   426,   385,   380,   381,   398,   422,   382,   419,   383,
     404,   215,   281,   252,   392,    70,    23,    44,   100,    45,
     251,   113,   161,   135,    53,   394,   185,   223,   182,   229,
     418,   414,   409,   328,   304,   396,   427,   407,   154,   425,
     314,   261,   324,   384,   299,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   423
  };

  /* YYCHECK.  */
  const short int
  Cube4Parser::yycheck_[] =
  {
        83,   134,   119,    10,   160,    10,    14,     9,    11,   134,
      11,   257,    51,     9,   160,    36,   112,    12,    11,    13,
      50,    46,    47,    29,   105,    31,    24,    25,   412,   112,
     186,   112,     0,   116,   117,   160,    61,   109,     5,    45,
      37,    34,    35,   427,   177,     5,    43,   143,   120,   121,
      62,    63,   177,    22,   187,   188,   212,    87,    59,   109,
     143,    73,    57,   102,    65,    86,   212,    97,    66,   119,
      32,    87,    34,    37,    77,    39,    79,    61,    81,    23,
      83,    97,    11,    65,    87,   331,    89,   212,     5,   113,
     114,    74,    75,   117,   118,   103,   104,    26,    85,    28,
     105,    30,   109,    86,   103,   104,     5,   112,   130,   131,
      63,   113,   114,   120,   121,   117,   118,   113,   114,   115,
     116,    15,    16,    17,    18,    19,    20,    21,   113,   114,
     115,   116,    60,   289,     7,     8,    44,    45,    67,    95,
      69,    64,    71,    62,    73,   110,    75,   110,    77,     5,
      79,   122,   123,   124,   125,   126,   127,   128,     3,     4,
      56,    57,    46,    47,    40,    41,    48,    49,     9,     5,
       9,   110,    24,    32,   101,     5,     9,   113,     5,     5,
       5,     5,    37,    99,     5,   318,    43,     5,     5,     9,
       5,   106,    36,     9,    96,     9,   129,    78,   105,    82,
      80,    88,    90,     5,    33,    84,   107,   290,    55,    87,
       5,   328,     9,   133,    68,    76,    29,    70,     5,   112,
     134,    72,   108,    91,    74,   100,     3,     9,     9,     5,
      79,    10,    10,     5,    92,     5,     9,    27,    31,    45,
     132,    53,   375,   399,   327,   119,    12,    52,    11,   405,
       5,     9,     9,     5,     5,    54,     5,     5,    38,     5,
       5,     5,     5,     5,    58,    97,    58,    58,     9,     9,
       9,     9,     9,     9,     9,     9,    98,    41,     9,    47,
       9,     9,   415,    58,    58,    93,    42,    58,    49,    58,
      94,   162,   264,   215,   385,    57,     8,    29,    84,    29,
     212,    90,   130,   110,    37,   388,   146,   177,   143,   186,
     415,   405,   401,   317,   291,   389,   426,   400,   121,   420,
     294,   234,   311,   374,   285,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   419
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned short int
  Cube4Parser::yystos_[] =
  {
         0,    13,   192,   193,   103,   104,   136,   137,   194,   195,
       0,    15,    16,    17,    18,    19,    20,    21,   196,   197,
       5,     5,    14,   195,    22,    11,    59,    65,   198,   199,
     200,   203,   208,   105,   112,   138,   139,   201,   202,    61,
     204,   205,   209,    23,   200,   203,   208,   208,    85,   220,
       5,     5,    10,   202,    63,   172,   206,   207,    60,   110,
     148,   208,   220,   220,   110,   149,    95,   234,    64,    62,
     172,     5,     9,   220,   234,   234,     5,     9,   110,   150,
     101,   284,   285,    24,   210,   211,   234,   284,   284,    32,
     221,   222,     5,     9,   286,   113,   155,   212,    25,    66,
     211,   219,   284,   114,   115,   116,   140,   155,   166,   167,
     226,   227,    34,   222,   228,   229,    37,    43,   235,   236,
     237,   249,   250,   287,     5,   130,   131,   141,   142,   213,
     214,     5,     5,     5,     9,   227,   117,   118,   140,   155,
     156,   168,   232,   233,    36,    86,   191,   230,   155,   155,
      99,   262,   263,   237,   250,    51,   102,   288,     5,     5,
       9,   213,    11,    77,    79,    81,    83,    87,    89,   178,
     183,   184,   185,   186,   187,   218,   223,   224,   225,     5,
       5,     9,   232,   106,   143,   230,   231,     9,     9,   264,
      96,   129,   164,    26,    28,    30,    67,    69,    71,    73,
      75,   173,   174,   175,   176,   177,   178,   180,   181,   182,
     183,   215,   216,   217,   218,   138,    78,    80,    82,    84,
      88,    90,    33,   225,     5,   107,   144,    35,   218,   229,
     184,   184,    55,   265,   266,   268,   270,     5,     9,   133,
     151,    29,   134,   152,   153,   154,    68,    70,    72,    74,
      76,   217,   139,     5,   108,   145,    91,   188,   179,   183,
     100,   268,   109,   119,   146,   169,   267,   289,     3,     9,
       7,     8,     9,    10,     5,    10,    92,   179,   251,     5,
       5,   169,     9,    53,   290,   291,   292,    27,    31,   239,
      45,   252,   253,    12,   271,   272,   132,   165,    52,   292,
     218,   238,   155,    44,   253,   109,   120,   121,   147,   170,
     171,   273,   274,    57,   272,     5,     9,   241,     9,     5,
       5,     3,     4,    10,   274,   269,   293,    39,   236,   240,
     242,   184,   122,   123,   124,   125,   126,   127,   128,   157,
     158,   159,   160,   161,   162,   163,   275,   276,   277,   278,
     279,   280,   281,   282,   283,    54,   155,    38,   179,     5,
       5,     5,     5,     5,     5,     5,     9,     9,     9,     9,
       9,     9,     9,    56,    57,     9,   254,    58,    58,    58,
      58,    58,    58,    58,   276,   184,   244,   245,    47,   255,
     256,    97,   190,   243,   155,    46,   256,    98,    93,   189,
      41,   247,   248,     9,    94,   218,   246,   155,    40,   248,
     184,   190,   258,   259,   246,     9,   257,   259,   244,    49,
     260,   261,    42,   155,    48,   261,     9,   258,    50
  };

#if CUBEPARSERDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Cube4Parser::yytoken_number_[] =
  {
         0,   256,  1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,
    1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,
    1019,  1020,  1000,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,
    1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,
    1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,
    1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,
    1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,
    1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,
    1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,
    1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,
    1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,
    1128,  1129,  1130,  1131,  1132
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned short int
  Cube4Parser::yyr1_[] =
  {
         0,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   148,   149,   149,   150,   150,
     151,   151,   152,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   194,   195,
     195,   196,   197,   197,   197,   197,   197,   197,   197,   198,
     198,   198,   198,   199,   199,   200,   201,   201,   202,   202,
     203,   204,   204,   205,   206,   206,   207,   207,   209,   208,
     210,   210,   210,   211,   212,   212,   213,   213,   214,   214,
     215,   215,   216,   216,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   218,   219,   220,   221,   221,
     222,   223,   223,   224,   224,   225,   225,   225,   225,   225,
     225,   225,   226,   226,   227,   227,   227,   227,   228,   228,
     228,   229,   230,   230,   231,   231,   232,   232,   232,   232,
     233,   233,   234,   235,   235,   236,   236,   238,   237,   239,
     239,   240,   240,   241,   241,   243,   242,   244,   245,   246,
     246,   247,   247,   248,   249,   249,   251,   250,   252,   252,
     254,   253,   255,   255,   257,   256,   258,   258,   259,   259,
     260,   260,   261,   262,   262,   264,   263,   265,   265,   266,
     266,   267,   267,   269,   268,   270,   271,   271,   272,   273,
     273,   274,   274,   274,   275,   275,   276,   276,   276,   276,
     276,   276,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   284,   286,   285,   287,   287,   289,   288,   290,   290,
     291,   291,   293,   292
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Cube4Parser::yyr2_[] =
  {
         0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     2,     0,     2,     0,     2,
       0,     2,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     0,     1,     4,     2,     4,     2,     2,     2,     2,
       2,     2,     2,     2,     5,     2,     3,     1,     2,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     6,
       5,     5,     4,     1,     2,     3,     1,     2,     1,     1,
       3,     0,     1,     3,     0,     1,     1,     2,     0,     6,
       1,     2,     2,     5,     0,     1,     0,     2,     1,     1,
       0,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     6,     1,     2,
       5,     0,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     4,
       4,     3,     0,     2,     0,     2,     1,     1,     1,     1,
       1,     2,     6,     1,     1,     1,     2,     0,    11,     0,
       2,     0,     1,     0,     2,     0,     7,     1,     4,     0,
       2,     1,     2,     5,     1,     2,     0,     8,     1,     2,
       0,     8,     1,     2,     0,     7,     1,     2,     1,     1,
       1,     2,     5,     0,     1,     0,     4,     0,     1,     1,
       2,     1,     2,     0,     8,     1,     1,     2,     3,     1,
       2,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       0,     1,     0,     4,     0,     2,     0,     6,     0,     1,
       1,     2,     0,     5
  };


  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Cube4Parser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"false\"", "\"true\"",
  "\"attribute value\"", "\"error\"", "\"plus\"", "\"minus\"", "\">\"",
  "\"/>\"", "\"<attr\"", "\"<dim\"", "\"<?xml\"", "\"?>\"",
  "\"<cube version=\\\"4.0\\\"\"", "\"<cube version=\\\"4.1\\\"\"",
  "\"<cube version=\\\"4.2\\\"\"", "\"<cube version=\\\"4.3\\\"\"",
  "\"<cube version=\\\"4.4\\\"\"", "\"<cube version=\\\"3.0\\\"\"",
  "\"<cube version=\\\"\"", "CUBE_OPEN_NOT_SUPPORTED", "\"</cube>\"",
  "\"<metric\"", "\"</metric>\"", "\"<cubepl\"", "\"</cubepl>\"",
  "\"<cubeplinit>\"", "\"</cubeplinit>\"", "\"<cubeplaggr\"",
  "\"</cubeplaggr>\"", "\"<region\"", "\"</region>\"", "\"<cnode\"",
  "\"</cnode>\"", "\"<parameter\"", "\"<systemtreenode\"",
  "\"</systemtreenode>\"", "\"<locationgroup\"", "\"</locationgroup>\"",
  "\"<location\"", "\"</location>\"", "\"<machine\"", "\"</machine>\"",
  "\"<node\"", "\"</node>\"", "\"<process\"", "\"</process>\"",
  "\"<thread\"", "\"</thread>\"", "\"<matrix\"", "\"</matrix>\"",
  "\"<row\"", "\"</row>\"", "\"<cart\"", "\"</cart>\"", "\"<coord\"",
  "\"</coord>\"", "\"<doc>\"", "\"</doc>\"", "\"<mirrors>\"",
  "\"</mirrors>\"", "\"<murl>\"", "\"</murl>\"", "\"<metrics\"",
  "\"</metrics>\"", "\"<disp_name>\"", "\"</disp_name>\"",
  "\"<uniq_name>\"", "\"</uniq_name>\"", "\"<dtype>\"", "\"</dtype>\"",
  "\"<uom>\"", "\"</uom>\"", "\"<val>\"", "\"</val>\"", "\"<url>\"",
  "\"</url>\"", "\"<descr>\"", "\"</descr>\"", "\"<paradigm>\"",
  "\"</paradigm>\"", "\"<role>\"", "\"</role>\"", "\"<program\"",
  "\"</program>\"", "\"<name>\"", "\"</name>\"", "\"<mangled_name>\"",
  "\"</mangled_name>\"", "\"<class>\"", "\"</class>\"", "\"<type>\"",
  "\"</type>\"", "\"<system\"", "\"</system>\"", "\"<rank>\"",
  "\"</rank>\"", "\"<topologies>\"", "\"</topologies>\"", "\"<severity>\"",
  "\"</severity>\"", "\"attribute name version\"",
  "\"attribute name encoding\"", "\"attribute name key\"",
  "\"attribute name partype\"", "\"attribute name parkey\"",
  "\"attribute name parvalue\"", "\"attribute name name\"",
  "\"attribute name title\"", "\"attribute name file\"",
  "\"attribute name value\"", "\"attribute name id\"",
  "\"attribute name mod\"", "\"attribute name begin\"",
  "\"attribute name end\"", "\"attribute name line\"",
  "\"attribute name calleeid\"", "\"attribute name ndims\"",
  "\"attribute name size\"", "\"attribute name periodic\"",
  "\"attribute name locId\"", "\"attribute name lgId\"",
  "\"attribute name stnId\"", "\"attribute name thrdId\"",
  "\"attribute name procId\"", "\"attribute name nodeId\"",
  "\"attribute name machId\"", "\"attribute name metricId\"",
  "\"attribute name type\"", "\"attribute name viztype\"",
  "\"attribute name cnodeId\"", "\"attribute name rowwise\"",
  "\"attribute name cubeplaggrtype\"", "$accept", "version_attr",
  "encoding_attr", "key_attr", "value_attr", "mod_attr",
  "metric_type_attr", "metric_viz_type_attr", "cnode_par_type_attr",
  "cnode_par_key_attr", "cnode_par_value_attr", "cart_name_attr",
  "dim_name_attr", "metrics_title_attr", "calltree_title_attr",
  "systemtree_title_attr", "expression_attr", "expression_aggr_attr",
  "expression_aggr_attr_plus", "expression_aggr_attr_minus", "id_attr",
  "calleeid_attr", "locid_attr", "lgid_attr", "stnid_attr", "thrdid_attr",
  "procid_attr", "nodeid_attr", "machid_attr", "metricid_attr",
  "cnodeid_attr", "begin_attr", "end_attr", "line_attr", "ndims_attr",
  "size_attr", "periodic_attr", "murl_tag", "disp_name_tag",
  "uniq_name_tag", "dtype_tag", "uom_tag", "val_tag", "url_tag",
  "descr_tag_opt", "expression_tag", "expression_init_tag",
  "expression_aggr_tag", "descr_tag", "name_tag", "mangled_name_tag",
  "paradigm_tag", "role_tag", "class_tag", "type_tag", "rank_tag",
  "parameter_tag", "document", "xml_tag", "xml_attributes",
  "xml_attribute", "cube_tag", "cube_begin", "cube_content", "attr_tags",
  "attr_tag", "attr_attributes", "attr_attribute", "doc_tag",
  "mirrors_tag_attr", "mirrors_tag", "murl_tags_attr", "murl_tags",
  "metrics_tag", "$@1", "metric_tag", "metric_begin", "id_attrs",
  "metric_attrs", "metric_attr", "tags_of_metric_attr", "tags_of_metric",
  "tag_of_metric", "generic_attr_tag", "metric_end", "program_tag",
  "region_tags", "region_tag", "tags_of_region_attr", "tags_of_region",
  "tag_of_region", "region_attributes", "region_attribute", "cnode_tag",
  "cnode_begin", "cnode_parameters", "cnode_attr_tags", "cnode_attribute",
  "cnode_attributes", "system_tag", "systemtree_tags",
  "flexsystemtree_tags", "systemtree_tag", "$@2", "systree_attr_tags",
  "systemtree_sub_tags", "location_group_tags", "location_group_tag",
  "$@3", "loc_tags", "loc_tag", "lg_attr_tags", "location_tags",
  "location_tag", "machine_tags", "machine_tag", "$@4", "node_tags",
  "node_tag", "$@5", "process_tags", "process_tag", "$@6",
  "tags_of_process", "tag_of_process", "thread_tags", "thread_tag",
  "topologies_tag_attr", "topologies_tag", "$@7", "cart_tags_attr",
  "cart_tags", "cart_attrs", "cart_tag", "$@8", "cart_open", "dim_tags",
  "dim_tag", "dim_attributes", "dim_attribute", "coord_tags", "coord_tag",
  "coord_tag_loc", "coord_tag_lg", "coord_tag_stn", "coord_tag_thrd",
  "coord_tag_proc", "coord_tag_node", "coord_tag_mach", "severity_tag",
  "severity_part", "$@9", "matrix_tags", "matrix_tag", "$@10",
  "row_tags_attr", "row_tags", "row_tag", "$@11", YY_NULL
  };

#if CUBEPARSERDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Cube4Parser::rhs_number_type
  Cube4Parser::yyrhs_[] =
  {
       192,     0,    -1,   103,     5,    -1,   104,     5,    -1,   105,
       5,    -1,   112,     5,    -1,   114,     5,    -1,   130,     5,
      -1,   131,     5,    -1,   106,     5,    -1,   107,     5,    -1,
     108,     5,    -1,   109,     5,    -1,   109,     5,    -1,    -1,
     110,     5,    -1,    -1,   110,     5,    -1,    -1,   110,     5,
      -1,    -1,   133,     3,    -1,   153,    -1,   154,    -1,   134,
       7,    -1,   134,     8,    -1,   113,     5,    -1,   118,     5,
      -1,   122,     5,    -1,   123,     5,    -1,   124,     5,    -1,
     125,     5,    -1,   126,     5,    -1,   127,     5,    -1,   128,
       5,    -1,   129,     5,    -1,   132,     5,    -1,   115,     5,
      -1,   116,     5,    -1,   117,     5,    -1,   119,     5,    -1,
     120,     5,    -1,   121,     3,    -1,   121,     4,    -1,    63,
      64,    -1,    67,    68,    -1,    69,    70,    -1,    71,    72,
      -1,    73,    74,    -1,    75,    76,    -1,    77,    78,    -1,
      -1,   183,    -1,    26,   151,     9,    27,    -1,    28,    29,
      -1,    30,   152,     9,    31,    -1,    79,    80,    -1,    87,
      88,    -1,    89,    90,    -1,    81,    82,    -1,    83,    84,
      -1,    91,    92,    -1,    93,    94,    -1,    97,    98,    -1,
      36,   143,   144,   145,    10,    -1,   193,   196,    -1,    13,
     194,    14,    -1,   195,    -1,   194,   195,    -1,   136,    -1,
     137,    -1,   197,   198,    23,    -1,    15,    -1,    16,    -1,
      17,    -1,    18,    -1,    19,    -1,    20,    -1,    21,    22,
      -1,   199,   203,   208,   220,   234,   284,    -1,   199,   208,
     220,   234,   284,    -1,   203,   208,   220,   234,   284,    -1,
     208,   220,   234,   284,    -1,   200,    -1,   199,   200,    -1,
      11,   201,    10,    -1,   202,    -1,   201,   202,    -1,   138,
      -1,   139,    -1,    59,   204,    60,    -1,    -1,   205,    -1,
      61,   206,    62,    -1,    -1,   207,    -1,   172,    -1,   207,
     172,    -1,    -1,    65,   209,   148,     9,   210,    66,    -1,
     211,    -1,   210,   211,    -1,   210,   219,    -1,    24,   212,
     213,     9,   215,    -1,    -1,   155,    -1,    -1,   214,   213,
      -1,   141,    -1,   142,    -1,    -1,   216,    -1,   217,    -1,
     216,   217,    -1,   173,    -1,   174,    -1,   175,    -1,   176,
      -1,   177,    -1,   178,    -1,   183,    -1,   180,    -1,   181,
      -1,   182,    -1,   218,    -1,    11,   138,   139,    10,    -1,
      25,    -1,    85,   149,     9,   221,   228,    86,    -1,   222,
      -1,   221,   222,    -1,    32,   226,     9,   223,    33,    -1,
      -1,   224,    -1,   225,    -1,   224,   225,    -1,   184,    -1,
     185,    -1,   186,    -1,   187,    -1,   178,    -1,   183,    -1,
     218,    -1,   227,    -1,   226,   227,    -1,   155,    -1,   140,
      -1,   166,    -1,   167,    -1,   229,    -1,   228,   230,   231,
     229,    -1,   228,   230,   231,    35,    -1,    34,   233,     9,
      -1,    -1,   191,   230,    -1,    -1,   231,   218,    -1,   155,
      -1,   168,    -1,   140,    -1,   156,    -1,   232,    -1,   233,
     232,    -1,    95,   150,     9,   235,   262,    96,    -1,   249,
      -1,   236,    -1,   237,    -1,   236,   237,    -1,    -1,    37,
     155,     9,   184,   188,   179,   239,   238,   241,   240,    38,
      -1,    -1,   239,   218,    -1,    -1,   236,    -1,    -1,   241,
     242,    -1,    -1,    39,   155,     9,   244,   243,   247,    40,
      -1,   245,    -1,   184,   190,   189,   246,    -1,    -1,   218,
     246,    -1,   248,    -1,   247,   248,    -1,    41,   155,     9,
     244,    42,    -1,   250,    -1,   249,   250,    -1,    -1,    43,
     155,     9,   184,   179,   251,   252,    44,    -1,   253,    -1,
     252,   253,    -1,    -1,    45,   155,     9,   184,   179,   254,
     255,    46,    -1,   256,    -1,   255,   256,    -1,    -1,    47,
     155,     9,   258,   257,   260,    48,    -1,   259,    -1,   258,
     259,    -1,   184,    -1,   190,    -1,   261,    -1,   260,   261,
      -1,    49,   155,     9,   258,    50,    -1,    -1,   263,    -1,
      -1,    99,   264,   265,   100,    -1,    -1,   266,    -1,   268,
      -1,   266,   268,    -1,   169,    -1,   146,   169,    -1,    -1,
     270,   267,     9,   271,    57,   269,   275,    56,    -1,    55,
      -1,   272,    -1,   271,   272,    -1,    12,   273,    10,    -1,
     274,    -1,   273,   274,    -1,   147,    -1,   170,    -1,   171,
      -1,   276,    -1,   275,    57,   276,    -1,   280,    -1,   281,
      -1,   282,    -1,   283,    -1,   277,    -1,   278,    -1,   279,
      -1,   157,     9,    58,    -1,   158,     9,    58,    -1,   159,
       9,    58,    -1,   160,     9,    58,    -1,   161,     9,    58,
      -1,   162,     9,    58,    -1,   163,     9,    58,    -1,    -1,
     285,    -1,    -1,   101,   286,   287,   102,    -1,    -1,   287,
     288,    -1,    -1,    51,   164,     9,   289,   290,    52,    -1,
      -1,   291,    -1,   292,    -1,   291,   292,    -1,    -1,    53,
     165,     9,   293,    54,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Cube4Parser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    12,    15,    18,    21,    24,
      27,    30,    33,    36,    39,    40,    43,    44,    47,    48,
      51,    52,    55,    57,    59,    62,    65,    68,    71,    74,
      77,    80,    83,    86,    89,    92,    95,    98,   101,   104,
     107,   110,   113,   116,   119,   122,   125,   128,   131,   134,
     137,   140,   141,   143,   148,   151,   156,   159,   162,   165,
     168,   171,   174,   177,   180,   186,   189,   193,   195,   198,
     200,   202,   206,   208,   210,   212,   214,   216,   218,   221,
     228,   234,   240,   245,   247,   250,   254,   256,   259,   261,
     263,   267,   268,   270,   274,   275,   277,   279,   282,   283,
     290,   292,   295,   298,   304,   305,   307,   308,   311,   313,
     315,   316,   318,   320,   323,   325,   327,   329,   331,   333,
     335,   337,   339,   341,   343,   345,   350,   352,   359,   361,
     364,   370,   371,   373,   375,   378,   380,   382,   384,   386,
     388,   390,   392,   394,   397,   399,   401,   403,   405,   407,
     412,   417,   421,   422,   425,   426,   429,   431,   433,   435,
     437,   439,   442,   449,   451,   453,   455,   458,   459,   471,
     472,   475,   476,   478,   479,   482,   483,   491,   493,   498,
     499,   502,   504,   507,   513,   515,   518,   519,   528,   530,
     533,   534,   543,   545,   548,   549,   557,   559,   562,   564,
     566,   568,   571,   577,   578,   580,   581,   586,   587,   589,
     591,   594,   596,   599,   600,   609,   611,   613,   616,   620,
     622,   625,   627,   629,   631,   633,   637,   639,   641,   643,
     645,   647,   649,   651,   655,   659,   663,   667,   671,   675,
     679,   680,   682,   683,   688,   689,   692,   693,   700,   701,
     703,   705,   708,   709
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Cube4Parser::yyrline_[] =
  {
         0,   305,   305,   312,   319,   326,   333,   340,   348,   356,
     365,   374,   383,   393,   401,   403,   410,   412,   420,   422,
     430,   432,   442,   444,   448,   458,   472,   481,   493,   502,
     511,   521,   530,   539,   548,   557,   566,   577,   585,   593,
     601,   611,   623,   629,   640,   650,   657,   663,   670,   677,
     684,   690,   691,   696,   704,   712,   720,   727,   737,   745,
     753,   762,   771,   778,   795,   840,   846,   862,   863,   867,
     868,   874,   878,   882,   886,   890,   894,   898,   902,   908,
     909,   910,   911,   917,   918,   922,   942,   943,   947,   948,
     954,   956,   957,   961,   964,   965,   969,   970,   979,   978,
    1060,  1061,  1062,  1068,  1151,  1152,  1156,  1157,  1160,  1161,
    1164,  1165,  1169,  1170,  1174,  1175,  1176,  1177,  1178,  1179,
    1180,  1181,  1182,  1183,  1184,  1189,  1217,  1228,  1337,  1338,
    1342,  1409,  1410,  1414,  1415,  1419,  1420,  1421,  1422,  1423,
    1424,  1425,  1431,  1432,  1436,  1437,  1438,  1439,  1443,  1444,
    1445,  1568,  1625,  1626,  1631,  1633,  1638,  1639,  1640,  1641,
    1645,  1646,  1652,  1663,  1665,  1669,  1670,  1674,  1674,  1731,
    1733,  1742,  1744,  1747,  1749,  1754,  1754,  1800,  1804,  1807,
    1809,  1815,  1816,  1819,  1864,  1865,  1869,  1869,  1889,  1890,
    1894,  1894,  1914,  1915,  1919,  1919,  1947,  1948,  1951,  1952,
    1956,  1957,  1961,  1991,  1992,  1996,  1996,  2002,  2003,  2007,
    2008,  2012,  2013,  2017,  2017,  2044,  2051,  2052,  2056,  2073,
    2074,  2078,  2079,  2080,  2084,  2085,  2092,  2093,  2094,  2095,
    2096,  2097,  2098,  2105,  2126,  2147,  2169,  2190,  2211,  2233,
    2256,  2257,  2260,  2260,  2263,  2265,  2270,  2270,  2293,  2294,
    2298,  2299,  2303,  2303
  };

  // Print the state stack on the debug stream.
  void
  Cube4Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Cube4Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // CUBEPARSERDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Cube4Parser::token_number_type
  Cube4Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      22,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Cube4Parser::yyeof_ = 0;
  const int Cube4Parser::yylast_ = 336;
  const int Cube4Parser::yynnts_ = 159;
  const int Cube4Parser::yyempty_ = -2;
  const int Cube4Parser::yyfinal_ = 10;
  const int Cube4Parser::yyterror_ = 1;
  const int Cube4Parser::yyerrcode_ = 256;
  const int Cube4Parser::yyntokens_ = 135;

  const unsigned int Cube4Parser::yyuser_token_number_max_ = 1132;
  const Cube4Parser::token_number_type Cube4Parser::yyundef_token_ = 2;


} // cubeparser
/* Line 1141 of lalr1.cc  */
#line 3507 "../../build-backend/../src/cube/src/syntax/Cube4Parser.cpp"
/* Line 1142 of lalr1.cc  */
#line 2341 "../../build-backend/../src/cube/src/syntax/Cube4Parser.yy"
 /*** Additional Code ***/


void cubeparser::Cube4Parser::error(const Cube4Parser::location_type& l,
			    const std::string& m)
{
   if (strstr(m.c_str(),"expecting <?xml")!=NULL) {
     driver.error_just_message("The cube file is probably empty or filled with wrong content. The file has ended before the header of cube started. \n");
    }
   if (strstr(m.c_str()," expecting </row>")!=NULL) {
     driver.error_just_message("One of the possible reasons is \n    1) that the severity value is malformed. CUBE expects the \"double\" value in C_LOCALE with dot instead of comma;. \n    2) that the CUBE file is not properly ended. Probably the writing of CUBE file was interrupted.");
    }
   if (strstr(m.c_str()," expecting <matrix")!=NULL || 
        (strstr(m.c_str()," expecting <severity>")!=NULL) ) {
     driver.error_just_message("The cube file has probably a proper structure, but doesn't contain any severity values.");
    }
   if (strstr(m.c_str()," expecting <metric")!=NULL) {
     driver.error_just_message("The cube file doesn't contain any information about metric dimension.");
    }
   if (strstr(m.c_str()," expecting <region")!=NULL) {
     driver.error_just_message("The cube file doesn't contain any information about program dimension.");
    }
   if (strstr(m.c_str()," expecting <machine")!=NULL) {
     driver.error_just_message("The cube file doesn't contain any information about system dimension.");
    }
   if (strstr(m.c_str()," expecting <thread")!=NULL) {
     driver.error_just_message("The system dimension of the cube file is malformed. It contains a process without any threads.");
    }
   if (strstr(m.c_str()," expecting <process")!=NULL) {
     driver.error_just_message("The system dimension of the cube file is malformed. It contains a node without any processes.");
    }
   if (strstr(m.c_str()," expecting <node")!=NULL) {
     driver.error_just_message("The system dimension of the cube file is malformed. It contains a machine without any computing nodes.");
    }
    driver.error(l, m);

}




