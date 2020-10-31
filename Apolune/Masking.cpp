#include "Masking.h"
#include "Material.h"

extern Materials materials;

void Masker2d::init() {
 materials.Append(maskShader=new MaskShader);
}

Masker2d masker2d;