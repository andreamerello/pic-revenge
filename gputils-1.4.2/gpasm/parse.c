/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parse.y" /* yacc.c:339  */

/* Parser for gpasm
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005
   James Bowman, Craig Franklin

This file is part of gputils.

gputils is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

gputils is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with gputils; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "stdhdr.h"

#include "libgputils.h"
#include "gpasm.h"
#include "evaluate.h"
#include "gperror.h"
#include "directive.h"
#include "lst.h"
#include "macro.h"
#include "coff.h"
#include "scan.h"

/* #define DEBUG */

#ifdef DEBUG
/* enable debugging */
#include "parse.h"

int yydebug = 1;

#define YYPRINT(file, type, value)   yyprint (file, type, value)

static void
yyprint (FILE *file, int type, YYSTYPE value)
{
  switch (type) {
  case LABEL:
  case IDENTIFIER:
  case IDENT_BRACKET:
  case DEBUG_LINE:
  case ERRORLEVEL:
  case FILL:
  case LIST:
  case PROCESSOR:
  case DEFINE:
    fprintf (file, "%s", value.s);
    break;

  case STRING:
    fprintf (file, "\"%s\"", value.s);
    break;

  case NUMBER:
  case UPPER:
  case HIGH:
  case LOW:
  case LSH:
  case RSH:
  case GREATER_EQUAL:
  case LESS_EQUAL:
  case EQUAL:
  case NOT_EQUAL:
  case '<':
  case '>':
  case '&':
  case '|':
  case '^':
  case LOGICAL_AND:
  case LOGICAL_OR:
  case '=':
  case ASSIGN_PLUS:
  case ASSIGN_MINUS:
  case ASSIGN_MULTIPLY:
  case ASSIGN_DIVIDE:
  case ASSIGN_MODULUS:
  case ASSIGN_LSH:
  case ASSIGN_RSH:
  case ASSIGN_AND:
  case ASSIGN_OR:
  case ASSIGN_XOR:
  case INCREMENT:
  case DECREMENT:
  case POSTINCREMENT:
  case POSTDECREMENT:
  case INDFOFFSET:
  case TBL_NO_CHANGE:
  case TBL_POST_INC:
  case TBL_POST_DEC:
  case TBL_PRE_INC:
  case '[':
  case ']':
    fprintf (file, "%d", value.i);
    break;

  case CBLOCK:
  case ENDC:
  default:
    break;
  }
}
#endif

int yylex(void);

/************************************************************************/

/* Some simple functions for building parse trees */

static struct pnode *mk_pnode(enum pnode_tag tag)
{
  struct pnode *new = (struct pnode *)GP_Malloc(sizeof(struct pnode));

  new->tag = tag;
  return new;
}

struct pnode *mk_constant(int value)
{
  struct pnode *new = mk_pnode(PTAG_CONSTANT);

  new->value.constant = value;
  return new;
}

struct pnode *mk_offset(struct pnode *p)
{
  struct pnode *new = mk_pnode(PTAG_OFFSET);

  new->value.offset = p;
  return new;
}

struct pnode *mk_symbol(char *value)
{
  struct pnode *new = mk_pnode(PTAG_SYMBOL);

  new->value.symbol = value;
  return new;
}

struct pnode *mk_string(char *value)
{
  struct pnode *new = mk_pnode(PTAG_STRING);

  new->value.string = value;
  return new;
}

struct pnode *mk_list(struct pnode *head, struct pnode *tail)
{
  struct pnode *new = mk_pnode(PTAG_LIST);

  new->value.list.head = head;
  new->value.list.tail = tail;
  return new;
}

struct pnode *mk_2op(int op, struct pnode *p0, struct pnode *p1)
{
  struct pnode *new = mk_pnode(PTAG_BINOP);

  new->value.binop.op = op;
  new->value.binop.p0 = p0;
  new->value.binop.p1 = p1;
  return new;
}

struct pnode *mk_1op(int op, struct pnode *p0)
{
  struct pnode *new = mk_pnode(PTAG_UNOP);

  new->value.unop.op = op;
  new->value.unop.p0 = p0;
  return new;
}

/************************************************************************/
/* shared functions */

gpasmVal set_label(char *label, struct pnode *parms)
{
  gpasmVal value = 0;

  if (asm_enabled()) {
    if ((state.mode == MODE_RELOCATABLE) && !IN_MACRO_WHILE_DEFINITION &&
        !(SECTION_FLAGS & (STYP_TEXT | STYP_DATA | STYP_BPACK | STYP_BSS)))
      gpverror(GPE_LABEL_IN_SECTION, NULL);

    value = do_or_append_insn("set", parms);
    if (!IN_MACRO_WHILE_DEFINITION) {
      set_global(label, value, TEMPORARY, GVT_CONSTANT);
    }
  }

  return value;
}

int return_op(int operation);

