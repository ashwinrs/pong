//
//  Logging.h
//  pong
//
//  Created by Ashwin R S on 5/4/14.
//  File contaings macros for logging
//

#ifndef pong_Logging_h
#define pong_Logging_h

#ifdef LOG
#define PRINTA(a) printf(a)
#else
#define PRINTA(a)
#endif

#ifdef LOG
#define PRINT1(a,b) printf(a,b)
#else
#define PRINT1(a,b)
#endif

#ifdef LOG
#define PRINT2(a,b,c) printf(a,b,c)
#else
#define PRINT2(a,b,c)
#endif

#ifdef LOG
#define PRINT3(a,b,c,d) printf(a,b,c,d)
#else
#define PRINT3(a,b,c,d)
#endif


#endif
