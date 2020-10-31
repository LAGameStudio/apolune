/*	Public domain	*/

#ifndef _AGAR_CORE_CONFIG_H_
#define _AGAR_CORE_CONFIG_H_
#include <agar/core/begin.h>

typedef struct ag_object AG_Config;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_Config *agConfig;
extern DECLSPEC AG_ObjectClass agConfigClass;
extern DECLSPEC int agKbdDelay, agKbdRepeat;
extern DECLSPEC int agMouseDblclickDelay, agMouseSpinDelay, agMouseSpinIval;
extern DECLSPEC int agTextComposition, agTextBidi, agTextTabWidth, agTextBlinkRate, agPageIncrement, agTextSymbols, agTextCache;
extern DECLSPEC int agIdleThresh;
extern DECLSPEC int agScreenshotQuality;
extern DECLSPEC int agWindowAnySize;
extern DECLSPEC int agMsgDelay;
extern DECLSPEC int AG_ConfigInit(AG_Config *, Uint);
extern DECLSPEC int AG_ConfigFile(const char *, const char *, const char *, char *, size_t) BOUNDED_ATTRIBUTE(__string__, 4, 5);
extern DECLSPEC int AG_CreateDataDir(void);
extern DECLSPEC int AG_ConfigSave(void);
extern DECLSPEC int AG_ConfigLoad(void);
extern DECLSPEC void AG_SetCfgString(const char *, const char *, ...);
__END_DECLS
/* Close generated block */

#define AG_CfgDefined(k)            AG_Defined(agConfig,(k))
#define AG_CfgUint(k)               AG_GetUint(agConfig,(k))
#define AG_CfgInt(k)                AG_GetInt(agConfig,(k))
#define AG_CfgBool(k)               AG_GetBool(agConfig,(k))
#define AG_CopyCfgString(k,dst,len) AG_GetString(agConfig,(k),(dst),(len))
#define AG_CfgString(k)             AG_GetStringDup(agConfig,(k))

#define AG_SetCfgUint(k,v) AG_SetUint(agConfig,(k),(v))
#define AG_SetCfgInt(k,v) AG_SetInt(agConfig,(k),(v))
#define AG_SetCfgBool(k,v) AG_SetBool(agConfig,(k),(v))

#include <agar/core/close.h>
#endif /* _AGAR_CORE_CONFIG_H_ */