void next_line(int value)
{
  if ((state.pass == 2) && (state.lst.line.linetype == LTY_DOLIST_DIR)) {
    state.lst.line.linetype = LTY_NONE;
    lst_format_line(state.src->curr_src_line.line, value);
  }

  if (IN_WHILE_EXPANSION || IN_MACRO_EXPANSION) {
    if (!IN_WHILE_DEFINITION && state.lst.expand &&
        (state.pass == 2) &&
        (state.lst.line.linetype != LTY_DOLIST_DIR) &&
        (state.lst.line.linetype != LTY_NOLIST_DIR)) {

      if (state.src->curr_src_line.line != NULL) {
        /* Empty macro. */
        lst_format_line(state.src->curr_src_line.line, value);
      }
      preproc_emit();
    }

    /* while loops can be defined inside a macro or nested */
    if (IN_MACRO_WHILE_DEFINITION) {
      state.lst.line.linetype = LTY_NONE;

      if (state.mac_body != NULL) {
        /* Empty macro. */
        state.mac_body->src_line = GP_Strdup(state.src->mac_body->src_line);
      }
    }

    if (state.src->mac_body != NULL) {
      state.src->mac_body = state.src->mac_body->next;
    }
  } else if (IN_FILE_EXPANSION) {
    if (!IN_WHILE_DEFINITION && (state.pass == 2) &&
        (state.lst.line.linetype != LTY_DOLIST_DIR) &&
        (state.lst.line.linetype != LTY_NOLIST_DIR)) {
      lst_format_line(state.src->curr_src_line.line, value);

      if (!IN_MACRO_WHILE_DEFINITION) {
        preproc_emit();
      }
    }

    if (IN_MACRO_WHILE_DEFINITION) {
      state.lst.line.linetype = LTY_NONE;

      if (state.mac_body != NULL) {
        state.mac_body->src_line = GP_Strdup(state.src->curr_src_line.line);
      }
    }

   if (state.next_state == STATE_INCLUDE) {
      /* includes have to be evaluated here and not in the following
       * switch statetems so that the errors are reported correctly */
      state.src->line_number++;
      open_src(state.next_buffer.file, true);
      free(state.next_buffer.file);
    }
  }

  switch (state.next_state) {
    case STATE_EXITMACRO:
      state.src->line_number++;
      execute_exitm();
      break;

    case STATE_MACRO:
      state.src->line_number++;
      /* push the label for local directive */
      state.stTop = push_macro_symbol_table(state.stTop);
      execute_macro(state.next_buffer.macro, false);
      break;

    case STATE_SECTION:
      state.src->line_number++;
      /* create a new coff section */
      coff_new_section(state.obj.new_sec_name,
                       state.obj.new_sec_addr,
                       state.obj.new_sec_flags);
      break;

    case STATE_WHILE:
      state.src->line_number++;
      execute_macro(state.next_buffer.macro, true);
      break;

    case STATE_INCLUDE:
      /* already evaluated */
      break;

    default:
      state.src->line_number++;
      break;
  }
}

void yyerror(const char *message)
{
  if (!IN_MACRO_WHILE_DEFINITION) {
    /* throw error if not in macro definition */
    gpverror(GPE_PARSER, NULL, message);
  }
}

/************************************************************************/


#line 386 "parse.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LABEL = 258,
    IDENTIFIER = 259,
    IDENT_BRACKET = 260,
    CBLOCK = 261,
    DEBUG_LINE = 262,
    ENDC = 263,
    ERRORLEVEL = 264,
    FILL = 265,
    LIST = 266,
    NUMBER = 267,
    PROCESSOR = 268,
    STRING = 269,
    DEFINE = 270,
    UPPER = 271,
    HIGH = 272,
    LOW = 273,
    LSH = 274,
    RSH = 275,
    GREATER_EQUAL = 276,
    LESS_EQUAL = 277,
    EQUAL = 278,
    NOT_EQUAL = 279,
    LOGICAL_AND = 280,
    LOGICAL_OR = 281,
    ASSIGN_PLUS = 282,
    ASSIGN_MINUS = 283,
    ASSIGN_MULTIPLY = 284,
    ASSIGN_DIVIDE = 285,
    ASSIGN_MODULUS = 286,
    ASSIGN_LSH = 287,
    ASSIGN_RSH = 288,
    ASSIGN_AND = 289,
    ASSIGN_OR = 290,
    ASSIGN_XOR = 291,
    INCREMENT = 292,
    DECREMENT = 293,
    POSTINCREMENT = 294,
    POSTDECREMENT = 295,
    INDFOFFSET = 296,
    TBL_NO_CHANGE = 297,
    TBL_POST_INC = 298,
    TBL_POST_DEC = 299,
    TBL_PRE_INC = 300
  };
#endif
/* Tokens.  */
#define LABEL 258
#define IDENTIFIER 259
#define IDENT_BRACKET 260
#define CBLOCK 261
#define DEBUG_LINE 262
#define ENDC 263
#define ERRORLEVEL 264
#define FILL 265
#define LIST 266
#define NUMBER 267
#define PROCESSOR 268
#define STRING 269
#define DEFINE 270
#define UPPER 271
#define HIGH 272
#define LOW 273
#define LSH 274
#define RSH 275
#define GREATER_EQUAL 276
#define LESS_EQUAL 277
#define EQUAL 278
#define NOT_EQUAL 279
#define LOGICAL_AND 280
#define LOGICAL_OR 281
#define ASSIGN_PLUS 282
#define ASSIGN_MINUS 283
#define ASSIGN_MULTIPLY 284
#define ASSIGN_DIVIDE 285
#define ASSIGN_MODULUS 286
#define ASSIGN_LSH 287
#define ASSIGN_RSH 288
#define ASSIGN_AND 289
#define ASSIGN_OR 290
#define ASSIGN_XOR 291
#define INCREMENT 292
#define DECREMENT 293
#define POSTINCREMENT 294
#define POSTDECREMENT 295
#define INDFOFFSET 296
#define TBL_NO_CHANGE 297
#define TBL_POST_INC 298
#define TBL_POST_DEC 299
#define TBL_PRE_INC 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 323 "parse.y" /* yacc.c:355  */

  gpasmVal i;
  char *s;
  struct pnode *p;

