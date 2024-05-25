/*
 * CySecDrv.c
 *  Description: Cyber Security Driver c source file
 *
 *  Created on: March 20, 2024
 *  Author: Maroiu Alex
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/

/*Include project header files*/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "CySecDrv.h"

/*#################################*/
/*         Local defines           */
/*#################################*/

#define SHA1_HASH_SIZE                  (20U)
#define BLOCK_SIZE_BYTE                 (64U)
#define TRIGGER_HASH_CALCULATION        (0U)
#define FINISHED_HASH_CALCULATION       (32U)
#define TIME_ELAPSED_HASH_CALCULATION   (100U)

/*
 *  Define the SHA1 circular left shift macro
 */
#define SHA1CircularShift(bits, word) (((word) << (bits)) | ((word) >> (32 - (bits))))

/*#################################*/
/*        Local data types         */
/*#################################*/

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct
{
    uint32_t Intermediate_Hash[SHA1_HASH_SIZE >> 2]; /* Message Digest  */

    uint32_t Length_Low;  /* Message length in bits      */
    uint32_t Length_High; /* Message length in bits      */

    /* Index into message block array   */
    int32_t Message_Block_Index;
    uint8_t Message_Block[64]; /* 512-bit message blocks      */

    uint8_t Computed;  /* Is the digest computed?         */
    uint8_t Corrupted; /* Is the message digest corrupted? */
    CySecDrv_StateType isSecured;
    uint8_t isExpectedHashInitialized;
} SHA1Context;

/*#################################*/
/*        Global ROM data          */
/*#################################*/

/*#################################*/
/*        Global RAM data          */
/*#################################*/

/*#################################*/
/*        Local ROM data           */
/*#################################*/

/*#################################*/
/*        Local RAM data           */
/*#################################*/

SHA1Context context;
uint8_t hash[SHA1_HASH_SIZE];
uint8_t expected_hash[SHA1_HASH_SIZE] = {0};
// {
//     0x8E,
//     0x65,
//     0x64,
//     0xF3,
//     0x7B,
//     0xAB,
//     0xBA,
//     0x71,
//     0x85,
//     0x3E,
//     0x0E,
//     0xB2,
//     0xA1,
//     0x43,
//     0x4F,
//     0xBC,
//     0xFE,
//     0x96,
//     0x18,
//     0xCB,
// };

uint8_t block[BLOCK_SIZE_BYTE] = {0};

/*#################################*/
/*    Local function declaration   */
/*#################################*/

void SHA1Reset(void);
CySecDrv_SHA_StatusType SHA1Input(const uint8_t * input, unsigned int length);
CySecDrv_SHA_StatusType SHA1Result(uint8_t Message_Digest[SHA1_HASH_SIZE]);
void SHA1PadMessage(void);
void SHA1ProcessMessageBlock(void);
void CheckSha1Hash(void);

/*#################################*/
/*  Global function implementation */
/*#################################*/


void CySecDrv_Init(void)
{
    SHA1Reset();
    context.isSecured = CYSEC_DRV_IS_SECURED;
    context.isExpectedHashInitialized = 0;
}

void CySecDrv_Main(void)
{
    CySecDrv_SHA_StatusType error;
    static uint8_t phase_counter = 0;
    
    if(TRIGGER_HASH_CALCULATION == phase_counter)
    {
        SHA1Reset();
    }
    else if(FINISHED_HASH_CALCULATION == phase_counter)
    {
        error = SHA1Result(hash);
        if(error)
        {
            context.isSecured = CYSEC_DRV_IS_NOT_SECURED;
        }
        else
        {
            if (context.isExpectedHashInitialized == 0)
            {
                uint8_t index;
                for (index = 0; index < SHA1_HASH_SIZE; index++)
                {
                    expected_hash[index] = hash[index];
                }
            }

            CheckSha1Hash();
        }
    }
    else if(TIME_ELAPSED_HASH_CALCULATION == phase_counter)
    {
        phase_counter = 255; //overflow for next incrementation / reset timer
    }

    if(FINISHED_HASH_CALCULATION > phase_counter)
    {
        uint8_t * flash = (uint8_t *) 0x2000 + (phase_counter * BLOCK_SIZE_BYTE);
        uint8_t i;
        for (i=0; i<BLOCK_SIZE_BYTE; i++)
        {
            block[i] = pgm_read_byte(flash);
            flash++;
        }
        error = SHA1Input((const uint8_t *)block, BLOCK_SIZE_BYTE);
        if(error)
        {
            context.isSecured = CYSEC_DRV_IS_NOT_SECURED;
        }
    }

    phase_counter++;
}

