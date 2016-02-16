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
// be\be_codegen.cpp:461

#ifndef _TAO_IDL_IBASICEVENTCORBAS_OOI24L_H_
#define _TAO_IDL_IBASICEVENTCORBAS_OOI24L_H_

#include /**/ "ace/pre.h"

#include "IBasicEventCorbaC.h"
#include "tao/PortableServer/Basic_SArguments.h"
#include "tao/PortableServer/Special_Basic_SArguments.h"
#include "tao/PortableServer/Fixed_Size_SArgument_T.h"
#include "tao/PortableServer/Var_Size_SArgument_T.h"
#include "tao/PortableServer/UB_String_SArguments.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// TAO_IDL - Generated from
// be\be_visitor_arg_traits.cpp:68

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{

  // TAO_IDL - Generated from
  // be\be_visitor_arg_traits.cpp:904

  template<>
  class SArg_Traits< ::IBASICEVENTCORBA_module::VariantTypes>
    : public
        Basic_SArg_Traits_T<
            ::IBASICEVENTCORBA_module::VariantTypes,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be\be_visitor_arg_traits.cpp:1058

  template<>
  class SArg_Traits< ::IBASICEVENTCORBA_module::Arg>
    : public
        Var_Size_SArg_Traits_T<
            ::IBASICEVENTCORBA_module::Arg,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be\be_visitor_arg_traits.cpp:947

  template<>
  class SArg_Traits< ::IBASICEVENTCORBA_module::EventArg>
    : public
        Var_Size_SArg_Traits_T<
            ::IBASICEVENTCORBA_module::EventArg,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be\be_visitor_arg_traits.cpp:685

  template<>
  class SArg_Traits< ::IBASICEVENTCORBA_module::EventArgSeq>
    : public
        Var_Size_SArg_Traits_T<
            ::IBASICEVENTCORBA_module::EventArgSeq,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be\be_visitor_arg_traits.cpp:947

  template<>
  class SArg_Traits< ::IBASICEVENTCORBA_module::EventSpec>
    : public
        Var_Size_SArg_Traits_T<
            ::IBASICEVENTCORBA_module::EventSpec,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be\be_visitor_arg_traits.cpp:685

  template<>
  class SArg_Traits< ::IBASICEVENTCORBA_module::EventSpecSeq>
    : public
        Var_Size_SArg_Traits_T<
            ::IBASICEVENTCORBA_module::EventSpecSeq,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be\be_visitor_arg_traits.cpp:947

  template<>
  class SArg_Traits< ::IBASICEVENTCORBA_module::BasicEventCorba>
    : public
        Var_Size_SArg_Traits_T<
            ::IBASICEVENTCORBA_module::BasicEventCorba,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// be\be_visitor_module\module_sh.cpp:38

namespace POA_IBASICEVENTCORBA_module
{
  

// TAO_IDL - Generated from
// be\be_visitor_module\module_sh.cpp:69

} // module IBASICEVENTCORBA_module

#include "IBasicEventCorbaS_T.h"


#include /**/ "ace/post.h"

#endif /* ifndef */