#line 522 "parse.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 539 "parse.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   445

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  130
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  200

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      61,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    59,     2,     2,    65,    58,    27,     2,
      66,    62,    56,    54,    63,    55,     2,    57,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,     2,
      25,    32,    26,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,    29,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    28,     2,    60,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      30,    31,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   420,   420,   424,   423,   429,   445,   457,   466,   475,
     484,   571,   614,   614,   617,   628,   628,   635,   640,   640,
     647,   647,   654,   660,   666,   672,   677,   682,   687,   692,
     708,   725,   724,   744,   743,   762,   769,   771,   779,   781,
     788,   798,   810,   810,   815,   819,   826,   834,   834,   834,
     835,   835,   835,   835,   836,   836,   836,   839,   844,   849,
     854,   859,   864,   869,   874,   879,   884,   891,   893,   901,
     904,   906,   914,   917,   919,   927,   930,   932,   940,   940,
     940,   943,   945,   953,   953,   953,   953,   953,   953,   956,
     958,   966,   966,   969,   971,   979,   979,   982,   984,   992,
     992,   992,   995,   997,  1004,  1004,  1004,  1004,  1004,  1004,
    1004,  1007,  1012,  1017,  1022,  1027,  1032,  1037,  1042,  1047,
    1052,  1057,  1062,  1069,  1076,  1081,  1089,  1088,  1100,  1107,
    1112
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LABEL", "IDENTIFIER", "IDENT_BRACKET",
  "CBLOCK", "DEBUG_LINE", "ENDC", "ERRORLEVEL", "FILL", "LIST", "NUMBER",
  "PROCESSOR", "STRING", "DEFINE", "UPPER", "HIGH", "LOW", "LSH", "RSH",
  "GREATER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "'<'", "'>'", "'&'",
  "'|'", "'^'", "LOGICAL_AND", "LOGICAL_OR", "'='", "ASSIGN_PLUS",
  "ASSIGN_MINUS", "ASSIGN_MULTIPLY", "ASSIGN_DIVIDE", "ASSIGN_MODULUS",
  "ASSIGN_LSH", "ASSIGN_RSH", "ASSIGN_AND", "ASSIGN_OR", "ASSIGN_XOR",
  "INCREMENT", "DECREMENT", "POSTINCREMENT", "POSTDECREMENT", "INDFOFFSET",
  "TBL_NO_CHANGE", "TBL_POST_INC", "TBL_POST_DEC", "TBL_PRE_INC", "'['",
  "']'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'\\n'", "')'",
  "','", "':'", "'$'", "'('", "$accept", "program", "$@1", "line",
  "decimal_ops", "statement", "$@2", "$@3", "$@4", "$@5", "$@6",
  "const_block", "const_line", "const_def_list", "$@7", "const_def",
  "assign_equal_ops", "parameter_list", "expr", "e9op", "e8", "e8op", "e7",
  "e7op", "e6", "e6op", "e5", "e5op", "e4", "e4op", "e3", "e3op", "e2",
  "e2op", "e1", "e1op", "e0", "cidentifier", "list_block", "list_expr",
  "$@8", "list_args", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,    60,    62,    38,   124,    94,
     280,   281,    61,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,    91,    93,    43,    45,    42,    47,    37,    33,
     126,    10,    41,    44,    58,    36,    40
};
# endif

#define YYPACT_NINF -93

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-93)))

