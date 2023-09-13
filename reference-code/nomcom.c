   /****************************************************************
     *
     *  Reference code for
     *      "Publicly Verifiable Random Selection"
     *          Donald E. Eastlake 3rd
     *              February 2004
     *
     ****************************************************************/
    #include <limits.h>
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    /* From RFC 1321 */
    #include "global.h"
    #include "MD5.h"

    /* local prototypes */
    int longremainder ( unsigned short divisor,
                        unsigned char dividend[16] );
    long int getinteger ( char *string );
    double NPentropy ( int N, int P );


    /* limited to up to 16 inputs of up to sixteen integers each */
    /* pool limit of 2**8-1 extended to 2**16-1 by Erik Nordmark */
    /****************************************************************/

    int main ()
    {
    int         i, j,  k, k2, err, keysize, selection, usel;
    unsigned short   remaining, *selected;
    long int    pool, temp, array[16];
    MD5_CTX     ctx;
    char        buffer[257], key [800], sarray[16][256];
    char        extra[256];
    unsigned char    uc16[16], unch1, unch2;

    pool = getinteger ( "Type size of pool:\n" );
    if ( pool > 65535 )

        {
        printf ( "Pool too big.\n" );
        exit ( 1 );
        }
    selected = (unsigned short *) malloc ( (size_t)pool );
    if ( !selected )
        {
        printf ( "Out of memory.\n" );
        exit ( 1 );
        }
    selection = getinteger ( "Type number of items to be selected:\n" );
    if ( selection > pool )
        {
        printf ( "Pool too small.\n" );
        exit ( 1 );
        }
    if ( selection == pool )
        printf ( "All of the pool is selected.\n" );
    else
        {
        err = printf ( "Approximately %.1f bits of entropy needed.\n",
                        NPentropy ( selection, pool ) + 0.1 );
        if ( err <= 0 ) exit ( 1 );
        }
    for ( i = 0, keysize = 0; i < 16; ++i )
        {
        if ( keysize > 500 )
            {
            printf ( "Too much input.\n" );
            exit ( 1 );
            }
        /* get the "random" inputs. echo back to user so the user may
           be able to tell if truncation or other glitches occur.  */
        err = printf (
            "\nType #%d randomness or 'end' followed by new line.\n"
            "Up to 16 integers or the word 'float' followed by up\n"
            "to 16 x.y format reals.\n", i+1 );
        if ( err <= 0 ) exit ( 1 );
        gets ( buffer );
        j = sscanf ( buffer,
                "%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld%ld",
            &array[0], &array[1], &array[2], &array[3],
            &array[4], &array[5], &array[6], &array[7],
            &array[8], &array[9], &array[10], &array[11],
            &array[12], &array[13], &array[14], &array[15] );
        if ( j == EOF )
            exit ( j );
        if ( !j )
            if ( buffer[0] == 'e' )
                break;

            else
                {   /* floating point code by Matt Crawford */
                j = sscanf ( buffer,
                    "float %ld.%[0-9]%ld.%[0-9]%ld.%[0-9]%ld.%[0-9]"
                    "%ld.%[0-9]%ld.%[0-9]%ld.%[0-9]%ld.%[0-9]"
                    "%ld.%[0-9]%ld.%[0-9]%ld.%[0-9]%ld.%[0-9]"
                    "%ld.%[0-9]%ld.%[0-9]%ld.%[0-9]%ld.%[0-9]",
                    &array[0], sarray[0], &array[1], sarray[1],
                    &array[2], sarray[2], &array[3], sarray[3],
                    &array[4], sarray[4], &array[5], sarray[5],
                    &array[6], sarray[6], &array[7], sarray[7],
                    &array[8], sarray[8], &array[9], sarray[9],
                    &array[10], sarray[10], &array[11], sarray[11],
                    &array[12], sarray[12], &array[13], sarray[13],
                    &array[14], sarray[14], &array[15], sarray[15] );
                if ( j == 0 || j & 1 )
                    printf ( "Bad format." );
                else {
                     for ( k = 0, j /= 2; k < j; k++ )
                     {
                           /* strip trailing zeros */
                     for ( k2=strlen(sarray[k]); sarray[k][--k2]=='0';)
                           sarray[k][k2] = '\0';
                     err = printf ( "%ld.%s\n", array[k], sarray[k] );
                     if ( err <= 0 ) exit ( 1 );
                     keysize += sprintf ( &key[keysize], "%ld.%s",
                                          array[k], sarray[k] );
                     }
                     keysize += sprintf ( &key[keysize], "/" );
                     }
                }
        else
            {   /* sort values, not a very efficient algorithm */
            for ( k2 = 0; k2 < j - 1; ++k2 )
                for ( k = 0; k < j - 1; ++k )
                    if ( array[k] > array[k+1] )
                        {
                        temp = array[k];
                        array[k] = array[k+1];
                        array[k+1] = temp;
                        }
            for ( k = 0; k < j; ++k )
                { /* print for user check */
                err = printf ( "%ld ", array[k] );
                if ( err <= 0 ) exit ( 1 );
                keysize += sprintf ( &key[keysize], "%ld.", array[k] );
                }
            keysize += sprintf ( &key[keysize], "/" );
            }
        }   /* end for i */
    printf ( "Extra key material (appended) or hit return\n" );
    if ( gets(extra) != NULL && *extra != '\0' ) {
	keysize += sprintf( &key[keysize], "%s./");


    /* have obtained all the input, now produce the output */
    err = printf ( "Key is:\n %s\n", key );
    if ( err <= 0 ) exit ( 1 );
    for ( i = 0; i < pool; ++i )
        selected [i] = (unsigned short)(i + 1);
    printf ( "index        hex value of MD5        div  selected\n" );
    for (   usel = 0, remaining = (unsigned short)pool;
            usel < selection;
            ++usel, --remaining )
        {
        unch1 = (unsigned char)usel;
        unch2 = (unsigned char)(usel>>8);
        /* prefix/suffix extended to 2 bytes by Donald Eastlake */
        MD5Init ( &ctx );
        MD5Update ( &ctx, &unch2, 1 );
        MD5Update ( &ctx, &unch1, 1 );
        MD5Update ( &ctx, (unsigned char *)key, keysize );
        MD5Update ( &ctx, &unch2, 1 );
        MD5Update ( &ctx, &unch1, 1 );
        MD5Final ( uc16, &ctx );
        k = longremainder ( remaining, uc16 );
    /* printf ( "Remaining = %d, remainder = %d.\n", remaining, k ); */
        for ( j = 0; j < pool; ++j )
            if ( selected[j] )
                if ( --k < 0 )
                    {
                    printf ( "%2d  "
    "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X  "
    "%2d  -> %2d <-\n",
    usel+1, uc16[0],uc16[1],uc16[2],uc16[3],uc16[4],uc16[5],uc16[6],
    uc16[7],uc16[8],uc16[9],uc16[10],uc16[11],uc16[12],uc16[13],
    uc16[14],uc16[15], remaining, selected[j] );
                    selected[j] = 0;
                    break;
                    }
        }
     printf ( "\nDone, type any character to exit.\n" );
     getchar ();
     return 0;
     }

     /* prompt for a positive non-zero integer input */
     /****************************************************************/
     long int getinteger ( char *string )
     {
     long int     i;
     int          j;
     char    tin[257];

     while ( 1 )
     {
     printf ( "%s", string );
     printf ( "(or 'exit' to exit) " );
     gets ( tin );
     j = sscanf ( tin, "%ld", &i );
     if (    ( j == EOF )

         ||  ( !j && ( ( tin[0] == 'e' ) || ( tin[0] == 'E' ) ) )
             )
         exit ( j );
     if ( ( j == 1 ) &&
          ( i > 0 ) )
         return i;
     }   /* end while */
     }

     /* get remainder of dividing a 16 byte unsigned int
        by a small positive number */
     /****************************************************************/
     int longremainder ( unsigned short divisor,
                         unsigned char dividend[16] )
     {
     int i;
     long int kruft;

     if ( !divisor )
         return -1;
     for ( i = 0, kruft = 0; i < 16; ++i )
         {
         kruft = ( kruft << 8 ) + dividend[i];
         kruft %= divisor;
         }
     return kruft;
     }   /* end longremainder */

    /* calculate how many bits of entropy it takes to select N from P */
    /****************************************************************/
    /*             P!
      log  ( ----------------- )
         2    N! * ( P - N )!
    */

    double NPentropy ( int N, int P )
    {
    int         i;
    double      result = 0.0;

    if (    ( N < 1 )   /* not selecting anything? */
       ||   ( N >= P )  /* selecting all of pool or more? */
       )
        return 0.0;     /* degenerate case */
    for ( i = P; i > ( P - N ); --i )
	printf(">> %d\n", i),
        result += log ( i );
    for ( i = N; i > 1; --i )
	printf(">> %d\n", i),
        result -= log ( i );
    /* divide by [ log (base e) of 2 ] to convert to bits */
    result /= 0.69315;

    return result;
    }   /* end NPentropy */

