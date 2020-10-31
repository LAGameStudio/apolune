#pragma once
#include "ListItem.h"
#include "Indexed.h"
#include "Strings.h"

SINGLE_NAMED(CSV,{})
 Indexed<Zstring> table;
 CSV( const char *s, char delimitor, bool trimValues=true ) : ListItem() { fromString(s,delimitor,trimValues); }
 CSV( const char *s, unsigned int w, char delimitor, bool trimValues=true ) : ListItem() { fromString(s,w,delimitor,trimValues); }
 CSV( const char *n, const char *s, char delimitor=',', bool trimValues=true ) : ListItem() { name=n; fromString(s,delimitor,trimValues); }
 CSV( const char *n, const char *s, unsigned int w, char delimitor=',', bool trimValues=true ) : ListItem() { name=n; fromString(s,w,delimitor,trimValues); }
 void fromString( const char *s, unsigned int w, char delimitor=',', bool trimValues=true ) {
  Zstring in(s);
  in("\r","");
  Strings lines;
  lines.SplitPush(in.c_str());
  if ( table.w < w ) table.IncreaseW((size_t)w-table.w);
  if ( trimValues ) {
   EACH(lines.first,String,row) {
    Strings values;
    values.SplitPush(row->s.c_str(),delimitor);
    if( values.count > 0 ) {
     unsigned int cursor=(unsigned int)table.w;
     table.IncreaseH();
     unsigned int i=0;
     EACH(values.first,String,column) {
      table(cursor,i)=column->s;
      i++;
     }
    }
   }
  } else {
   EACH(lines.first,String,row) {
    Strings values;
    values.SplitPush(row->s.c_str(),delimitor);
    if( values.count > 0 ) {
     unsigned int cursor=(unsigned int)table.w;
     table.IncreaseH();
     unsigned int i=0;
     EACH(values.first,String,column) {
      table(cursor,i)=column->s;
      table(cursor,i).Trim();
      i++;
     }
    }
   }
  }
 }
 void fromString( const char *s, char delimitor=',', bool trimValues=true ) {
  Zstring in(s);
  in("\r","");
  Strings lines;
  lines.SplitPush(in.c_str());
  if ( trimValues ) {
   EACH(lines.first,String,row) {
    Strings values;
    values.SplitPush(row->s.c_str(),delimitor);
    if( values.count > 0 ) {
     if( values.count > table.w ) {
      table.IncreaseW(((size_t)(int)values.count)-table.w);
     }
     unsigned int cursor=(unsigned int)table.w;
     table.IncreaseH();
     unsigned int i=0;
     EACH(values.first,String,column) {
      table(cursor,i)=column->s;
      i++;
     }
    }
   }
  } else {
   EACH(lines.first,String,row) {
    Strings values;
    values.SplitPush(row->s.c_str(),delimitor);
    if( values.count > 0 ) {
     if( values.count > table.w ) {
      table.IncreaseW(((size_t)(int)values.count)-table.w);
     }
     unsigned int cursor=(unsigned int)table.w;
     table.IncreaseH();
     unsigned int i=0;
     EACH(values.first,String,column) {
      table(cursor,i)=column->s;
      table(cursor,i).Trim();
      i++;
     }
    }
   }
  }
 }
PLURAL_NAMED(CSV,CSVHandle,CSVHandles,"CSV",Spreadsheet,{}) 
ENDSET_NAMED(Spreadsheet);

// The following extends LinkedList.h macros.  Use in MANY clause.
// Provides vanilla fromCSV method using this macro.
#define FROMCSV(single,string_name,assignments) \
 void fromCSV( const char *in ) {\
  CSV sheet(string_name,in);\
  int id=count;\
  for ( unsigned int i=0; i<(unsigned int)sheet.table.h; i++ ) {\
   Commodity *row=new single(); Append(row);\
   { assignments } id++;\
  }\
 }