#define YYTABLE_NINF -127

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -93,   140,   -93,   -48,    23,   -93,   384,    58,     0,   -93,
     -93,    27,   -16,   -93,     3,   -93,   -93,   -93,   -93,   254,
     -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     -14,    22,   -93,   254,   -93,   -93,   -93,   -93,   -93,   -93,
     178,   178,   -93,   -93,   -93,   254,   -93,   -93,   -93,   -93,
     -93,   -93,   178,   -93,   254,    84,   -10,    54,    69,    68,
      67,    96,   104,    73,   -93,   321,   -93,   -93,    95,   -93,
     -93,   -93,   -26,   121,   -93,   272,   101,     7,     5,   -93,
     178,   -13,   -93,   -93,     8,   118,   118,   -29,   118,    -4,
     -93,   -93,   -93,   -93,   254,   178,   254,   -93,   254,   -93,
     254,   -93,   -93,   -93,   254,   -93,   -93,   -93,   -93,   -93,
     -93,   254,   -93,   -93,   254,   -93,   -93,   254,   -93,   -93,
     -93,   254,   -93,    59,   -93,   -93,    63,    36,    90,    54,
      71,    79,    75,    91,   -93,   100,   120,    92,   113,   -93,
     -93,   -93,   -93,    25,   118,    54,    69,    68,    67,    96,
     104,    73,   -93,   -93,     2,   -93,   254,   103,   135,   -93,
     272,   -93,   -93,   165,   117,   171,   -93,   -93,   196,   128,
     -93,   -93,   -22,   175,    12,    39,   254,   254,   -93,   -93,
     -93,   130,   -93,    45,   -93,   -93,   175,   -93,   129,   136,
     -93,    50,    54,   -93,   -93,   -93,   254,   -93,   -93,   135
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     5,     0,     0,     0,    13,
      12,     0,    18,    15,     0,    14,     4,    20,    11,     0,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
       0,     0,    10,     0,   123,   112,   114,   104,   105,   106,
     121,   122,   118,   119,   120,     0,   110,   107,   117,   108,
     109,    27,    62,   113,     0,     0,    58,    67,    70,    73,
      76,    81,    89,    93,    97,     0,   102,   111,     0,   121,
     122,    33,     0,     0,    17,     0,     0,     0,     0,    26,
       0,     0,     9,     8,     0,    63,    64,     0,    61,     0,
      65,    66,    28,    69,     0,    59,     0,    72,     0,    75,
       0,    78,    79,    80,     0,    87,    88,    85,    86,    83,
      84,     0,    91,    92,     0,    95,    96,     0,    99,   100,
     101,     0,   103,     0,    36,    31,     0,     0,   123,   128,
       0,   124,     0,     0,    25,   129,     0,     0,     0,     7,
       6,   116,   115,     0,    60,    68,    71,    74,    77,    82,
      90,    94,    98,    35,    42,    36,     0,     0,     0,    19,
       0,    16,    22,     0,     0,     0,    21,    57,     0,     0,
      38,    37,     0,     0,    42,     0,     0,     0,   125,   130,
      24,     0,    40,     0,    34,    39,     0,    43,    45,     0,
      29,     0,   127,    23,    41,    44,     0,    32,    30,    46
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -93,   -93,   -93,   -93,   -93,   192,   -93,   -93,   -93,   -93,
     -93,    44,   -93,   -93,   -93,    16,   -93,   289,    -8,    46,
     -73,   -93,   105,   -93,   106,   -93,   107,   -93,    94,   -93,
      93,   -93,    98,   -93,    88,   -93,   151,   -92,    57,   -93,
     -93,    55
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     4,    16,    17,    18,    76,    75,    80,   155,
     124,   154,   171,   172,   173,   187,    33,    55,    56,    96,
      57,    98,    58,   100,    59,   104,    60,   111,    61,   114,
      62,   117,    63,   121,    64,    65,    66,    67,   130,   131,
     158,   137
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      72,    68,   129,    93,    34,   168,    93,    77,    78,   135,
     169,    81,    35,     5,    36,   168,    37,    38,    39,    93,
     189,   133,    93,   145,   141,    84,     6,     7,    93,     8,
       9,    73,    10,    11,    12,   125,    13,    87,    14,   185,
      93,   186,    94,    69,    70,    74,    89,    82,   139,    42,
      43,    44,    45,    95,    46,    47,    48,    93,   142,    49,
      50,    71,    34,   170,    79,    53,    54,   136,   134,   140,
      35,    93,    36,   170,    37,    38,    39,    93,   167,    90,
      91,   188,    93,    83,    15,    97,   143,   129,   105,   106,
     107,   108,   109,   110,   188,   101,   102,   103,   157,    99,
     190,    40,    41,   123,   192,   132,   194,    42,    43,    44,
      45,   198,    46,    47,    48,   112,   113,    49,    50,    51,
     153,    52,  -126,    53,    54,    34,   156,    90,    91,   118,
     119,   120,   159,    35,   164,    36,   161,    37,    38,    39,
       2,     3,   160,    -3,    -3,    92,    -3,    -3,   175,    -3,
      -3,    -3,   162,    -3,   165,    -3,    90,    91,   115,   116,
     183,    90,    91,   163,    40,    41,   176,    93,   191,   135,
      42,    43,    44,    45,   166,    46,    47,    48,   180,    34,
      49,    50,    34,   126,    52,   181,    53,    54,   199,   184,
      35,   193,    36,   196,    37,    38,    39,   197,    32,   174,
      34,    -3,   195,   146,   177,   149,   147,   150,    35,   152,
      36,   148,    37,    38,    39,   151,   122,   178,   179,     0,
       0,    40,    41,     0,     0,     0,     0,    42,    43,    44,
      45,     0,    46,    47,    48,     0,     0,    49,    50,    69,
      70,    52,     0,    53,    54,    42,    43,    44,    45,     0,
      46,    47,    48,     0,     0,    49,    50,   182,    34,     0,
       0,    53,    54,     0,     0,     0,    35,     0,    36,     0,
      37,    38,    39,     0,     0,     0,   128,     0,     0,     0,
       0,     0,     0,     0,    35,     0,    36,     0,    37,    38,
      39,     0,     0,     0,     0,     0,     0,    69,    70,     0,
       0,     0,     0,    42,    43,    44,    45,     0,    46,    47,
      48,     0,     0,    49,    50,    69,    70,     0,     0,    53,
      54,    42,    43,    44,    45,    34,    46,    47,    48,    85,
      86,    49,    50,    35,     0,    36,     0,    53,    54,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   127,     0,    69,    70,     0,     0,     0,   138,
      42,    43,    44,    45,     0,     0,     0,    48,     0,     0,
       0,     0,     0,     0,   144,     0,    53,    54,     7,     0,
       8,     9,     0,    10,    11,    12,     0,    13,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15
};

