/*	Public domain	*/

#ifndef _AGAR_WIDGET_UNITS_H_
#define _AGAR_WIDGET_UNITS_H_

#include <agar/config/historical_units.h>

#include <agar/gui/begin.h>

typedef struct ag_unit {
	char *key;		/* Key */
	char *abbr;		/* Symbol */
	char *name;		/* Long name */
	double divider;		/* Base unit divider (for linear conv) */
	double (*func)(double, int);	/* For nonlinear conversions */
} AG_Unit;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC const AG_Unit *AG_FindUnit(const char *);
extern DECLSPEC const AG_Unit *AG_BestUnit(const AG_Unit[], double);
extern DECLSPEC int AG_UnitFormat(double, const AG_Unit[], char *, size_t);
extern DECLSPEC double AG_UnitFahrenheit(double, int);
extern DECLSPEC double AG_UnitCelsius(double, int);
#ifdef HISTORICAL_UNITS
extern DECLSPEC double AG_UnitRankine(double, int);
extern DECLSPEC double AG_UnitReaumur(double, int);
#endif
extern DECLSPEC const AG_Unit *agUnitGroups[];
extern DECLSPEC const char *agUnitGroupNames[];
extern DECLSPEC const int agnUnitGroups;
extern DECLSPEC const AG_Unit agIdentityUnit[];
extern DECLSPEC const AG_Unit agLengthUnits[];
extern DECLSPEC const AG_Unit agAngleUnits[];
extern DECLSPEC const AG_Unit agVideoUnits[];
extern DECLSPEC const AG_Unit agAreaUnits[];
extern DECLSPEC const AG_Unit agVolumeUnits[];
extern DECLSPEC const AG_Unit agSpeedUnits[];
extern DECLSPEC const AG_Unit agMassUnits[];
extern DECLSPEC const AG_Unit agTimeUnits[];
extern DECLSPEC const AG_Unit agCurrentUnits[];
extern DECLSPEC const AG_Unit agTemperatureUnits[];
extern DECLSPEC const AG_Unit agSubstanceAmountUnits[];
extern DECLSPEC const AG_Unit agLightUnits[];
extern DECLSPEC const AG_Unit agPowerUnits[];
extern DECLSPEC const AG_Unit agEMFUnits[];
extern DECLSPEC const AG_Unit agResistanceUnits[];
extern DECLSPEC const AG_Unit agResistanceTC1Units[];
extern DECLSPEC const AG_Unit agResistanceTC2Units[];
extern DECLSPEC const AG_Unit agCapacitanceUnits[];
extern DECLSPEC const AG_Unit agInductanceUnits[];
extern DECLSPEC const AG_Unit agFrequencyUnits[];
extern DECLSPEC const AG_Unit agPressureUnits[];
extern DECLSPEC const AG_Unit agMetabolicExpenditureUnits[];
extern DECLSPEC const AG_Unit agPercentageUnits[];
extern DECLSPEC const AG_Unit agKUnits[];
extern DECLSPEC const AG_Unit agVacuumUnits[];

static __inline__ double
AG_Unit2Base(double n, const AG_Unit *unit)
{
	return (unit->func != NULL ? unit->func(n, 1) : n*unit->divider);
}

static __inline__ double
AG_Base2Unit(double n, const AG_Unit *unit)
{
	return (unit->func != NULL ? unit->func(n, 0) : n/unit->divider);
}

static __inline__ double
AG_Unit2Unit(double n, const AG_Unit *ufrom, const AG_Unit *uto)
{
	return (AG_Base2Unit(AG_Unit2Base(n, ufrom), uto));
}

static __inline__ const char *
AG_UnitAbbr(const AG_Unit *unit)
{
	return (unit->abbr[0] != '\0' ? unit->abbr : unit->key);
}
#define AG_Unit2Basef(n, u) ((float)AG_Unit2Base((float)(n), (u)))
#define AG_Base2Unitf(n, u) ((float)AG_Base2Unit((float)(n), (u)))
#define AG_Unit2Unitf(n, u1, u2) ((float)AG_Unit2Unit((float)(n), (u1), (u2)))
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_UNITS_H_ */
