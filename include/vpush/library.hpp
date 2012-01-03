#ifndef __VPUSH_LIBRARY_HPP__
#define __VPUSH_LIBRARY_HPP__

#include <string>
#include <vpush/stackops.hpp>
#include <vpush/detail/typechecker.hpp>
#include <vpush/detail/functions.hpp>

#define VPUSH_DECL_LIBRARY(name) namespace name {void initialize();}
#define VPUSH_DEF_LIBRARY(name) namespace name

namespace vpush {
namespace library {

using vpush::functions;
using vpush::type;
using detail::Code;
using detail::Exec;
using detail::item;
using detail::get_item;
using detail::get_nth_item;
using detail::put_item;

// main initialization
void initialize();

///////////////////////////////
// sub-library initializations
///////////////////////////////

VPUSH_DECL_LIBRARY(arithmetic)
VPUSH_DECL_LIBRARY(basic)
VPUSH_DECL_LIBRARY(boolean)
VPUSH_DECL_LIBRARY(code)
VPUSH_DECL_LIBRARY(combinators)
VPUSH_DECL_LIBRARY(comparison)
VPUSH_DECL_LIBRARY(conditional)
VPUSH_DECL_LIBRARY(gestation)
VPUSH_DECL_LIBRARY(interaction)
VPUSH_DECL_LIBRARY(math)
VPUSH_DECL_LIBRARY(movement)
VPUSH_DECL_LIBRARY(soup)
VPUSH_DECL_LIBRARY(stackops)

} // namespace library
} // namespace vpush

#endif

