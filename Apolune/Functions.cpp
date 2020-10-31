#include "Functions.h"
#include "Scripting.h"

MathOperator StringToOperator( char op ) {
 if ( op == '+' ) return mo_add;
 if ( op == '-' ) return mo_sub;
 if ( op == '/' ) return mo_div;
 if ( op == '*' ) return mo_mul;
 if ( op == '%' ) return mo_mod;
 if ( op == '<' ) return mo_less;
 if ( op == '>' ) return mo_more;
 if ( op == '=' ) return mo_equal;
 return mo_unknown;
}

MathOperator StringToOperator( const char *op ) {
 if ( *op == '+' ) return mo_add;
 if ( *op == '-' ) return mo_sub;
 if ( *op == '/' ) return mo_div;
 if ( *op == '*' ) return mo_mul;
 if ( *op == '%' ) return mo_mod;
 if ( *op == '=' ) return mo_equal;
 if ( !str_cmp(op,"<=") ) return mo_lessEq;
 if ( !str_cmp(op,">=") ) return mo_moreEq;
 if ( *op == '<' ) return mo_less;
 if ( *op == '>' ) return mo_more;
 return mo_unknown;
}

void BoundFunction::EvalMath( MathOperator op, Consulate *es, const char *a, const char *b, const char *t ) {
 // Figure out what a and b actually are

 Zp<Variadic> A;
 Variadic inline_a;
 VariadicType type_a=GuessType(a);
 if ( type_a == v_is_reference ) {
  A=es->variables->Get(a+1); // advance past ^
  if ( !A ) {
   A=&inline_a;
   inline_a.string_value.Instantiate();
   (*inline_a.string_value)="";
   inline_a.type=v_string;
  } // Create an empty string (no variable found)
 } else if ( type_a == v_is_decimal ) {
  A=&inline_a;
  inline_a.decimal_value=atof(a);
  inline_a.type=v_decimal;
 } else if ( type_a == v_is_integer ) {
  A=&inline_a;
  inline_a.integer_value=atoi(a);
  inline_a.type=v_integer;
 } else {
  A=&inline_a;
  inline_a.string_value.Instantiate();
  (*inline_a.string_value)=a;
  inline_a.type=v_string;  
 }

 Zp<Variadic> B;
 Variadic inline_b;
 VariadicType type_b=GuessType(b);
 if ( type_b == v_is_reference ) {
  B=es->variables->Get(b+1); // advance past ^
  if ( !B ) {
   B=&inline_b;
   inline_b.string_value.Instantiate();
   (*inline_b.string_value)="";
   inline_b.type=v_string;
  } // Create an empty string (no variable found)
 } else if ( type_b == v_is_decimal ) {
  B=&inline_b;
  inline_b.decimal_value=atof(b);
  inline_b.type=v_decimal;
 } else if ( type_b == v_is_integer ) {
  B=&inline_b;
  inline_b.integer_value=atoi(b);
  inline_b.type=v_integer;
 } else {
  B=&inline_b;
  inline_b.string_value.Instantiate();
  (*inline_b.string_value)=b;
  inline_b.type=v_string;  
 }

 // Now figure out what to do with what they are

 if ( type_a == type_b ) { // Identical types
  if ( type_a == v_integer ) { // Integer vs. Integer
   switch ( op ) {
    case mo_add: es->variables->Set(t, A->integer_value+B->integer_value); return;
    case mo_mul: es->variables->Set(t, A->integer_value*B->integer_value); return;
    case mo_sub: es->variables->Set(t, A->integer_value-B->integer_value); return;
    case mo_div: es->variables->Set(t, A->integer_value/B->integer_value); return;
    case mo_mod: es->variables->Set(t, A->integer_value%B->integer_value); return;
    case mo_less: es->variables->Set(t, A->integer_value<B->integer_value?1:0); return;
    case mo_more: es->variables->Set(t, A->integer_value>B->integer_value?1:0); return;
    case mo_lessEq: es->variables->Set(t, A->integer_value<=B->integer_value?1:0); return;
    case mo_moreEq: es->variables->Set(t, A->integer_value>=B->integer_value?1:0); return;
    case mo_equal: es->variables->Set(t, A->integer_value==B->integer_value?1:0); return;
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_decimal ) { // Decimal vs. Decimal
   switch ( op ) {
    case mo_add: es->variables->Set(t, A->decimal_value+B->decimal_value); return;
    case mo_mul: es->variables->Set(t, A->decimal_value*B->decimal_value); return;
    case mo_sub: es->variables->Set(t, A->decimal_value-B->decimal_value); return;
    case mo_div: es->variables->Set(t, A->decimal_value/B->decimal_value); return;
    case mo_mod: es->variables->Set(t, fmod(A->decimal_value,B->decimal_value) ); return;
    case mo_less: es->variables->Set(t, A->decimal_value<B->decimal_value?1:0); return;
    case mo_more: es->variables->Set(t, A->decimal_value>B->decimal_value?1:0); return;
    case mo_lessEq: es->variables->Set(t, A->decimal_value<=B->decimal_value?1:0); return;
    case mo_moreEq: es->variables->Set(t, A->decimal_value>=B->decimal_value?1:0); return;
    case mo_equal: es->variables->Set(t, A->decimal_value==B->decimal_value?1:0); return;
    default: /* no recognizable op */ return;
   }
  } else if ( type_a == v_cartesian ) { // Cartesian vs. Cartesian
   switch ( op ) {
    case mo_add: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveBy(B->cartesian->x,B->cartesian->y); es->variables->Set(t, d); } return;
    case mo_mul: { Cartesiand e; e.Set((double)B->cartesian->fx,(double)B->cartesian->fy); Cartesian *d=new Cartesian; d->Set(A->cartesian); d->Multiply(&e); es->variables->Set(t, d); } return;
    case mo_sub: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveBy(-B->cartesian->x,-B->cartesian->y); es->variables->Set(t, d); } return;
    case mo_div: { Cartesian *d=new Cartesian; d->Set(A->cartesian); if ( B->cartesian->fx != 0 ) d->x=(int)((float)d->x/B->cartesian->fx); if ( B->cartesian->fy != 0 ) d->y=(int)((float)d->y/B->cartesian->fy); d->SetRect(d->x,d->y,d->w,d->h); es->variables->Set(t, d); } return;
    case mo_mod: return;
    case mo_less: es->variables->Set(t, A->cartesian->Intersects(B->cartesian)?1:0); return;
    case mo_more: es->variables->Set(t, B->cartesian->Intersects(A->cartesian)?1:0); return;
    case mo_lessEq: es->variables->Set(t, A->cartesian->w*A->cartesian->h<=B->cartesian->w*B->cartesian->h?1:0); return;
    case mo_moreEq: es->variables->Set(t, A->cartesian->w*A->cartesian->h>=B->cartesian->w*B->cartesian->h?1:0); return;
    case mo_equal: es->variables->Set(t,
                   A->cartesian->x==B->cartesian->x
                && A->cartesian->y==B->cartesian->y
                && A->cartesian->x2==B->cartesian->x2
                && A->cartesian->y2==B->cartesian->y2?1:0); return;
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_crayon ) { // Crayon vs. Crayon
   switch ( op ) {
    case mo_add: { Crayon d; d.AddColorf(*A->crayon,*B->crayon); es->variables->Set(t, d); } return;
    case mo_mul: { Crayon d; d.Multiply(*A->crayon,*B->crayon); es->variables->Set(t, d); } return;
    case mo_sub: { Crayon d; d.SubColorf(*A->crayon,*B->crayon); es->variables->Set(t, d); } return;
    case mo_div: { Crayon d; d.fromCrayon(*(A->crayon)); d.MultiplyClampRGB(B->crayon->rf,B->crayon->gf,B->crayon->bf); es->variables->Set(t, d); } return;
    case mo_mod: { Crayon d; d.Multiply(*A->crayon,*B->crayon,true); es->variables->Set(t, d); } return;
    case mo_less: es->variables->Set(t, (*A->crayon).Luminance()<(*B->crayon).Luminance()?1:0); return;
    case mo_more: es->variables->Set(t, (*A->crayon).Luminance()>(*B->crayon).Luminance()?1:0); return;
    case mo_lessEq: es->variables->Set(t, (*A->crayon).Luminance()<=(*B->crayon).Luminance()?1:0); return;
    case mo_moreEq: es->variables->Set(t, (*A->crayon).Luminance()>=(*B->crayon).Luminance()?1:0); return;
    case mo_equal: es->variables->Set(t, (*A->crayon).Luminance()==(*B->crayon).Luminance()?1:0); return;
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_string ) { // String vs. String
   switch ( op ) {
    case mo_add: es->variables->Set(t, ((*A->string_value)+(*B->string_value)).c_str() ); return;
    case mo_mul: es->variables->Set(t, ((*A->string_value)*(*B->string_value)) ); return;
    case mo_sub: es->variables->Set(t, ((*A->string_value)-(*B->string_value)).c_str() ); return;
    case mo_div: es->variables->Set(t, ((*A->string_value)/(*B->string_value)).c_str() ); return;
    case mo_mod: es->variables->Set(t, A->string_value->contains(B->string_value->c_str()) ?1:0); return;
    case mo_less: es->variables->Set(t, A->string_value->Compare(B->string_value->c_str()) < 0?1:0); return;
    case mo_more: es->variables->Set(t, A->string_value->Compare(B->string_value->c_str()) > 0?1:0); return;
    case mo_lessEq: es->variables->Set(t, A->string_value->Compare(B->string_value->c_str()) >= 0?1:0); return;
    case mo_moreEq: es->variables->Set(t, A->string_value->Compare(B->string_value->c_str()) >= 0?1:0); return;
    case mo_equal: es->variables->Set(t, !str_cmp(A->string_value->c_str(),B->string_value->c_str()) ?1:0); return;
    default: /* no recognizable op */ return;
   }
  } else { /* An outlying type (or unknown type) that cannot be arithmetically added such as an image, blend, tween or other singleton */ }
 } else { // Not identical types, but both variadics are present
         if ( type_a == v_integer   && type_b == v_decimal   ) { // int dec
   switch ( op ) {
    case mo_add: es->variables->Set(t, ((double)A->integer_value+B->decimal_value)); return;
    case mo_mul: es->variables->Set(t, ((double)A->integer_value*B->decimal_value)); return;
    case mo_sub: es->variables->Set(t, ((double)A->integer_value-B->decimal_value)); return;
    case mo_div: es->variables->Set(t, ((double)A->integer_value/B->decimal_value)); return;
    case mo_mod: es->variables->Set(t, fmod((double)A->integer_value,B->decimal_value)); return;
    case mo_less: es->variables->Set(t, (double)A->integer_value<B->decimal_value?1:0); return;
    case mo_more: es->variables->Set(t, (double)A->integer_value>B->decimal_value?1:0); return;
    case mo_lessEq: es->variables->Set(t, (double)A->integer_value<=B->decimal_value?1:0); return;
    case mo_moreEq: es->variables->Set(t, (double)A->integer_value>=B->decimal_value?1:0); return;
    case mo_equal: es->variables->Set(t, (double)A->integer_value==B->decimal_value?1:0); return;
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_decimal   && type_b == v_integer   ) { // dec int
   switch ( op ) {
    case mo_add: es->variables->Set(t, (A->decimal_value+(double)B->integer_value)); return;
    case mo_mul: es->variables->Set(t, (A->decimal_value*(double)B->integer_value)); return;
    case mo_sub: es->variables->Set(t, (A->decimal_value-(double)B->integer_value)); return;
    case mo_div: es->variables->Set(t, (A->decimal_value/(double)B->integer_value)); return;
    case mo_mod: es->variables->Set(t, fmod(A->decimal_value,(double)B->integer_value)); return;
    case mo_less: es->variables->Set(t, A->decimal_value<(double)B->integer_value?1:0); return;
    case mo_more: es->variables->Set(t, A->decimal_value>(double)B->integer_value?1:0); return;
    case mo_lessEq: es->variables->Set(t, A->decimal_value<=(double)B->integer_value?1:0); return;
    case mo_moreEq: es->variables->Set(t, A->decimal_value>=(double)B->integer_value?1:0); return;
    case mo_equal: es->variables->Set(t, A->decimal_value==(double)B->integer_value?1:0); return;
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_string    && type_b == v_integer   ) { // str int
   if ( is_decimal(A->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->string_value->decimal()+(double)B->integer_value); return;
     case mo_mul: es->variables->Set(t, A->string_value->decimal()*(double)B->integer_value); return;
     case mo_sub: es->variables->Set(t, A->string_value->decimal()-(double)B->integer_value); return;
     case mo_div: es->variables->Set(t, A->string_value->decimal()/(double)B->integer_value); return;
     case mo_mod: es->variables->Set(t, fmod(A->string_value->decimal(),(double)B->integer_value)); return;
     case mo_less: es->variables->Set(t, A->string_value->decimal()<(double)B->integer_value?1:0); return;
     case mo_more: es->variables->Set(t, A->string_value->decimal()>(double)B->integer_value?1:0); return;
     case mo_lessEq: es->variables->Set(t, A->string_value->decimal()<=(double)B->integer_value?1:0); return;
     case mo_moreEq: es->variables->Set(t, A->string_value->decimal()>=(double)B->integer_value?1:0); return;
     case mo_equal: es->variables->Set(t, A->string_value->decimal()==(double)B->integer_value?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else if ( is_integer(A->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->string_value->integer()+B->integer_value); return;
     case mo_mul: es->variables->Set(t, A->string_value->integer()*B->integer_value); return;
     case mo_sub: es->variables->Set(t, A->string_value->integer()-B->integer_value); return;
     case mo_div: es->variables->Set(t, A->string_value->integer()/B->integer_value); return;
     case mo_mod: es->variables->Set(t, A->string_value->integer()%B->integer_value); return;
     case mo_less: es->variables->Set(t, A->string_value->integer()<B->integer_value?1:0); return;
     case mo_more: es->variables->Set(t, A->string_value->integer()>B->integer_value?1:0); return;
     case mo_lessEq: es->variables->Set(t, A->string_value->integer()<=B->integer_value?1:0); return;
     case mo_moreEq: es->variables->Set(t, A->string_value->integer()>=B->integer_value?1:0); return;
     case mo_equal: es->variables->Set(t, A->string_value->integer()==B->integer_value?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else {
    switch ( op ) {
     case mo_add: es->variables->Set(t, (A->string_value->value+FORMAT("%d",B->integer_value)).c_str()); return;
     case mo_sub: { Zstring A_Dup(A->string_value->value.c_str()); A_Dup-=B->integer_value.value; es->variables->Set(t, A_Dup.c_str() ); } break;
     case mo_mul: // unsupported, no op
     case mo_div: // unsupported, no op
     case mo_mod: // unsupported, no op
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_integer   && type_b == v_string    ) { // int str
   if ( is_decimal(B->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, (double)A->integer_value+B->string_value->decimal()); return;
     case mo_mul: es->variables->Set(t, (double)A->integer_value*B->string_value->decimal()); return;
     case mo_sub: es->variables->Set(t, (double)A->integer_value-B->string_value->decimal()); return;
     case mo_div: es->variables->Set(t, (double)A->integer_value/B->string_value->decimal()); return;
     case mo_mod: es->variables->Set(t, fmod((double)A->integer_value,B->string_value->decimal())); return;
     case mo_less: es->variables->Set(t,(double)A->integer_value<B->string_value->decimal()?1:0); return;
     case mo_more: es->variables->Set(t,(double)A->integer_value>B->string_value->decimal()?1:0); return;
     case mo_lessEq: es->variables->Set(t,(double)A->integer_value<=B->string_value->decimal()?1:0); return;
     case mo_moreEq: es->variables->Set(t,(double)A->integer_value>=B->string_value->decimal()?1:0); return;
     case mo_equal: es->variables->Set(t,(double)A->integer_value==B->string_value->decimal()?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else if ( is_integer(B->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->integer_value+B->string_value->integer()); return;
     case mo_mul: es->variables->Set(t, A->integer_value*B->string_value->integer()); return;
     case mo_sub: es->variables->Set(t, A->integer_value-B->string_value->integer()); return;
     case mo_div: es->variables->Set(t, A->integer_value/B->string_value->integer()); return;
     case mo_mod: es->variables->Set(t, A->integer_value%B->string_value->integer()); return;
     case mo_less: es->variables->Set(t, A->integer_value<B->string_value->integer()?1:0); return;
     case mo_more: es->variables->Set(t, A->integer_value>B->string_value->integer()?1:0); return;
     case mo_lessEq: es->variables->Set(t, A->integer_value<=B->string_value->integer()?1:0); return;
     case mo_moreEq: es->variables->Set(t, A->integer_value>=B->string_value->integer()?1:0); return;
     case mo_equal: es->variables->Set(t, A->integer_value==B->string_value->integer()?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else {
    switch ( op ) {
     case mo_add: es->variables->Set(t, (FORMAT("%d",A->integer_value.value)+B->string_value->value).c_str()); return;
     case mo_mul: // unsupported, no op
     case mo_sub: // unsupported, no op
     case mo_div: // unsupported, no op
     case mo_mod: // unsupported, no op
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_string    && type_b == v_decimal   ) { // str dec
   if ( is_decimal(A->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->string_value->decimal()+B->decimal_value); return;
     case mo_mul: es->variables->Set(t, A->string_value->decimal()*B->decimal_value); return;
     case mo_sub: es->variables->Set(t, A->string_value->decimal()-B->decimal_value); return;
     case mo_div: es->variables->Set(t, A->string_value->decimal()/B->decimal_value); return;
     case mo_mod: es->variables->Set(t, fmod(A->string_value->decimal(),B->decimal_value)); return;
     case mo_less: es->variables->Set(t, A->string_value->decimal()<B->decimal_value?1:0); return;
     case mo_more: es->variables->Set(t, A->string_value->decimal()>B->decimal_value?1:0); return;
     case mo_lessEq: es->variables->Set(t, A->string_value->decimal()<=B->decimal_value?1:0); return;
     case mo_moreEq: es->variables->Set(t, A->string_value->decimal()>=B->decimal_value?1:0); return;
     case mo_equal: es->variables->Set(t, A->string_value->decimal()==B->decimal_value?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else if ( is_integer(A->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->string_value->decimal()+B->decimal_value); return;
     case mo_mul: es->variables->Set(t, A->string_value->decimal()*B->decimal_value); return;
     case mo_sub: es->variables->Set(t, A->string_value->decimal()-B->decimal_value); return;
     case mo_div: es->variables->Set(t, A->string_value->decimal()/B->decimal_value); return;
     case mo_mod: es->variables->Set(t, fmod(A->string_value->decimal(),B->decimal_value)); return;
     case mo_less: es->variables->Set(t, A->string_value->decimal()<B->decimal_value?1:0); return;
     case mo_more: es->variables->Set(t, A->string_value->decimal()>B->decimal_value?1:0); return;
     case mo_lessEq: es->variables->Set(t, A->string_value->decimal()<=B->decimal_value?1:0); return;
     case mo_moreEq: es->variables->Set(t, A->string_value->decimal()>=B->decimal_value?1:0); return;
     case mo_equal: es->variables->Set(t, A->string_value->decimal()==B->decimal_value?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else {
    switch ( op ) {
     case mo_add: es->variables->Set(t, (A->string_value->value+FORMAT("%1.2f",B->decimal_value)).c_str()); return;
     case mo_mul: // unsupported, no op
     case mo_sub: // unsupported, no op
     case mo_div: // unsupported, no op
     case mo_mod: // unsupported, no op
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_decimal   && type_b == v_string    ) { // dec str
   if ( is_decimal(B->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->decimal_value+B->string_value->decimal()); return;
     case mo_mul: es->variables->Set(t, A->decimal_value*B->string_value->decimal()); return;
     case mo_sub: es->variables->Set(t, A->decimal_value-B->string_value->decimal()); return;
     case mo_div: es->variables->Set(t, A->decimal_value/B->string_value->decimal()); return;
     case mo_mod: es->variables->Set(t, fmod(A->decimal_value,B->string_value->decimal())); return;
     case mo_less: es->variables->Set(t, A->decimal_value<B->string_value->decimal()?1:0); return;
     case mo_more: es->variables->Set(t, A->decimal_value>B->string_value->decimal()?1:0); return;
     case mo_lessEq: es->variables->Set(t, A->decimal_value<=B->string_value->decimal()?1:0); return;
     case mo_moreEq: es->variables->Set(t, A->decimal_value>=B->string_value->decimal()?1:0); return;
     case mo_equal: es->variables->Set(t, A->decimal_value==B->string_value->decimal()?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else if ( is_integer(B->string_value->c_str()) ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->decimal_value+B->string_value->decimal()); return;
     case mo_mul: es->variables->Set(t, A->decimal_value*B->string_value->decimal()); return;
     case mo_sub: es->variables->Set(t, A->decimal_value-B->string_value->decimal()); return;
     case mo_div: es->variables->Set(t, A->decimal_value/B->string_value->decimal()); return;
     case mo_mod: es->variables->Set(t, fmod(A->decimal_value,B->string_value->decimal())); return;
     case mo_less: es->variables->Set(t, A->decimal_value<B->string_value->decimal()?1:0); return;
     case mo_more: es->variables->Set(t, A->decimal_value>B->string_value->decimal()?1:0); return;
     case mo_lessEq: es->variables->Set(t, A->decimal_value<=B->string_value->decimal()?1:0); return;
     case mo_moreEq: es->variables->Set(t, A->decimal_value>=B->string_value->decimal()?1:0); return;
     case mo_equal: es->variables->Set(t, A->decimal_value==B->string_value->decimal()?1:0); return;
     default: /* no recognizable op */  return;
    }
   } else {
    switch ( op ) {
     case mo_add: es->variables->Set(t, (FORMAT("%1.2f",A->decimal_value.value)+B->string_value->value).c_str()); return;
     case mo_mul: // unsupported, no op
     case mo_sub: // unsupported, no op
     case mo_div: // unsupported, no op
     case mo_mod: // unsupported, no op
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_cartesian && type_b == v_string    ) { // car str
   Cartesian cb; cb.fromString(B->string_value->c_str());
   switch ( op ) {
    case mo_add: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveBy(cb.x,cb.y); es->variables->Set(t, d); } return;
    case mo_mul: { Cartesiand e; e.Set((double)cb.fx,(double)cb.fy); Cartesian *d=new Cartesian; d->Set(A->cartesian); d->Multiply(&e); es->variables->Set(t, d); } return;
    case mo_sub: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveBy(-cb.x,-cb.y); es->variables->Set(t, d); } return;
    case mo_div: { Cartesian *d=new Cartesian; d->Set(A->cartesian); if ( cb.fx != 0 ) d->x=(int)((float)d->x/cb.fx); if ( cb.fy != 0 ) d->y=(int)((float)d->y/cb.fy); d->SetRect(d->x,d->y,d->w,d->h); es->variables->Set(t, d); } return;
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_string    && type_b == v_cartesian ) { // str car
   switch ( op ) {
    case mo_add: es->variables->Set(t, (A->string_value->value+B->cartesian->toString()).c_str()); return;
    case mo_mul: // unsupported, no op
    case mo_sub: // unsupported, no op
    case mo_div: // unsupported, no op
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_cartesian && type_b == v_integer   ) { // car int
   switch ( op ) {
    case mo_add: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveBy(B->integer_value,B->integer_value); es->variables->Set(t, d); } return;
    case mo_mul: { Cartesiand e; e.Set((double)B->integer_value,(double)B->integer_value); Cartesian *d=new Cartesian; d->Set(A->cartesian); d->Multiply(&e); es->variables->Set(t, d); } return;
    case mo_sub: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveBy(-B->integer_value,-B->integer_value); es->variables->Set(t, d); } return;
    case mo_div: { Cartesian *d=new Cartesian; d->Set(A->cartesian); if ( B->integer_value != 0 ) { d->x=(int)((float)d->x/(float)B->integer_value); d->y=(int)((float)d->y/(float)B->integer_value); } d->SetRect(d->x,d->y,d->w,d->h); es->variables->Set(t, d); } return;
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_integer   && type_b == v_cartesian ) { // int car 
   switch ( op ) {
    case mo_add: { Cartesian *d=new Cartesian; d->Set(B->cartesian); d->MoveBy(A->integer_value,A->integer_value); es->variables->Set(t, d); } return;
    case mo_mul: { Cartesiand e; e.Set((double)A->integer_value,(double)A->integer_value); Cartesian *d=new Cartesian; d->Set(B->cartesian); d->Multiply(&e); es->variables->Set(t, d); } return;
    case mo_sub: { Cartesian *d=new Cartesian; d->Set(B->cartesian); d->SetRect(-d->x,-d->y,d->w,d->h); d->MoveBy(A->integer_value,A->integer_value); es->variables->Set(t, d); } return;
    case mo_div: { Cartesian *d=new Cartesian; d->Set(B->cartesian); if ( A->integer_value != 0 ) { d->x=(int)iratiof(A->integer_value,d->x); d->y=(int)iratiof(A->integer_value,d->y); } d->SetRect(d->x,d->y,d->w,d->h); es->variables->Set(t, d); } return;
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_cartesian && type_b == v_decimal   ) { // car dec
   switch ( op ) {
    case mo_add: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveByd(B->decimal_value,B->decimal_value); es->variables->Set(t, d); } return;
    case mo_mul: { Cartesiand e; e.Set(B->decimal_value,B->decimal_value); Cartesian *d=new Cartesian; d->Set(A->cartesian); d->Multiply(&e); es->variables->Set(t, d); } return;
    case mo_sub: { Cartesian *d=new Cartesian; d->Set(A->cartesian); d->MoveByd(-B->decimal_value,-B->decimal_value); es->variables->Set(t, d); } return;
    case mo_div: { Cartesian *d=new Cartesian; d->Set(A->cartesian); if ( B->decimal_value != 0 ) { d->x=(int)((float)d->x/B->decimal_value); d->y=(int)((float)d->y/B->decimal_value); } d->SetRect(d->x,d->y,d->w,d->h); es->variables->Set(t, d); } return;
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_decimal   && type_b == v_cartesian ) { // dec car 
   switch ( op ) {
    case mo_add: { Cartesian *d=new Cartesian; d->Set(B->cartesian); d->MoveByd(A->decimal_value,A->decimal_value); es->variables->Set(t, d); } return;
    case mo_mul: { Cartesiand e; e.Set(A->decimal_value,A->decimal_value); Cartesian *d=new Cartesian; d->Set(B->cartesian); d->Multiply(&e); es->variables->Set(t, d); } return;
    case mo_sub: { Cartesian *d=new Cartesian; d->Set(B->cartesian); d->SetRect(-d->x,-d->y,d->w,d->h); d->MoveByd(A->decimal_value,A->decimal_value); es->variables->Set(t, d); } return;
    case mo_div: { Cartesian *d=new Cartesian; d->Set(B->cartesian); if ( A->decimal_value != 0 ) { d->x=(int)(A->decimal_value/(double)d->x); d->y=(int)(A->decimal_value/(double)d->y); } d->SetRect(d->x,d->y,d->w,d->h); es->variables->Set(t, d); } return;
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_crayon    && type_b == v_string    ) { // cra str
   Crayon c;
   if ( is_decimal(B->string_value->c_str()) ) {
    double d=B->string_value->decimal();
    c.fromCrayon(*A->crayon);
    switch ( op ) {
     case mo_add: c.Double((double)c.rf+d,(double)c.gf+d,(double)c.bf+d,(double)c.af); es->variables->Set(t, c); return;
     case mo_mul: c.Double((double)c.rf*d,(double)c.gf*d,(double)c.bf*d,(double)c.af); es->variables->Set(t, c); return;
     case mo_sub: c.Double((double)c.rf-d,(double)c.gf-d,(double)c.bf-d,(double)c.af); es->variables->Set(t, c); return;
     case mo_div: c.Double((double)c.rf/d,(double)c.gf/d,(double)c.bf/d,(double)c.af); es->variables->Set(t, c); return;
     case mo_mod: c.Double(fmod((double)c.rf,d),fmod((double)c.gf,d),fmod((double)c.bf,d),(double)c.af); es->variables->Set(t, c); return;
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   } else if ( is_integer(B->string_value->c_str()) ) {
    int d=B->string_value->integer();
    c.fromCrayon(*A->crayon);
    switch ( op ) {
     case mo_add: c.Int(c.r+d,c.g+d,c.b+d,c.a); es->variables->Set(t, c); return;
     case mo_mul: c.Int(c.r*d,c.g*d,c.b*d,c.a); es->variables->Set(t, c); return;
     case mo_sub: c.Int(c.r-d,c.g-d,c.b-d,c.a); es->variables->Set(t, c); return;
     case mo_div: c.Int(c.r/d,c.g/d,c.b/d,c.a); es->variables->Set(t, c); return;
     case mo_mod: c.Int(c.r%d,c.g%d,c.b%d,c.a); es->variables->Set(t, c); return;
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   } else {
    c.fromString(B->string_value->c_str());
    switch ( op ) {
     case mo_add: c.AddColorf(*A->crayon,c); es->variables->Set(t, c); return;
     case mo_mul: c.Multiply(*A->crayon,c);  es->variables->Set(t, c); return;
     case mo_sub: c.SubColorf(*A->crayon,c); es->variables->Set(t, c); return;
     case mo_div: c.MultiplyClampRGB(A->crayon->rf,A->crayon->gf,A->crayon->bf); es->variables->Set(t, c); return;
     case mo_mod: c.Multiply(*A->crayon,c,true); es->variables->Set(t, c); return;
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_string    && type_b == v_crayon    ) { // str cra
   switch ( op ) {
    case mo_add: es->variables->Set(t, (A->string_value->value+B->crayon->toString()).c_str()); return;
    case mo_mul: // unsupported, no op
    case mo_sub: // unsupported, no op
    case mo_div: // unsupported, no op
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_crayon    && type_b == v_integer   ) { // cra int
   int d=B->integer_value;
   Crayon c;
   c.fromCrayon(*A->crayon);
   switch ( op ) {
    case mo_add: c.Int(c.r+d,c.g+d,c.b+d,c.a); es->variables->Set(t, c); return;
    case mo_mul: c.Int(c.r*d,c.g*d,c.b*d,c.a); es->variables->Set(t, c); return;
    case mo_sub: c.Int(c.r-d,c.g-d,c.b-d,c.a); es->variables->Set(t, c); return;
    case mo_div: c.Int(c.r/d,c.g/d,c.b/d,c.a); es->variables->Set(t, c); return;
    case mo_mod: c.Int(c.r%d,c.g%d,c.b%d,c.a); es->variables->Set(t, c); return;
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_integer   && type_b == v_crayon    ) { // int cra 
   int d=A->integer_value;
   Crayon c;
   c.fromCrayon(*B->crayon);
   switch ( op ) {
    case mo_add: c.Int(d+c.r,d+c.g,d+c.b,c.a); es->variables->Set(t, c); return;
    case mo_mul: c.Int(d*c.r,d*c.g,d*c.b,c.a); es->variables->Set(t, c); return;
    case mo_sub: c.Int(d-c.r,d-c.g,d-c.b,c.a); es->variables->Set(t, c); return;
    case mo_mod: c.Int(d%c.r,d%c.g,d%c.b,c.a); es->variables->Set(t, c); return;
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_crayon    && type_b == v_decimal   ) { // cra dec
   double d=B->string_value->decimal();
   Crayon c;
   c.fromCrayon(*A->crayon);
   switch ( op ) {
    case mo_add: c.Double((double)c.rf+d,(double)c.gf+d,(double)c.bf+d,(double)c.af); es->variables->Set(t, c); return;
    case mo_mul: c.Double((double)c.rf*d,(double)c.gf*d,(double)c.bf*d,(double)c.af); es->variables->Set(t, c); return;
    case mo_sub: c.Double((double)c.rf-d,(double)c.gf-d,(double)c.bf-d,(double)c.af); es->variables->Set(t, c); return;
    case mo_div: c.Double((double)c.rf/d,(double)c.gf/d,(double)c.bf/d,(double)c.af); es->variables->Set(t, c); return;
    case mo_mod: c.Double(fmod((double)c.rf,d),fmod((double)c.gf,d),fmod((double)c.bf,d),(double)c.af); es->variables->Set(t, c); return;
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_decimal   && type_b == v_crayon    ) { // dec cra 
   double d=A->string_value->decimal();
   Crayon c;
   c.fromCrayon(*A->crayon);
   switch ( op ) {
    case mo_add: c.Double(d+(double)c.rf,d+(double)c.gf,d+(double)c.bf,(double)c.af); es->variables->Set(t, c); return;
    case mo_mul: c.Double(d*(double)c.rf,d*(double)c.gf,d*(double)c.bf,(double)c.af); es->variables->Set(t, c); return;
    case mo_sub: c.Double(d-(double)c.rf,d-(double)c.gf,d-(double)c.bf,(double)c.af); es->variables->Set(t, c); return;
    case mo_div: c.Double(d/(double)c.rf,d/(double)c.gf,d/(double)c.bf,(double)c.af); es->variables->Set(t, c); return;
    case mo_mod: c.Double(fmod(d,(double)c.rf),fmod(d,(double)c.gf),fmod(d,(double)c.bf),(double)c.af); es->variables->Set(t, c); return;
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_string    && type_b == v_image     ) { // str img 
   switch ( op ) {
    case mo_add: es->variables->Set(t, (A->string_value->value+B->image->filename).c_str()); return;
    case mo_mul: // unsupported, no op
    case mo_sub: // unsupported, no op
    case mo_div: // unsupported, no op
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_string    && type_b == v_blend     ) { // str ble
   switch ( op ) {
    case mo_add: es->variables->Set(t, (A->string_value->value+B->blend.toString()).c_str()); return;
    case mo_mul: // unsupported, no op
    case mo_sub: // unsupported, no op
    case mo_div: // unsupported, no op
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_string    && type_b == v_tween     ) { // str twe
   switch ( op ) {
    case mo_add: es->variables->Set(t, (A->string_value->value+B->tween->name).c_str()); return;
    case mo_mul: // unsupported, no op
    case mo_sub: // unsupported, no op
    case mo_div: // unsupported, no op
    case mo_mod: // unsupported, no op
    case mo_less: // unsupported, no op
    case mo_more: // unsupported, no op
    case mo_lessEq: // unsupported, no op
    case mo_moreEq: // unsupported, no op
    case mo_equal: // unsupported, no op
    default: /* no recognizable op */  return;
   }
  } else if ( type_a == v_tween     && type_b == v_integer   ) { // twe int
   if ( A->tween ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->tween->tween[(int)B->integer_value%(int)A->tween->tween.length]); return;
     case mo_mul: es->variables->Set(t, A->tween->tween[URANGE(0,(int)B->integer_value,(int)A->tween->tween.length-1)]); return;
     case mo_sub: es->variables->Set(t,-A->tween->tween[(int)B->integer_value%(int)A->tween->tween.length]); return;
     case mo_div: es->variables->Set(t, A->tween->tween[A->tween->tween.length-1-URANGE(0,(int)B->integer_value,(int)A->tween->tween.length-1)]); return;
     case mo_mod: es->variables->Set(t,-A->tween->tween[A->tween->tween.length-1-URANGE(0,(int)B->integer_value,(int)A->tween->tween.length-1)]); return;
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_tween     && type_b == v_decimal   ) { // twe dec
   if ( A->tween ) {
    float d=(float) B->decimal_value;
    switch ( op ) {
     case mo_add: es->variables->Set(t, A->tween->tweenf(d)); return;
     case mo_mul: es->variables->Set(t, A->tween->tweenf(1.0f-d)); return;
     case mo_sub: es->variables->Set(t, -A->tween->tweenf(d)); return;
     case mo_div: es->variables->Set(t, A->tween->tweenf(-d)); return;
     case mo_mod: es->variables->Set(t, -A->tween->tweenf(-d)); return;
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_integer   && type_b == v_tween     ) { // int twe
   if ( B->tween ) {
    switch ( op ) {
     case mo_add: es->variables->Set(t, B->tween->tween[(int)A->integer_value%(int)B->tween->tween.length]); return;
     case mo_mul: es->variables->Set(t, B->tween->tween[URANGE(0,(int)A->integer_value,(int)B->tween->tween.length-1)]); return;
     case mo_sub: es->variables->Set(t,-B->tween->tween[(int)A->integer_value%(int)B->tween->tween.length]); return;
     case mo_div: es->variables->Set(t, B->tween->tween[B->tween->tween.length-1-URANGE(0,(int)A->integer_value,(int)B->tween->tween.length-1)]); return;
     case mo_mod: es->variables->Set(t,-B->tween->tween[B->tween->tween.length-1-URANGE(0,(int)A->integer_value,(int)B->tween->tween.length-1)]); return;
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else if ( type_a == v_decimal   && type_b == v_tween     ) { // dec twe
   if ( B->tween ) {
    float d=(float) A->decimal_value;
    switch ( op ) {
     case mo_add: es->variables->Set(t, B->tween->tweenf(d)); return;
     case mo_mul: es->variables->Set(t, B->tween->tweenf(1.0f-d)); return;
     case mo_sub: es->variables->Set(t, -B->tween->tweenf(d)); return;
     case mo_div: es->variables->Set(t, B->tween->tweenf(-d)); return;
     case mo_mod: es->variables->Set(t, -B->tween->tweenf(-d)); return;
     case mo_less: // unsupported, no op
     case mo_more: // unsupported, no op
     case mo_lessEq: // unsupported, no op
     case mo_moreEq: // unsupported, no op
     case mo_equal: // unsupported, no op
     default: /* no recognizable op */  return;
    }
   }
  } else { 
   /* other combinations exist, but are not supported, add more here as more types are added */
   return;
  }
 }
}

void BoundArea::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 while ( w != '\0' ) {
  if ( !str_cmp(w,"color") || !str_cmp(w,"crayon") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&color);
  } else if ( !str_cmp(w,"x") ) {
   w=words.Next();
   place.x=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y") ) {
   w=words.Next();
   place.y=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"x2") ) {
   w=words.Next();
   place.x2=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y2") ) {
   w=words.Next();
   place.y2=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"extents") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&place);
  }
  w=words.Next();
 }
}

void BoundRectangle::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 while ( w != '\0' ) {
  if ( !str_cmp(w,"color") || !str_cmp(w,"crayon") || !str_cmp(w,"tint") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&color);
  } else if ( !str_cmp(w,"x") ) {
   w=words.Next();
   place.x=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y") ) {
   w=words.Next();
   place.y=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"x2") ) {
   w=words.Next();
   place.x2=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y2") ) {
   w=words.Next();
   place.y2=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"extents") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&place);
  }
  w=words.Next();
 }
}

void BoundStretch::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 while ( w != '\0' ) {
  if ( !str_cmp(w,"color") || !str_cmp(w,"crayon") || !str_cmp(w,"tint") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint);
  } else if ( !str_cmp(w,"x") ) {
   w=words.Next();
   place.x=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y") ) {
   w=words.Next();
   place.y=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"x2") ) {
   w=words.Next();
   place.x2=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"y2") ) {
   w=words.Next();
   place.y2=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"extents") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&place);
  } else if ( !str_cmp(w,"alpha") ) {
   w=words.Next();
   alpha=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
  }
 }
}

#include "FastGUIProcessors.h"
void BoundWindow::fromString( GLWindowScriptable *win, const char *in ) {
 win->fromString(in);
}

void BoundWindow::fromString( Consulate *es, const char *in ) {
 GLWindowScriptable *win=new GLWindowScriptable(es);
 fromString(win,in);
 windows.Add(win);
}

void BoundPresentation::fromString( PresentationScriptable *p, const char *in ) {
 p->fromString(in);
}

void BoundPresentation::fromString( Consulate *es, const char *in ) {
 PresentationScriptable *p=new PresentationScriptable(es);
 fromString(p,in);
 windows.Add(p);
}

void BoundDiscard::fromString( Consulate *es, const char *in ) {
 name=es->variables->EvaluateString(in);
}

void BoundDiscard::Execute() {
 const char *w=name.c_str();
 if ( is_integer(w) ) {
  int id=atoi(w);
  EACH(windows.first,GLWindow,g) {
   if ( id == g->code ) g->destroyMe=true;
  }
 } else {
  EACH(windows.first,GLWindow,g) {
   if ( !str_cmp(w,g->name.c_str()) ) g->destroyMe=true;
  }
 }
}

// text.functions

void BoundStringLoad::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 target_variable=es->variables->EvaluateString(w);
 w=words.Next();
 filename=es->variables->EvaluateString(w);
 es->variables->Set(target_variable.c_str(),file_as_string(filename.c_str()).c_str());
}

void BoundStringSave::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 output=es->variables->EvaluateString(w);
 w=words.Next();
 filename=es->variables->EvaluateString(w);
 output.Save(filename.c_str());
}

void BoundStringLine::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 number.value=es->variables->EvaluateInteger(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Strings lines;
 lines.SplitPush(source.c_str(),'\n');
 String *line=(String *)lines.Element(number);
 Zstring result;
 if ( line ) result=line->s;
 es->variables->Set(target.c_str(),result.c_str());
}

void BoundStringLines::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Strings lines;
 lines.SplitPush(source.c_str(),'\n');
 es->variables->Set(target.c_str(),lines.count.value);
}

void BoundStringConcat::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 a=es->variables->EvaluateString(w);
 w=words.Next();
 b=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(),(a.value+b.value).c_str());
}

void BoundStringConcatLine::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 a=es->variables->EvaluateString(w);
 w=words.Next();
 b=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(),(a.value+string("\n")+b.value).c_str());
}

void BoundStringReplace::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source1=es->variables->EvaluateString(w);
 w=words.Next();
 search=es->variables->EvaluateString(w);
 w=words.Next();
 replace=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Zstring result;
 result=source1;
 result(search.c_str(),replace.c_str());
 es->variables->Set(target.c_str(),result.c_str());
}

void BoundStringReplace1::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source1=es->variables->EvaluateString(w);
 w=words.Next();
 search=es->variables->EvaluateString(w);
 w=words.Next();
 replace=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Zstring result=source1;
 replaceOnce(result.value,search.value,replace.value);
 es->variables->Set(target.c_str(),result.c_str());
}

void BoundStringChar::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 number=es->variables->EvaluateInteger(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Zstring result;
 result+=target.At(number.value);
 es->variables->Set(target.c_str(),result.c_str());
}

void BoundStringInteger::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(),atoi(source.c_str()));
}

void BoundStringDecimal::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(),atof(source.c_str()));
}

void BoundStringWord::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 number=es->variables->EvaluateInteger(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Zstring result;
 result=source.word(number.value);
 es->variables->Set(target.c_str(),result.c_str());
}

void BoundStringWords::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 int count=0;
 const char *p=source.Next();
 while ( *p != '\0' ) {
  count++;
  p=source.Next();
 }
 source.rewind();
 es->variables->Set(target.c_str(),count);
}

void BoundStringMatches::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 source=es->variables->EvaluateString(w);
 w=words.Next();
 query=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(),
  !str_cmp(source.c_str(),query.c_str()) ? 1 : 0
 );
}

// math.functions

void BoundMathAdd::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundMathSub::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundMathDiv::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundMathMul::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundMathMod::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundMathRad::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), deg2rad(a) );
}

void BoundMathDeg::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), rad2deg(a) );
}

void BoundMathLeast::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double b=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), UMIN(a,b) );
}

