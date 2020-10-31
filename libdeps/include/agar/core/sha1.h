/*
 * SHA-1 in C
 * By Steve Reid <steve@edmweb.com>
 * 100% Public Domain
 */

#include <agar/core/begin.h>

#define AG_SHA1_BLOCK_LENGTH		64
#define AG_SHA1_DIGEST_LENGTH		20
#define AG_SHA1_DIGEST_STRING_LENGTH	(AG_SHA1_DIGEST_LENGTH * 2 + 1)

typedef struct ag_sha1_ctx {
	Uint32 state[5];
	Uint64 count;
	Uint8 buffer[AG_SHA1_BLOCK_LENGTH];
} AG_SHA1_CTX;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void AG_SHA1Init(AG_SHA1_CTX *);
extern DECLSPEC void AG_SHA1Pad(AG_SHA1_CTX *);
extern DECLSPEC void AG_SHA1Transform(Uint32 [5], const Uint8 [AG_SHA1_BLOCK_LENGTH]);
extern DECLSPEC void AG_SHA1Update(AG_SHA1_CTX *, const Uint8 *, size_t) BOUNDED_ATTRIBUTE(__string__,2,3);
extern DECLSPEC void AG_SHA1Final(Uint8 [AG_SHA1_DIGEST_LENGTH], AG_SHA1_CTX *);
extern DECLSPEC char *AG_SHA1End(AG_SHA1_CTX *, char *);
extern DECLSPEC char *AG_SHA1Data(const Uint8 *, size_t, char *) BOUNDED_ATTRIBUTE(__string__,1,2);
__END_DECLS
/* Close generated block */

#include <agar/core/close.h>
