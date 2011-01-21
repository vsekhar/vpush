#ifndef __VPUSH_CODES_HPP__
#define __VPUSH_CODES_HPP__

#include <map>
#include <string>
#include <vpush/code_fwd.hpp>

namespace vpush {

typedef std::map<std::string, detail::code_base*> codes_t;

extern codes_t codes;

} // namespace vpush

#endif // __VPUSH_CODES_HPP__