void BoundMathGreatest::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double b=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), UMAX(a,b) );
}

void BoundMathDice::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 int a=es->variables->EvaluateInteger(w);
 w=words.Next();
 float b=(float)es->variables->EvaluateInteger(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 int rolled=0;
 while ( a > 0 ) {
  rolled+=(int)(((float)random(1.0)+randoms.Unique())*0.5f*b);
  a--;
 }
 es->variables->Set(target.c_str(), rolled);
}

void BoundMathRandom::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double b=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), (b-a)*randoms.Unique()+a );
}

void BoundMathPseudo::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double b=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), (b-a)*pseudo((int)((a+b)*60000.0))+a );
}

void BoundMathLerp::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double b=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double c=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), (c*(b-a)+a) );
}

void BoundMathRange::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double b=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double c=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), URANGE(a,b,c) );
}
// bool.functions

void BoundBoolEqual::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundBoolLess::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundBoolMore::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundBoolLessEqual::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundBoolMoreEqual::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zstring a=w; w=words.Next();
 Zstring b=w; w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 EvalMath( operation, es, a.c_str(), b.c_str(), target.c_str() );
}

void BoundBoolOdd::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 int a=es->variables->EvaluateInteger(w);
 w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(),
  a%2==1 ? 1 : 0
 );
}

