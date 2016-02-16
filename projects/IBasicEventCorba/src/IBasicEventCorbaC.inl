// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.3.1
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.dre.vanderbilt.edu/~schmidt/TAO.html
 **/


// TAO_IDL - Generated from
// be\be_visitor_union\union_ci.cpp:39

// *************************************************************
// Inline operations for union IBASICEVENTCORBA_module::Arg
// *************************************************************

// TAO_IDL - Generated from
// be\be_visitor_union\discriminant_ci.cpp:58

ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::_default ()
{
  this->_reset ();
  this->disc_ = static_cast <IBASICEVENTCORBA_module::VariantTypes> (-1);
}

// Accessor to set the discriminant.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::_d (IBASICEVENTCORBA_module::VariantTypes discval)
{
  this->disc_ = discval;
}

// Accessor to get the discriminant.
ACE_INLINE
IBASICEVENTCORBA_module::VariantTypes
IBASICEVENTCORBA_module::Arg::_d (void) const
{
  return this->disc_;
}

// TAO_IDL - Generated from
// be\be_visitor_union_branch\public_ci.cpp:580

// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::lValue (CORBA::Long val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_LONG;
  // Set the value.
  this->u_.lValue_ = val;
}

/// Retrieve the member.
ACE_INLINE
::CORBA::Long
IBASICEVENTCORBA_module::Arg::lValue (void) const
{
  return this->u_.lValue_;
}

// TAO_IDL - Generated from
// be\be_visitor_union_branch\public_ci.cpp:580

// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::ulValue (CORBA::ULong val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_ULONG;
  // Set the value.
  this->u_.ulValue_ = val;
}

/// Retrieve the member.
ACE_INLINE
::CORBA::ULong
IBASICEVENTCORBA_module::Arg::ulValue (void) const
{
  return this->u_.ulValue_;
}

// TAO_IDL - Generated from
// be\be_visitor_union_branch\public_ci.cpp:580

// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::dValue (CORBA::Double val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_DOUBLE;
  // Set the value.
  this->u_.dValue_ = val;
}

/// Retrieve the member.
ACE_INLINE
::CORBA::Double
IBASICEVENTCORBA_module::Arg::dValue (void) const
{
  return this->u_.dValue_;
}

// TAO_IDL - Generated from
// be\be_visitor_union_branch\public_ci.cpp:580

// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::llValue (CORBA::ULongLong val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_LONGLONG;
  // Set the value.
  this->u_.llValue_ = val;
}

/// Retrieve the member.
ACE_INLINE
::CORBA::ULongLong
IBASICEVENTCORBA_module::Arg::llValue (void) const
{
  return this->u_.llValue_;
}

// TAO_IDL - Generated from
// be\be_visitor_union_branch\public_ci.cpp:580

// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::ullValue (CORBA::ULongLong val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_ULONGLONG;
  // Set the value.
  this->u_.ullValue_ = val;
}

/// Retrieve the member.
ACE_INLINE
::CORBA::ULongLong
IBASICEVENTCORBA_module::Arg::ullValue (void) const
{
  return this->u_.ullValue_;
}

// TAO_IDL - Generated from
// be\be_visitor_union_branch\public_ci.cpp:865

/// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::strValue (char *val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_STRING;
  this->u_.strValue_ = val;
}

// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::strValue (const char *val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_STRING;
  // Set the value.
  this->u_.strValue_ = ::CORBA::string_dup (val);
}

/// Accessor to set the member.
ACE_INLINE
void
IBASICEVENTCORBA_module::Arg::strValue (const ::CORBA::String_var &val)
{
  // Set the discriminant value.
  this->_reset ();
  this->disc_ = IBASICEVENTCORBA_module::VT_STRING;
  // Set the value.
  ::CORBA::String_var strValue_var = val;
  this->u_.strValue_ = strValue_var._retn ();
}

ACE_INLINE
const char *
IBASICEVENTCORBA_module::Arg::strValue (void) const // get method
{
  return this->u_.strValue_;
}

