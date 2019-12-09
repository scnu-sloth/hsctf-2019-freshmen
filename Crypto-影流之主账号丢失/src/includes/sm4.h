/* ====================================================================
 * Copyright (c) 2014 - 2017 The GmSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the GmSSL Project.
 *    (http://gmssl.org/)"
 *
 * 4. The name "GmSSL Project" must not be used to endorse or promote
 *    products derived from this software without prior written
 *    permission. For written permission, please contact
 *    guanzhi1980@gmail.com.
 *
 * 5. Products derived from this software may not be called "GmSSL"
 *    nor may "GmSSL" appear in their names without prior written
 *    permission of the GmSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the GmSSL Project
 *    (http://gmssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE GmSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE GmSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */

#ifndef HEADER_SM4_H
#define HEADER_SM4_H

#ifndef NO_GMSSL

# define SM4_ENCRYPT     1
# define SM4_DECRYPT     0

#define SM4_KEY_LENGTH		16
#define SM4_BLOCK_SIZE		16
#define SM4_IV_LENGTH		(SM4_BLOCK_SIZE)
#define SM4_NUM_ROUNDS		32

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <modes.h>
#include "stdlib.h"
#include "stdio.h"

// #define SIMPLE_EXTERNAL_ENCODINGS

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sm4_key_t {
	uint32_t rk[SM4_NUM_ROUNDS];
#ifdef SIMPLE_EXTERNAL_ENCODINGS
    uint8_t G[SM4_NUM_ROUNDS+1][256];
    uint8_t F[SM4_NUM_ROUNDS+1][256];
#endif //SIMPLE_EXTERNAL_ENCODINGS
} sm4_key_t;

typedef struct sm4_key_t SM4_KEY;

/**
 sm4_set_encrypt_key

 @param key SM4_KEY
 @param user_key key
 */
void sm4_set_encrypt_key(sm4_key_t *key, const unsigned char *user_key);

/**
 sm4_set_decrypt_key

 @param key SM4_KEY
 @param user_key key
 */
void sm4_set_decrypt_key(sm4_key_t *key, const unsigned char *user_key);

/**
 sm4_encrypt

 @param in in
 @param out out
 @param key SM4_KEY
 */
void sm4_encrypt(const unsigned char *in, unsigned char *out, const sm4_key_t *key);
#define sm4_decrypt(in,out,key)  sm4_encrypt(in,out,key)


/**
 sm4_cbc_encrypt

 @param in in
 @param out out
 @param len byte size of in
 @param key key
 @param iv iv
 @param enc 1 to SM4_ENCRYPT, 0 to SM4_DECRYPT
 */
void sm4_cbc_encrypt(const unsigned char *in, unsigned char *out,
	size_t len, const sm4_key_t *key, unsigned char *iv, int enc);


#ifdef __cplusplus
}
#endif
#endif
#endif