void BoundBoolEven::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 int a=es->variables->EvaluateInteger(w);
 w=words.Next();
 Zstring target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(),
  a%2==0 ? 1 : 0
 );
}

// type.functions

void BoundTypeIs::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 if ( is_integer(w) ) {
   w=words.Next();
   VariadicType vt=HintToType(w);
   w=words.Next();
   target=es->variables->EvaluateString(w);
   es->variables->Set(target.c_str(),v_integer==vt?1:0);   
 } else if ( is_decimal(w) ) {
   w=words.Next();
   VariadicType vt=HintToType(w);
   w=words.Next();
   target=es->variables->EvaluateString(w);
   es->variables->Set(target.c_str(),v_decimal==vt?1:0);   
 } else {
  if ( *w == '^' ) w++;
  Variadic *v=es->variables->Get(w);
  if ( v ) {
   w=words.Next();
   VariadicType vt=HintToType(w);
   w=words.Next();
   target=es->variables->EvaluateString(w);
   es->variables->Set(target.c_str(),v->type==vt?1:0);   
  } else {
   w=words.Next();
   target=es->variables->EvaluateString(w);
   es->variables->Set(target.c_str(),0);
  }
 }
}

void BoundTypeInteger::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 int a=es->variables->EvaluateInteger(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a);
}