static const yytype_int16 yycheck[] =
{
       8,     1,    75,    32,     4,     3,    32,     4,     5,     4,
       8,    19,    12,    61,    14,     3,    16,    17,    18,    32,
       8,    14,    32,    96,    53,    33,     3,     4,    32,     6,
       7,     4,     9,    10,    11,    61,    13,    45,    15,    61,
      32,    63,    52,    43,    44,    61,    54,    61,    61,    49,
      50,    51,    52,    63,    54,    55,    56,    32,    62,    59,
      60,    61,     4,    61,    61,    65,    66,    62,    61,    61,
      12,    32,    14,    61,    16,    17,    18,    32,    53,    43,
      44,   173,    32,    61,    61,    31,    94,   160,    21,    22,
      23,    24,    25,    26,   186,    27,    28,    29,    62,    30,
      61,    43,    44,     8,   177,     4,    61,    49,    50,    51,
      52,    61,    54,    55,    56,    19,    20,    59,    60,    61,
      61,    63,    32,    65,    66,     4,    63,    43,    44,    56,
      57,    58,    61,    12,    14,    14,    61,    16,    17,    18,
       0,     1,    63,     3,     4,    61,     6,     7,   156,     9,
      10,    11,    61,    13,    62,    15,    43,    44,    54,    55,
     168,    43,    44,    63,    43,    44,    63,    32,   176,     4,
      49,    50,    51,    52,    61,    54,    55,    56,    61,     4,
      59,    60,     4,    62,    63,    14,    65,    66,   196,    61,
      12,    61,    14,    64,    16,    17,    18,    61,     6,   155,
       4,    61,   186,    98,   158,   111,   100,   114,    12,   121,
      14,   104,    16,    17,    18,   117,    65,   160,   163,    -1,
      -1,    43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,
      52,    -1,    54,    55,    56,    -1,    -1,    59,    60,    43,
      44,    63,    -1,    65,    66,    49,    50,    51,    52,    -1,
      54,    55,    56,    -1,    -1,    59,    60,    61,     4,    -1,
      -1,    65,    66,    -1,    -1,    -1,    12,    -1,    14,    -1,
      16,    17,    18,    -1,    -1,    -1,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    14,    -1,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    -1,    54,    55,
      56,    -1,    -1,    59,    60,    43,    44,    -1,    -1,    65,
      66,    49,    50,    51,    52,     4,    54,    55,    56,    40,
      41,    59,    60,    12,    -1,    14,    -1,    65,    66,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    43,    44,    -1,    -1,    -1,    80,
      49,    50,    51,    52,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    65,    66,     4,    -1,
       6,     7,    -1,     9,    10,    11,    -1,    13,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    68,     0,     1,    69,    61,     3,     4,     6,     7,
       9,    10,    11,    13,    15,    61,    70,    71,    72,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    72,    83,     4,    12,    14,    16,    17,    18,
      43,    44,    49,    50,    51,    52,    54,    55,    56,    59,
      60,    61,    63,    65,    66,    84,    85,    87,    89,    91,
      93,    95,    97,    99,   101,   102,   103,   104,     1,    43,
      44,    61,    85,     4,    61,    74,    73,     4,     5,    61,
      75,    85,    61,    61,    85,    84,    84,    85,    84,    85,
      43,    44,    61,    32,    52,    63,    86,    31,    88,    30,
      90,    27,    28,    29,    92,    21,    22,    23,    24,    25,
      26,    94,    19,    20,    96,    54,    55,    98,    56,    57,
      58,   100,   103,     8,    77,    61,    62,    84,     4,    87,
     105,   106,     4,    14,    61,     4,    62,   108,    84,    61,
      61,    53,    62,    85,    84,    87,    89,    91,    93,    95,
      97,    99,   101,    61,    78,    76,    63,    62,   107,    61,
      63,    61,    61,    63,    14,    62,    61,    53,     3,     8,
      61,    79,    80,    81,    78,    85,    63,    86,   105,   108,
      61,    14,    61,    85,    61,    61,    63,    82,   104,     8,
      61,    85,    87,    61,    61,    82,    64,    61,    61,    85
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    67,    68,    69,    68,    68,    70,    70,    70,    70,
      70,    70,    71,    71,    72,    73,    72,    72,    74,    72,
      75,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    76,    72,    77,    72,    72,    78,    78,    79,    79,
      79,    79,    81,    80,    80,    82,    82,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    85,    85,    86,
      87,    87,    88,    89,    89,    90,    91,    91,    92,    92,
      92,    93,    93,    94,    94,    94,    94,    94,    94,    95,
      95,    96,    96,    97,    97,    98,    98,    99,    99,   100,
     100,   100,   101,   101,   102,   102,   102,   102,   102,   102,
     102,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   104,   105,   105,   107,   106,   106,   108,
     108
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     3,     3,     4,     4,     3,     3,
       2,     1,     1,     1,     1,     0,     4,     2,     0,     4,
       0,     4,     4,     6,     5,     3,     2,     2,     3,     6,
       7,     0,     7,     0,     6,     4,     0,     2,     1,     2,
       2,     3,     0,     2,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     2,
       3,     2,     1,     2,     2,     2,     2,     1,     3,     1,
       1,     3,     1,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     4,     1,     1,
       3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 424 "parse.y" /* yacc.c:1646  */
    {
          state.lst.line.was_byte_addr = state.byte_addr;
          state.lst.line.linetype = LTY_NONE;
          state.next_state = STATE_NOCHANGE;
        }
#line 1826 "parse.c" /* yacc.c:1646  */
    break;

  case 5:
#line 430 "parse.y" /* yacc.c:1646  */
    {
          state.lst.line.was_byte_addr = state.byte_addr;
          state.lst.line.linetype = LTY_NONE;
          state.next_state = STATE_NOCHANGE;

          yyerrok;  /* generate multiple errors */
          if (IN_MACRO_WHILE_DEFINITION) {
            /* in macro definition: append the macro */
            macro_append();
          }
          next_line(0);
        }
#line 1843 "parse.c" /* yacc.c:1646  */
    break;

  case 6:
#line 446 "parse.y" /* yacc.c:1646  */
    {
          struct pnode *parms;
          int exp_result;

          exp_result = do_insn("set", mk_list((yyvsp[-1].p), NULL));
          parms = mk_list(mk_2op(return_op((yyvsp[-2].i)),
                                 mk_symbol((yyvsp[-3].s)),
                                 mk_constant(exp_result)), NULL);
          next_line(set_label((yyvsp[-3].s), parms));
        }
#line 1858 "parse.c" /* yacc.c:1646  */
    break;

  case 7:
#line 458 "parse.y" /* yacc.c:1646  */
    {
          struct pnode *parms;

          /* implements i = 6 + 1 */
          parms = mk_list((yyvsp[-1].p), NULL);
          next_line(set_label((yyvsp[-3].s), parms));
        }
#line 1870 "parse.c" /* yacc.c:1646  */
    break;

  case 8:
#line 467 "parse.y" /* yacc.c:1646  */
    {
          struct pnode *parms;

          /* implements i-- */
          parms = mk_list(mk_1op(DECREMENT, mk_symbol((yyvsp[-2].s))), NULL);
          next_line(set_label((yyvsp[-2].s), parms));
        }
#line 1882 "parse.c" /* yacc.c:1646  */
    break;

  case 9:
#line 476 "parse.y" /* yacc.c:1646  */
    {
          struct pnode *parms;

          /* implements i++ */
          parms = mk_list(mk_1op(INCREMENT, mk_symbol((yyvsp[-2].s))), NULL);
          next_line(set_label((yyvsp[-2].s), parms));
        }
#line 1894 "parse.c" /* yacc.c:1646  */
    break;

  case 10:
#line 485 "parse.y" /* yacc.c:1646  */
    {
          if (asm_enabled() && (state.lst.line.linetype == LTY_NONE)) {
            if (IS_RAM_ORG) {
              /* alias to next definition */
              state.lst.line.linetype = LTY_RES;
            }
            else {
              state.lst.line.linetype = LTY_INSN;
            }
          }

          if (asm_enabled()) {
            if (state.mac_head != NULL) {
              /* This is a macro definition. Set it up */
              struct symbol *mac;
              struct macro_head *h = NULL;

              mac = get_symbol(state.stMacros, (yyvsp[-1].s));
              if (mac != NULL) {
                h = get_symbol_annotation(mac);
              }

              /* It's not an error if macro was defined on pass 1 and we're in pass 2. */
              if ((h != NULL) && !((h->pass == 1) && (state.pass == 2))) {
                gpverror(GPE_DUPLICATE_MACRO, NULL);
              } else {
                if (mac == NULL) {
                  mac = add_symbol(state.stMacros, (yyvsp[-1].s));
                }
                annotate_symbol(mac, state.mac_head);
                h = state.mac_head;
                h->line_number = state.src->line_number;
                h->file_symbol = state.src->file_symbol;
              }
              h->pass = state.pass;

              /* The macro is defined so allow calls. */
              if (state.pass == 2) {
                h->defined = true;
              }

              state.mac_head = NULL;
            } else if (!IN_MACRO_WHILE_DEFINITION) {
              /* Outside a macro definition, just define the label. */
              switch (state.lst.line.linetype) {
              case LTY_SEC:
                gp_strncpy(state.obj.new_sec_name, (yyvsp[-1].s), sizeof(state.obj.new_sec_name));
                break;

              case LTY_SET:
                set_global((yyvsp[-1].s), (yyvsp[0].i), TEMPORARY, GVT_CONSTANT);
                break;

              case LTY_ORG:
              case LTY_EQU:
                set_global((yyvsp[-1].s), (yyvsp[0].i), PERMANENT, GVT_CONSTANT);
                break;

              case LTY_INSN:
              case LTY_DATA:
              case LTY_RES:
                if ((state.mode == MODE_RELOCATABLE) && !IN_MACRO_WHILE_DEFINITION &&
                    !(SECTION_FLAGS & (STYP_TEXT | STYP_DATA | STYP_BPACK | STYP_BSS))) {
                  gpverror(GPE_LABEL_IN_SECTION, NULL);
                }

                if (IS_RAM_ORG) {
                  set_global((yyvsp[-1].s), (yyvsp[0].i), PERMANENT, GVT_STATIC);
                }
                else {
                  set_global((yyvsp[-1].s), (yyvsp[0].i), PERMANENT, GVT_ADDRESS);
                }
                break;

              case LTY_DIR:
                gpverror(GPE_ILLEGAL_LABEL, NULL, (yyvsp[-1].s));
                break;

              default:
                break;
              }
            }
          }
          next_line((yyvsp[0].i));
        }
#line 1984 "parse.c" /* yacc.c:1646  */
    break;

  case 11:
#line 572 "parse.y" /* yacc.c:1646  */
    {
          if (state.mac_head != NULL) {
            /* This is a macro definition, but the label was missing. */
            state.mac_head = NULL;
            gpverror(GPE_NO_MACRO_NAME, NULL);
          } else {
            if (state.found_end) {
              switch (state.src->type) {
              case SRC_WHILE:
                gperror(GPE_EXPECTED, "Expected (ENDW)");
                break;

              case SRC_MACRO:
                gperror(GPW_EXPECTED,"Expected (ENDM)");
                /* fall through */

              default:
                break;
              }

              if (state.astack != NULL) {
                struct amode *old;

                while (state.astack != NULL) {
                  old = state.astack;
                  state.astack = state.astack->prev;
                  free(old);
                }
                gpwarning(GPW_EXPECTED, "Expected (ENDIF)");
              }
            }

            next_line((yyvsp[0].i));

            if (state.found_end) {
              found_end();
              YYACCEPT;
            }
          }
        }
#line 2029 "parse.c" /* yacc.c:1646  */
    break;

  case 14:
#line 618 "parse.y" /* yacc.c:1646  */
    {
          if (!IN_MACRO_WHILE_DEFINITION) {
            (yyval.i) = (IS_RAM_ORG) ? state.byte_addr :
                                /* We want to have r as the value to assign to label. */
                                gp_processor_byte_to_real(state.processor, state.byte_addr);
          } else {
            macro_append();
          }
        }
#line 2043 "parse.c" /* yacc.c:1646  */
    break;

  case 15:
#line 628 "parse.y" /* yacc.c:1646  */
    { force_ident = true; }
#line 2049 "parse.c" /* yacc.c:1646  */
    break;

  case 16:
#line 630 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-3].s), mk_list(mk_symbol((yyvsp[-1].s)), NULL));
          force_ident = false;
        }