CySecDrv_StateType CySecDrv_IsHashValid(void)
{
    return context.isSecured;
}

/*#################################*/
/*  Local function implementation  */
/*#################################*/

void CheckSha1Hash(void)
{
    uint8_t index;

    context.isSecured  = CYSEC_DRV_IS_SECURED;

    for (index = 0; index < SHA1_HASH_SIZE; index++)
    {
        if(hash[index] != expected_hash[index])
        {
            context.isSecured = CYSEC_DRV_IS_NOT_SECURED;
        }
    }
}

/*
 *  SHA1Reset
 *
 *  Description:
 *      This function will initialize the SHA1Context in preparation
 *      for computing a new SHA1 message digest.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
void SHA1Reset(void)
{
    context.Length_Low = 0;
    context.Length_High = 0;
    context.Message_Block_Index = 0;

    context.Intermediate_Hash[0] = 0x67452301;
    context.Intermediate_Hash[1] = 0xEFCDAB89;
    context.Intermediate_Hash[2] = 0x98BADCFE;
    context.Intermediate_Hash[3] = 0x10325476;
    context.Intermediate_Hash[4] = 0xC3D2E1F0;

    context.Computed = 0;
    context.Corrupted = 0;
}

/*
 *  SHA1Result
 *
 *  Description:
 *      This function will return the 160-bit message digest into the
 *      Message_Digest array  provided by the caller.
 *      NOTE: The first octet of hash is stored in the 0th element,
 *            the last octet of hash in the 19th element.
 *
 *  Parameters:
 *      Message_Digest: [out]
 *          Where the digest is returned.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
CySecDrv_SHA_StatusType SHA1Result(uint8_t Message_Digest[SHA1_HASH_SIZE])
{
    int i;

    if (!Message_Digest)
    {
        return SHA_NULL;
    }

    if (context.Corrupted)
    {
        return SHA_NULL;
    }

    if (!context.Computed)
    {
        SHA1PadMessage();
        for (i = 0; i < 64; ++i)
        {
            /* message may be sensitive, clear it out */
            context.Message_Block[i] = 0;
        }
        context.Length_Low = 0; /* and clear length */
        context.Length_High = 0;
        context.Computed = 1;
    }

    for (i = 0; i < SHA1_HASH_SIZE; ++i)
    {
        Message_Digest[i] = context.Intermediate_Hash[i >> 2] >> 8 * (3 - (i & 0x03));
    }

    return SHA_SUCCESS;
}

/*
 *  SHA1Input
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the message.
 *
 *  Parameters:
 *      input: [in]
 *          An array of characters representing the next portion of
 *          the message.
 *      length: [in]
 *          The length of the message in message_array
 *
 *  Returns:
 *      sha Error Code.
 *
 */
CySecDrv_SHA_StatusType SHA1Input(const uint8_t * input, unsigned int length)
{
    CySecDrv_SHA_StatusType result = SHA_SUCCESS;

    if (!input)
    {
        result = SHA_NULL;
    }

    if (context.Computed)
    {
        context.Corrupted = SHA_STATE_ERROR;

        result = SHA_STATE_ERROR;
    }

    if (context.Corrupted)
    {
        result = SHA_NULL;
    }
    while (length-- && !context.Corrupted)
    {
        context.Message_Block[context.Message_Block_Index++] =
            (*input & 0xFF);

        context.Length_Low += 8;
        if (context.Length_Low == 0)
        {
            context.Length_High++;
            if (context.Length_High == 0)
            {
                /* Message is too long */
                context.Corrupted = 1;
            }
        }

        if (context.Message_Block_Index == 64)
        {
            SHA1ProcessMessageBlock();
        }

        input++;
    }

    return result;
}