void BoundTypeDecimal::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a);
}

void BoundTypeString::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string a=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a.c_str());
}

void BoundTypeBlend::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Blends a=es->variables->EvaluateBlend(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a);
}

void BoundTypeImage::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zp<GLImage> image; es->variables->EvaluateImage(w,&image.pointer);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), image);
}

void BoundTypeCrayon::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon color; es->variables->EvaluateCrayon(w,&color);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), color);
}

void BoundTypeCartesian::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian *xy=new Cartesian; es->variables->EvaluateCartesian(w,xy);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), xy);
}

void BoundTypeTween::fromString( Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zp<Tween> tween; es->variables->EvaluateTween(w,&tween.pointer);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), tween);
}

// tween.functions

void BoundTweenInstant::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zp<Tween> tween; es->variables->EvaluateTween(w,&tween.pointer);
 w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 if ( tween ) {
  es->variables->Set(target.c_str(), tween->tweend(a) );
 }
}

void BoundCartesianX::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian xy; es->variables->EvaluateCartesian(w,&xy);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), xy.x );
}

void BoundCartesianY::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian xy; es->variables->EvaluateCartesian(w,&xy);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), xy.y );
}

void BoundCartesianW::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian xy; es->variables->EvaluateCartesian(w,&xy);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), xy.w );
}

