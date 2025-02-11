// Author: Morten Grønnesby <morten.gronnesby@uit.no>
#ifndef PRINTING_H
#define PRINTING_H

#include <stdlib.h>

/* This file defines different macros for printing debug and error information using different colors.
 * To control the amount of printing use the LOG_LEVEL constant in the Makefile.
 * The higher the log level, the higher the verbosity of the printing.
 * Defining -DERROR_FATAL in the Makefile will casue error prints to also exit the program.
 */

//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

//Regular underline text
#define UBLK "\e[4;30m"
#define URED "\e[4;31m"
#define UGRN "\e[4;32m"
#define UYEL "\e[4;33m"
#define UBLU "\e[4;34m"
#define UMAG "\e[4;35m"
#define UCYN "\e[4;36m"
#define UWHT "\e[4;37m"

//Regular background
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

//High intensty background 
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

//High intensty text
#define HBLK "\e[0;90m"
#define HRED "\e[0;91m"
#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HBLU "\e[0;94m"
#define HMAG "\e[0;95m"
#define HCYN "\e[0;96m"
#define HWHT "\e[0;97m"

//Bold high intensity text
#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"

//Reset
#define reset "\e[0m"


/*
 * Prints an info message to stdout.
 * Use this to print information that should be redirected.
 */
#if LOG_LEVEL <= 0
#define INFO_PRINT(...) do { fprintf(stderr, "%s", BGRN); fprintf(stderr, "[INFO][%s %d]: ", __FILE__, __LINE__); fprintf(stderr, "%s", reset); fprintf(stdout, __VA_ARGS__); } while(0)
#else
#define INFO_PRINT(...) do { } while(0)
#endif

/*
 * Prints a debug message to stderr.
 * Use this to print debug information.
 */
#if LOG_LEVEL <= 1
#define DEBUG_PRINT(...) do { fprintf(stderr, "%s", BYEL); fprintf(stderr, "[DEBUG][%s %d]: ", __FILE__, __LINE__); fprintf(stderr, "%s", reset); fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define DEBUG_PRINT(...) do { } while(0)
#endif

/*
 * Prints an error message and terminates the program.
 * Use this to report fatal errors that prevent your program from proceeding.
 * If ERROR_FATAL is defined, the program will exit after printing.
 */
#if LOG_LEVEL <= 2
#ifdef ERROR_FATAL
#define ERROR_PRINT(...) do { fprintf(stderr, "%s", BRED); fprintf(stderr, "[ERROR][%s %d]: ", __FILE__, __LINE__); fprintf(stderr, "%s", reset); fprintf(stderr, __VA_ARGS__); exit(1); } while(0)
#else
#define ERROR_PRINT(...) do { fprintf(stderr, "%s", BRED); fprintf(stderr, "[ERROR][%s %d]: ", __FILE__, __LINE__); fprintf(stderr, "%s", reset); fprintf(stderr, __VA_ARGS__); } while(0)
#endif
#else
#define ERROR_PRINT(...) do { } while(0)
#endif


#define TEST_PRINT(...) do { fprintf(stderr, "%s", BCYN); fprintf(stderr, "[TEST]: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "%s", reset);} while(0)

#endif // __PRINTING_H__