#line 2058 "parse.c" /* yacc.c:1646  */
    break;

  case 17:
#line 636 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-1].s), NULL);
        }
#line 2066 "parse.c" /* yacc.c:1646  */
    break;

  case 18:
#line 640 "parse.y" /* yacc.c:1646  */
    { force_decimal = true; }
#line 2072 "parse.c" /* yacc.c:1646  */
    break;

  case 19:
#line 642 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-3].s), (yyvsp[-1].p));
          force_decimal = false;
        }
#line 2081 "parse.c" /* yacc.c:1646  */
    break;

  case 20:
#line 647 "parse.y" /* yacc.c:1646  */
    { force_decimal = true; }
#line 2087 "parse.c" /* yacc.c:1646  */
    break;

  case 21:
#line 649 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-3].s), (yyvsp[-1].p));
          force_decimal = false;
        }
#line 2096 "parse.c" /* yacc.c:1646  */
    break;

  case 22:
#line 655 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-3].s), mk_list(mk_string((yyvsp[-2].s)),
                                 mk_list(mk_string((yyvsp[-1].s)), NULL)));
        }
#line 2105 "parse.c" /* yacc.c:1646  */
    break;

  case 23:
#line 661 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-5].s), mk_list(mk_string((yyvsp[-4].s)),
                                 mk_list(mk_string((yyvsp[-1].s)), (yyvsp[-3].p))));
        }