void BoundCartesianH::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian xy; es->variables->EvaluateCartesian(w,&xy);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), xy.h );
}

void BoundCartesianX2::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian xy; es->variables->EvaluateCartesian(w,&xy);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), xy.x );
}

void BoundCartesianY2::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian xy; es->variables->EvaluateCartesian(w,&xy);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), xy.y );
}

// crayon.functions

void BoundCrayonR::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.rf );
}

void BoundCrayonG::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.gf );
}

void BoundCrayonB::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.bf );
}

void BoundCrayonA::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.af );
}

void BoundCrayonR255::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.r );
}

void BoundCrayonG255::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.g );
}

void BoundCrayonB255::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.b );
}

void BoundCrayonA255::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Crayon c; es->variables->EvaluateCrayon(w,&c);
 w=words.Next();
 target=es->variables->EvaluateString(w); 
 es->variables->Set(target.c_str(), c.a );
}

// image.functions

void BoundImageName::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Zp<GLImage> image; es->variables->EvaluateImage(w,&image.pointer);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), image ? image->filename.c_str() : "null" );
}

// geo.functions

void BoundGeoDist1d::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 double a=es->variables->EvaluateDecimal(w);
 w=words.Next();
 double b=es->variables->EvaluateDecimal(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), abs(b-a) );
}

