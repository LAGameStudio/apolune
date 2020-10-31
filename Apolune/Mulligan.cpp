#include "Mulligan.h"

void PFX::LoadTrailEffect( const char *w ) {
 trailsEffect.Recycle(new PFX);
 trailsEffect->fromString(w);
}