/*
 *  SHA1ProcessMessageBlock
 *
 *  Description:
 *      This function will process the next 512 bits of the message
 *      stored in the Message_Block array.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      Many of the variable names in this code, especially the
 *      single character names, were used because those were the
 *      names used in the publication.
 *
 *
 */
void SHA1ProcessMessageBlock(void)
{
    const uint32_t K[] = {/* Constants defined in SHA-1   */
                          0x5A827999,
                          0x6ED9EBA1,
                          0x8F1BBCDC,
                          0xCA62C1D6};
    int t;                  /* Loop counter                */
    uint32_t temp;          /* Temporary word value        */
    uint32_t W[80];         /* Word sequence               */
    uint32_t A, B, C, D, E; /* Word buffers                */

    /*
     *  Initialize the first 16 words in the array W
     */
    for (t = 0; t < 16; t++)
    {
        W[t] = ((uint32_t)context.Message_Block[t * 4]) << 24;
        W[t] |= ((uint32_t)context.Message_Block[t * 4 + 1]) << 16;
        W[t] |= ((uint32_t)context.Message_Block[t * 4 + 2]) << 8;
        W[t] |= ((uint32_t)context.Message_Block[t * 4 + 3]);
    }

    for (t = 16; t < 80; t++)
    {
        W[t] = SHA1CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
    }

    A = context.Intermediate_Hash[0];
    B = context.Intermediate_Hash[1];
    C = context.Intermediate_Hash[2];
    D = context.Intermediate_Hash[3];
    E = context.Intermediate_Hash[4];

    for (t = 0; t < 20; t++)
    {
        temp = SHA1CircularShift(5, A) +
               ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }

    for (t = 20; t < 40; t++)
    {
        temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }

    for (t = 40; t < 60; t++)
    {
        temp = SHA1CircularShift(5, A) +
               ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }

    for (t = 60; t < 80; t++)
    {
        temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = SHA1CircularShift(30, B);
        B = A;
        A = temp;
    }

    context.Intermediate_Hash[0] += A;
    context.Intermediate_Hash[1] += B;
    context.Intermediate_Hash[2] += C;
    context.Intermediate_Hash[3] += D;
    context.Intermediate_Hash[4] += E;

    context.Message_Block_Index = 0;
}

/*
 *  SHA1PadMessage
 *
 *  Description:
 *      According to the standard, the message must be padded to an even
 *      512 bits.  The first padding bit must be a '1'.  The last 64
 *      bits represent the length of the original message.  All bits in
 *      between should be 0.  This function will pad the message
 *      according to those rules by filling the Message_Block array
 *      accordingly.  It will also call the ProcessMessageBlock function
 *      provided appropriately.  When it returns, it can be assumed that
 *      the message digest has been computed.
 *  Returns:
 *      Nothing.
 *
 */

void SHA1PadMessage(void)
{
    /*
     *  Check to see if the current message block is too small to hold
     *  the initial padding bits and length.  If so, we will pad the
     *  block, process it, and then continue padding into a second
     *  block.
     */
    if (context.Message_Block_Index > 55)
    {
        context.Message_Block[context.Message_Block_Index++] = 0x80;
        while (context.Message_Block_Index < 64)
        {
            context.Message_Block[context.Message_Block_Index++] = 0;
        }

        SHA1ProcessMessageBlock();

        while (context.Message_Block_Index < 56)
        {
            context.Message_Block[context.Message_Block_Index++] = 0;
        }
    }
    else
    {
        context.Message_Block[context.Message_Block_Index++] = 0x80;
        while (context.Message_Block_Index < 56)
        {

            context.Message_Block[context.Message_Block_Index++] = 0;
        }
    }

    /*
     *  Store the message length as the last 8 octets
     */
    context.Message_Block[56] = context.Length_High >> 24;
    context.Message_Block[57] = context.Length_High >> 16;
    context.Message_Block[58] = context.Length_High >> 8;
    context.Message_Block[59] = context.Length_High;
    context.Message_Block[60] = context.Length_Low >> 24;
    context.Message_Block[61] = context.Length_Low >> 16;
    context.Message_Block[62] = context.Length_Low >> 8;
    context.Message_Block[63] = context.Length_Low;

    SHA1ProcessMessageBlock();
}