void BoundGeoDist2d::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a.Distance(b.x,b.y) );
}

void BoundGeoDist3d::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), idistance(a.x,a.y,a.z,b.x,b.y,b.z) );
}

void BoundGeoBoxBox::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a.Intersects(&b) ? 1 : 0 );
}
void BoundGeoBoxBoxIntersection::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Cartesian *xy=new Cartesian;
 if ( a.Intersects(&b) ) {
  xy->SetRect( max(a.x, b.x), max(a.y, b.y), min(a.x2,b.x2), min(a.y2,b.y2) );
 }
 es->variables->Set(target.c_str(), xy );
}

void BoundGeoBoxLine::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 Cartesian xy;
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a.BoxLine(&b) ? 1 : 0 );
}

void BoundGeoLineLine::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 Cartesian xy;
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), a.LineLine(&b) ? 1 : 0 );
}

void BoundGeoLineLineIntersection::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 Cartesian *xy = new Cartesian;
 a.LineLine(&b,xy);
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), xy );
}

void BoundGeoPointCircle::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), idistance(a.x,a.y,a.z,b.x,b.y,b.z) < b.w ?1:0 );
}

void BoundGeoLineCircle::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 Line line; line.Set(a.x,a.y,a.x2,a.y2);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 Circle c((double)b.x,(double)b.y,(double)b.w);
 CoplanarLinesResults out;
 Line result;
 bool collided=line.Collides(&c,&result,c.precision);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 es->variables->Set(target.c_str(), collided?1:0 );
}