#line 2114 "parse.c" /* yacc.c:1646  */
    break;

  case 24:
#line 667 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-4].s), mk_list(mk_string((yyvsp[-3].s)),
                                 mk_list(mk_string((yyvsp[-1].s)), NULL)));
        }
#line 2123 "parse.c" /* yacc.c:1646  */
    break;

  case 25:
#line 673 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-2].s), mk_list(mk_string((yyvsp[-1].s)), NULL));
        }
#line 2131 "parse.c" /* yacc.c:1646  */
    break;

  case 26:
#line 678 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-1].s), NULL);
        }
#line 2139 "parse.c" /* yacc.c:1646  */
    break;

  case 27:
#line 683 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-1].s), NULL);
        }
#line 2147 "parse.c" /* yacc.c:1646  */
    break;

  case 28:
#line 688 "parse.y" /* yacc.c:1646  */
    {
          (yyval.i) = do_or_append_insn((yyvsp[-2].s), (yyvsp[-1].p));
        }
#line 2155 "parse.c" /* yacc.c:1646  */
    break;

  case 29:
#line 693 "parse.y" /* yacc.c:1646  */
    {
          int number;
          int i;

          if (!IN_MACRO_WHILE_DEFINITION) {
            number = eval_fill_number((yyvsp[-1].p));

            for (i = 0; i < number; i++) {
              (yyval.i) = do_insn((yyvsp[-4].s), NULL);
            }
          } else {
            macro_append();
          }
        }
#line 2174 "parse.c" /* yacc.c:1646  */
    break;

  case 30:
#line 709 "parse.y" /* yacc.c:1646  */
    {
          int number;
          int i;

          if (!IN_MACRO_WHILE_DEFINITION) {
            number = eval_fill_number((yyvsp[-1].p));

            for (i = 0; i < number; i++) {
              (yyval.i) = do_insn((yyvsp[-5].s), (yyvsp[-4].p));
            }
          } else {
            macro_append();
          }
        }
#line 2193 "parse.c" /* yacc.c:1646  */
    break;

  case 31:
#line 725 "parse.y" /* yacc.c:1646  */
    {
          if (!IN_MACRO_WHILE_DEFINITION) {
            begin_cblock((yyvsp[-1].p));
          } else {
            macro_append();
          }
          next_line(0);
        }
#line 2206 "parse.c" /* yacc.c:1646  */
    break;

  case 32:
#line 735 "parse.y" /* yacc.c:1646  */
    {
          state.lst.line.linetype = LTY_NONE;
          if (IN_MACRO_WHILE_DEFINITION) {
            macro_append();
          }
          (yyval.i) = 0;
        }
#line 2218 "parse.c" /* yacc.c:1646  */
    break;

  case 33:
#line 744 "parse.y" /* yacc.c:1646  */
    {
          if (!IN_MACRO_WHILE_DEFINITION) {
            continue_cblock();
          } else {
            macro_append();
          }
          next_line(0);
        }
#line 2231 "parse.c" /* yacc.c:1646  */
    break;

  case 34:
#line 754 "parse.y" /* yacc.c:1646  */
    {
          state.lst.line.linetype = LTY_NONE;
          if (IN_MACRO_WHILE_DEFINITION) {
            macro_append();
          }
          (yyval.i) = 0;
        }
#line 2243 "parse.c" /* yacc.c:1646  */
    break;

  case 35:
#line 763 "parse.y" /* yacc.c:1646  */
    {
          state.lst.line.linetype = LTY_NONE;
          (yyval.i) = 0;
        }
#line 2252 "parse.c" /* yacc.c:1646  */
    break;

  case 37:
#line 772 "parse.y" /* yacc.c:1646  */
    {
          state.lst.line.linetype = LTY_SET;
          next_line(state.lst.cblock_lst);
        }
#line 2261 "parse.c" /* yacc.c:1646  */
    break;

  case 39:
#line 782 "parse.y" /* yacc.c:1646  */
    {
          if (IN_MACRO_WHILE_DEFINITION) {
            macro_append();
          }
        }
#line 2271 "parse.c" /* yacc.c:1646  */
    break;

  case 40:
#line 789 "parse.y" /* yacc.c:1646  */
    {
          if (!IN_MACRO_WHILE_DEFINITION) {
            state.lst.cblock_lst = state.cblock;
            cblock_expr(mk_symbol((yyvsp[-1].s)));
          } else {
            macro_append();
          }
        }
#line 2284 "parse.c" /* yacc.c:1646  */
    break;

  case 41:
#line 799 "parse.y" /* yacc.c:1646  */
    {
          if (!IN_MACRO_WHILE_DEFINITION) {
            state.lst.cblock_lst = state.cblock;
            cblock_expr_incr(mk_symbol((yyvsp[-2].s)), (yyvsp[-1].p));
          } else {
            macro_append();
          }
        }
#line 2297 "parse.c" /* yacc.c:1646  */
    break;

  case 42:
#line 810 "parse.y" /* yacc.c:1646  */
    {
          state.lst.cblock_lst = state.cblock;
        }
#line 2305 "parse.c" /* yacc.c:1646  */
    break;

  case 45:
#line 820 "parse.y" /* yacc.c:1646  */
    {
          if (!IN_MACRO_WHILE_DEFINITION) {
            cblock_expr((yyvsp[0].p));
          }
        }
#line 2315 "parse.c" /* yacc.c:1646  */
    break;

  case 46:
#line 827 "parse.y" /* yacc.c:1646  */
    {
          if (!IN_MACRO_WHILE_DEFINITION) {
            cblock_expr_incr((yyvsp[-2].p), (yyvsp[0].p));
          }
        }
