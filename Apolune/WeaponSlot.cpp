#include "WeaponSlot.h"

#include "Vehicle.h"

void WeaponSlots::Fire( Vehicle *v, VehicleModule *vm ) {
 FOREACH(WeaponSlot,slot) slot->Fire(v,vm);
}

void WeaponSlot::Fire( Vehicle *v, VehicleModule *vm ) {
 if ( mounted ) mounted->Fire(v,vm,this);
}

void WeaponSlots::Fire( Vehicle *v, VehicleModule *vm, AttachmentPoint *ap, VehicleModule *vm2 ) {
 FOREACH(WeaponSlot,slot) slot->Fire(v,vm,ap,vm2);
}

void WeaponSlot::Fire( Vehicle *v, VehicleModule *vm, AttachmentPoint *ap, VehicleModule *vm2 ) {
 if ( mounted ) mounted->Fire(v,vm,ap,vm2,this);
}