void BoundGeoLineCircleIntersection::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 Line line; line.Set(a.x,a.y,a.x2,a.y2);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 Circle c((double)b.x,(double)b.y,(double)b.w);
 CoplanarLinesResults out;
 Line result; bool collided=line.Collides(&c,&result,c.precision);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Cartesian *xy=new Cartesian; xy->Corners((int)line.x,(int)line.y,(int)line.x2,(int)line.y2);
 es->variables->Set(target.c_str(), xy );
}

void BoundGeoRotate::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 w=words.Next();
 Cartesian b; es->variables->EvaluateCartesian(w,&b);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Vertex v((float)a.x,(float)a.y,(float)a.z);
 v.Rotate((float)b.x,(float)b.y,(float)b.z);
 Cartesian *xy=new Cartesian;
 xy->Setf(v.x,v.y);
 xy->fz=v.z;
 xy->x=(int)v.x;
 xy->y=(int)v.y;
 xy->z=(int)v.z;
 es->variables->Set(target.c_str(), xy );
}

// fbo.functions

void BoundFBOActivate::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 FBO *from=FBOfromString(w);
 if ( !from ) return;
 w=words.Next();
 if ( !str_cmp(w,"keep") ) from->ActivateWithoutClearing();
 else from->Activate();
}

void BoundFBODeactivate::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 FBO *from=FBOfromString(w);
 if ( !from ) return;
 from->Deactivate();
}

void BoundFBOStretch::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 FBO *from=FBOfromString(w);
 if ( !from ) return;
 w=words.Next();
 Crayon a; es->variables->EvaluateCrayon(w,&a);
 Blends b; b=es->variables->EvaluateBlend(w);
 Cartesian c; es->variables->EvaluateCartesian(w,&c);
 w=words.Next();
 from->Render(a,b,c.x,c.y,c.x2,c.y2);
}

void BoundFBORead::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 FBO *from=FBOfromString(w);
 if ( !from ) return;
 w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 from->ReadPixel(gl_color0,a.x,a.y);
 w=words.Next();
 target=w;
 Crayon c; c.Int(from->ub_bgra[0],from->ub_bgra[1],from->ub_bgra[2],from->ub_bgra[3]);
 es->variables->Set(target.c_str(),c);
}

void BoundFBOReadStencil::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 FBO *from=FBOColorDepthStencilfromString(w);
 if ( !from ) return;
 w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 from->ReadPixel(gl_stencil,a.x,a.y);
 w=words.Next();
 target=w;
 Crayon c; c.Int(from->ub_bgra[0],from->ub_bgra[1],from->ub_bgra[2],from->ub_bgra[3]);
 es->variables->Set(target.c_str(),c);
}

void BoundFBOReadDepth::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 FBO *from=FBOColorDepthfromString(w);
 if ( !from ) from=FBOColorDepthStencilfromString(w);
 if ( !from ) return;
 w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 from->ReadPixel(gl_depth,a.x,a.y);
 w=words.Next();
 target=w;
 Crayon c; c.Int(from->ub_bgra[0],from->ub_bgra[1],from->ub_bgra[2],from->ub_bgra[3]);
 es->variables->Set(target.c_str(),c);
}

// vbo functions

// ui functions

void BoundUIVirtual::fromString(Consulate *es, const char *in) {
 Zstring words(in);
 const char *w=words.Next();
 Cartesian a; es->variables->EvaluateCartesian(w,&a);
 display.Virtual(a.w,a.h);
 es->variables->Set("virtual.display.w",a.w);
 es->variables->Set("virtual.display.h",a.h);
}

// audio.functions

void BoundAudioPlay::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
}

void BoundAudioStop::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
}

void BoundAudioCue::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string sound=es->variables->EvaluateString(w);
 w=words.Next();
 float gain=(float)es->variables->EvaluateDecimal(w);
 w=words.Next();
 float pitch=(float)es->variables->EvaluateDecimal(w);
 audio.manager.CueAndPlay(sound.c_str(),pitch,gain,false,true);
}

