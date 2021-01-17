/**
 * @brief General exceptions for vcr
 */

#include <stdexcept>

#pragma once

namespace vcr {


// Base exceptions for all the VCR lib
class VCRException : public std::runtime_error { using std::runtime_error::runtime_error; };
class DeviceError : public VCRException { using VCRException::VCRException; };

// Changer related exceptions
class ChangerException : public VCRException { using VCRException::VCRException; };

} // namespace vcr
