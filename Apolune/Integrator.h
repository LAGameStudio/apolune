#pragma once

class Integrator
{
public:
 Integrator(void) {}
 ~Integrator(void) {}

 virtual void Init( void *ps ) {}
 virtual void step( float t ) {}
 virtual Integrator *Copy() { return new Integrator; }
};