void BoundAudioCueLoop::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string sound=es->variables->EvaluateString(w);
 w=words.Next();
 float gain=(float)es->variables->EvaluateDecimal(w);
 w=words.Next();
 float pitch=(float)es->variables->EvaluateDecimal(w);
 audio.manager.CueAndPlay(sound.c_str(),pitch,gain,true,true);
}

void BoundAudioCueSingleton::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string sound=es->variables->EvaluateString(w);
 w=words.Next();
 float gain=(float)es->variables->EvaluateDecimal(w);
 w=words.Next();
 float pitch=(float)es->variables->EvaluateDecimal(w);
 audio.manager.CueAndPlayIfNotPlaying(sound.c_str(),pitch,gain,false,true);
}

void BoundAudioCueLimited::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string sound=es->variables->EvaluateString(w);
 w=words.Next();
 int limit=es->variables->EvaluateInteger(w);
 w=words.Next();
 float gain=(float)es->variables->EvaluateDecimal(w);
 w=words.Next();
 float pitch=(float)es->variables->EvaluateDecimal(w);
 audio.manager.CueAndPlayLimited(sound.c_str(),limit,pitch,gain,false,true);
}

void BoundAudioCueLimitedLoop::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string sound=es->variables->EvaluateString(w);
 w=words.Next();
 int limit=es->variables->EvaluateInteger(w);
 w=words.Next();
 float gain=(float)es->variables->EvaluateDecimal(w);
 w=words.Next();
 float pitch=(float)es->variables->EvaluateDecimal(w);
 audio.manager.CueAndPlayLimited(sound.c_str(),limit,pitch,gain,true,true);
}

void BoundAudioVolumeMaster::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 float volume=(float) es->variables->EvaluateDecimal(w);
 audio.SetVolume(volume);
}

void BoundAudioVolumeMusic::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 float volume=(float) es->variables->EvaluateDecimal(w);
 audio.SetMusicVolume(volume);
}

void BoundAudioVolumeSounds::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 float volume=(float) es->variables->EvaluateDecimal(w);
 audio.SetSoundVolume(volume);
}


// system.functions

#include "WebPage.h"
void BoundSystemURL::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string url=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 string response=GetURL(url.c_str(),"",false);
 es->variables->Set(target.c_str(),response.c_str());
}

void BoundSystemURLSecured::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string url=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 string response=GetURL(url.c_str(),"",false);
 es->variables->Set(target.c_str(),response.c_str());
}

void BoundSystemTCPConnect::fromString(Consulate *es, const char *in ) {
}

void BoundSystemTCPSend::fromString(Consulate *es, const char *in ) {
}

void BoundSystemTCPListen::fromString(Consulate *es, const char *in ) {
}

void BoundSystemTCPDisconnect::fromString(Consulate *es, const char *in ) {
}

void BoundSystemMouse::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string url=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 Cartesian mouse(input.mxi,input.myi);
 es->variables->Set(target,&mouse);
 es->variables->Set((target+string(".x")).c_str(),(int)(input.mxi));
 es->variables->Set((target+string(".y")).c_str(),(int)(input.mxi));
 es->variables->Set((target+string(".left")).c_str(),(int)(input.left));
 es->variables->Set((target+string(".right")).c_str(),(int)(input.right));
 es->variables->Set((target+string(".middle")).c_str(),(int)(input.middle));
 es->variables->Set((target+string(".wheel.up")).c_str(),(int)(input.wheelUp));
 es->variables->Set((target+string(".wheel.down")).c_str(),(int)(input.wheelDown));
 es->variables->Set((target+string(".left.released")).c_str(),(input.leftReleased()?1:0));
 es->variables->Set((target+string(".right.released")).c_str(),(input.rightReleased()?1:0));
 es->variables->Set((target+string(".middle.released")).c_str(),(input.middleReleased()?1:0));
}

void BoundSystemTouches::fromString(Consulate *es, const char *in ) {
}

void BoundSystemTime::fromString(Consulate *es, const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 string url=es->variables->EvaluateString(w);
 w=words.Next();
 target=es->variables->EvaluateString(w);
 SYSTEMTIME st;
 GetSystemTime(&st);
 es->variables->Set((target).c_str(),FORMAT("%d/%d/%d %d:%d:%d", (int) st.wMonth, (int) st.wDay, (int) st.wYear, (int) st.wHour, (int) st.wMinute, (int) st.wSecond ).c_str());
 es->variables->Set((target+string(".24")).c_str(),FORMAT("%d:%d:%d",(int) st.wHour, (int) st.wMinute, (int) st.wSecond).c_str());
 es->variables->Set((target+string(".12")).c_str(),FORMAT("%d:%d:%d%s", st.wHour > 12 ? (int) st.wHour - 12 : st.wHour == 0 ? 1 : (int) st.wHour, (int) st.wMinute, (int) st.wSecond, st.wHour >= 12 ? "pm" : "am" ).c_str() );
 es->variables->Set((target+string(".date")).c_str(),FORMAT("%d/%d/%d", (int) st.wMonth, (int) st.wDay, (int) st.wYear ).c_str());
 es->variables->Set((target+string(".date.12")).c_str(),FORMAT("%d/%d/%d %d:%d:%d%s", (int) st.wMonth, (int) st.wDay, (int) st.wYear, st.wHour > 12 ? (int) st.wHour - 12 : st.wHour == 0 ? 1 : (int) st.wHour, (int) st.wMinute, (int) st.wSecond, st.wHour >= 12 ? "pm" : "am" ).c_str());
 es->variables->Set((target+string(".hour")).c_str(),(int)(st.wHour));
 es->variables->Set((target+string(".minute")).c_str(),(int)(st.wMinute));
 es->variables->Set((target+string(".second")).c_str(),(int)(st.wSecond));
 es->variables->Set((target+string(".day")).c_str(),(int)(st.wDay));
 es->variables->Set((target+string(".month")).c_str(),(int)(st.wMonth));
 es->variables->Set((target+string(".year")).c_str(),(int)(st.wYear));
 es->variables->Set((target+string(".dow")).c_str(),(int)(st.wDayOfWeek));
 switch ( st.wDayOfWeek ) {
  case 0: es->variables->Set((target+string(".dow.name")).c_str(),"Sunday"); break;
  case 1: es->variables->Set((target+string(".dow.name")).c_str(),"Monday"); break;
  case 2: es->variables->Set((target+string(".dow.name")).c_str(),"Tuesday"); break;
  case 3: es->variables->Set((target+string(".dow.name")).c_str(),"Wednesday"); break;
  case 4: es->variables->Set((target+string(".dow.name")).c_str(),"Thursday"); break;
  case 5: es->variables->Set((target+string(".dow.name")).c_str(),"Friday"); break;
  case 6: es->variables->Set((target+string(".dow.name")).c_str(),"Saturday"); break;
  default: es->variables->Set((target+string(".dow.name")).c_str(),"Unknown"); break;
 }
 switch ( st.wDayOfWeek ) {
  case 0: es->variables->Set((target+string(".dow.abbr")).c_str(),"Sun"); break;
  case 1: es->variables->Set((target+string(".dow.abbr")).c_str(),"Mon"); break;
  case 2: es->variables->Set((target+string(".dow.abbr")).c_str(),"Tue"); break;
  case 3: es->variables->Set((target+string(".dow.abbr")).c_str(),"Wed"); break;
  case 4: es->variables->Set((target+string(".dow.abbr")).c_str(),"Thu"); break;
  case 5: es->variables->Set((target+string(".dow.abbr")).c_str(),"Fri"); break;
  case 6: es->variables->Set((target+string(".dow.abbr")).c_str(),"Sat"); break;
  default: es->variables->Set((target+string(".dow.abbr")).c_str(),"???"); break;
 }
 es->variables->Set((target+string(".millis")).c_str(),(int)(st.wMilliseconds));
}

// line.function

void BoundLinewidth::fromString( Consulate *es, const char *in ) {
 setting=(float)es->variables->EvaluateDecimal(in);
 glLineWidth(setting.value);
}