#include "Variadics.h"

VariableStore variableStore;

VariadicType HintToType( const char *hint ) {
/* if ( *hint == '^' ) return v_is_reference;
 else */
 if ( is_integer(hint) ) {
  return v_is_integer;
 } else if ( is_decimal(hint) ) {
  return v_is_decimal;
 } else if ( !str_cmp(hint,"cartesian") ) {
  return v_cartesian;
 } else if ( !str_cmp(hint,"vertex") ) {
  return v_vertex;
 } else if ( !str_cmp(hint,"string") ) {
  return v_string;
 } else if ( !str_cmp(hint,"integer") || !str_cmp(hint,"int") ) {
  return v_integer;
 } else if ( !str_cmp(hint,"decimal") ) {
  return v_decimal;
 } else if ( !str_cmp(hint,"crayon") || !str_cmp(hint,"color") ) {
  return v_crayon;
 } else return v_unknown;
}

VariadicType GuessType( const char *hint ) {
 if ( *hint == '^' ) return v_is_reference;
 else if ( is_integer(hint) ) {
  return v_is_integer;
 } else if ( is_decimal(hint) ) {
  return v_is_decimal;
 } else return v_unknown;
}