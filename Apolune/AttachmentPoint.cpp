#include "AttachmentPoint.h"

#include "Campaign.h"

void AttachmentPoint::AttachModule() {
 if ( moduleString.length > 0 ) {
  module=new VehicleModule;
  module->fromString(moduleString.c_str());
 }
 if ( moduleBlueprintString.length > 0 ) {
  moduleBlueprint=veh_mbs.firstNamed(moduleBlueprintString.c_str());
 }
}

void AttachmentPoint::AttachModule( VehicleModule *in ) {
 module=in;
}

void AttachmentPoint::AttachModule( VehicleModuleBlueprint *vmb ) {
 moduleBlueprint=vmb;
}

string AttachmentPoint::GetModuleReference() {
 if ( module ) return module->toString(); 
 return string("");
}

string AttachmentPoint::GetModuleBlueprintReference() {
 if ( moduleBlueprint ) return moduleBlueprint->name; 
 return string("");
}


bool AttachmentPoint::HasModule() {
 if ( module ) return true;
 return false;
}

bool AttachmentPoint::HasModuleBlueprint() {
 if ( moduleBlueprint ) return true;
 return false;
}

void AttachmentPoint::DuplicateModule( AttachmentPoint *a ) {
 if ( module ) a->module.Recycle(module->Duplicate());
}

void AttachmentPoints::ParseReferences() {
 FOREACH(AttachmentPoint,a) {
  a->module.Recycle();
  a->module->fromString(a->moduleString.c_str());
  if ( a->moduleBlueprintString.length > 0 ) 
   a->moduleBlueprint=veh_mbs.firstNamed(a->moduleBlueprintString.c_str());
 }
}


void AttachmentPoint::Fire( Vehicle *v, VehicleModule *vm ) {
 module->Fire(v,vm,this);
}

void AttachmentPoints::Fire( Vehicle *v, VehicleModule *vm ) {
 FOREACH(AttachmentPoint,a) a->Fire(v,vm);
}