#line 2325 "parse.c" /* yacc.c:1646  */
    break;

  case 57:
#line 840 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_constant(INDFOFFSET), mk_list((yyvsp[-1].p), (yyvsp[-3].p)));
        }
#line 2333 "parse.c" /* yacc.c:1646  */
    break;

  case 58:
#line 845 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list((yyvsp[0].p), NULL);
        }
#line 2341 "parse.c" /* yacc.c:1646  */
    break;

  case 59:
#line 850 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list((yyvsp[-1].p), mk_list(mk_symbol(""), NULL));
        }
#line 2349 "parse.c" /* yacc.c:1646  */
    break;

  case 60:
#line 855 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list((yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2357 "parse.c" /* yacc.c:1646  */
    break;

  case 61:
#line 860 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_symbol(""), (yyvsp[0].p));
        }
#line 2365 "parse.c" /* yacc.c:1646  */
    break;

  case 62:
#line 865 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_symbol(""), mk_list(mk_symbol(""), NULL));
        }
#line 2373 "parse.c" /* yacc.c:1646  */
    break;

  case 63:
#line 870 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_constant((yyvsp[-1].i)), (yyvsp[0].p));
        }
#line 2381 "parse.c" /* yacc.c:1646  */
    break;

  case 64:
#line 875 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_constant((yyvsp[-1].i)), (yyvsp[0].p));
        }
#line 2389 "parse.c" /* yacc.c:1646  */
    break;

  case 65:
#line 880 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_constant(POSTINCREMENT), (yyvsp[-1].p));
        }
#line 2397 "parse.c" /* yacc.c:1646  */
    break;

  case 66:
#line 885 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_constant(POSTDECREMENT), (yyvsp[-1].p));
        }
#line 2405 "parse.c" /* yacc.c:1646  */
    break;

  case 68:
#line 894 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2415 "parse.c" /* yacc.c:1646  */
    break;

  case 71:
#line 907 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2425 "parse.c" /* yacc.c:1646  */
    break;

  case 74:
#line 920 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2435 "parse.c" /* yacc.c:1646  */
    break;

  case 77:
#line 933 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2445 "parse.c" /* yacc.c:1646  */
    break;

  case 82:
#line 946 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2455 "parse.c" /* yacc.c:1646  */
    break;

  case 90:
#line 959 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2465 "parse.c" /* yacc.c:1646  */
    break;

  case 94:
#line 972 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2475 "parse.c" /* yacc.c:1646  */
    break;

  case 98:
#line 985 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[-2].p));
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_2op((yyvsp[-1].i), (yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2485 "parse.c" /* yacc.c:1646  */
    break;

  case 103:
#line 998 "parse.y" /* yacc.c:1646  */
    {
          coerce_str1((yyvsp[0].p));
          (yyval.p) = mk_1op((yyvsp[-1].i), (yyvsp[0].p));
        }
#line 2494 "parse.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1008 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = (yyvsp[0].p);
        }
#line 2502 "parse.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1013 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_constant((yyvsp[0].i));
        }
#line 2510 "parse.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1018 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_symbol("$");
        }
#line 2518 "parse.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1023 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_string((yyvsp[0].s));
        }
#line 2526 "parse.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1028 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = (yyvsp[-1].p);
        }
#line 2534 "parse.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1033 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_offset((yyvsp[-1].p));
        }
#line 2542 "parse.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1038 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_constant(TBL_NO_CHANGE);
        }
#line 2550 "parse.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1043 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_constant((yyvsp[0].i));
        }
#line 2558 "parse.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1048 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_constant((yyvsp[0].i));
        }
#line 2566 "parse.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1053 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_constant((yyvsp[0].i));
        }
#line 2574 "parse.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1058 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_constant((yyvsp[0].i));
        }
#line 2582 "parse.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1063 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_constant((yyvsp[0].i));
        }
#line 2590 "parse.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1070 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_symbol((yyvsp[0].s));
        }
#line 2598 "parse.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1077 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list((yyvsp[0].p), NULL);
        }
#line 2606 "parse.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1082 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list((yyvsp[-2].p), (yyvsp[0].p));
        }
#line 2614 "parse.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1089 "parse.y" /* yacc.c:1646  */
    {
          if ((strcasecmp((yyvsp[0].s), "p") == 0) || (strcasecmp((yyvsp[0].s), "pe") == 0)) {
            force_ident = true;
          }
        }
#line 2624 "parse.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1095 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_2op((yyvsp[-1].i), mk_symbol((yyvsp[-3].s)), (yyvsp[0].p));
          force_ident = false;
        }
#line 2633 "parse.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1101 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = (yyvsp[0].p);
        }
#line 2641 "parse.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1108 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_symbol((yyvsp[0].s)), NULL);
        }
#line 2649 "parse.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1113 "parse.y" /* yacc.c:1646  */
    {
          (yyval.p) = mk_list(mk_symbol((yyvsp[-2].s)), (yyvsp[0].p));
        }
#line 2657 "parse.c" /* yacc.c:1646  */
    break;


#line 2661 "parse.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
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
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1117 "parse.y" /* yacc.c:1906  */


int return_op(int operation)
{
  /* returns an operator for the replacement of i+=1 with i=i+1*/
  switch(operation) {
  case ASSIGN_PLUS:     return '+';
  case ASSIGN_MINUS:    return '-';
  case ASSIGN_MULTIPLY: return '*';
  case ASSIGN_DIVIDE:   return '/';
  case ASSIGN_MODULUS:  return '%';
  case ASSIGN_LSH:      return LSH;
  case ASSIGN_RSH:      return RSH;
  case ASSIGN_AND:      return '&';
  case ASSIGN_OR:       return '|';
  case ASSIGN_XOR:      return '^';
  default:
    assert(0); /* Unhandled operator */
  }

  return 0;
}
