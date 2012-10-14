#ifndef EXCEPTIONTYPES_HPP_
#define EXCEPTIONTYPES_HPP_

#include <boost/exception/all.hpp>
#include <boost/current_function.hpp>
#include "enum.hpp"

namespace core {
/* Base exception struct */
struct Exception: virtual std::exception, virtual boost::exception {};

/*
 * Error severity
 *
 * Changes to this enumeration require changing:
 *      EXCEP_SEVERITY_..._STRING (in cpp)
 *      enumToValue<ExceptionSeverity, const std::string&> (in cpp)
 *      EXCEP_SEVERITY_STRING_BIMAP (in cpp)
 */
enum ExceptionSeverity {
    EXCEP_SEVERITY_NOTICE   = 0,
    EXCEP_SEVERITY_WARNING  = 5,
    EXCEP_SEVERITY_ERROR    = 10
};

typedef const boost::bimap<ExceptionSeverity, std::string> ServityStringMap;
extern ServityStringMap EXCEP_SEVERITY_STRING_BIMAP;

template<>
inline ServityStringMap& enumMappings<ExceptionSeverity, std::string>() {
    return EXCEP_SEVERITY_STRING_BIMAP;
}
// Optimization for const string&, which does no copying
template<>
const std::string& enumToValue<ExceptionSeverity, const std::string&>(ExceptionSeverity eval);

/*
 * Defines a generic exception throw without defaulted code
 * and severity settings. This is used by the exception throws
 * below.
 */
struct StrStarter;
extern const StrStarter strstart;

// Creates a core exception from meta data
#define ExceptionBasis(message, code, severity, exceptionType) \
    exceptionType() \
        << ::core::ThrowErrorMessage(strstart << message) \
        << ::core::ThrowErrorFunction(BOOST_CURRENT_FUNCTION) \
        << ::core::ThrowErrorFileName(__FILE__) \
        << ::core::ThrowErrorLineNumber(__LINE__) \
        << ::core::ThrowErrorCode(code) \
        << ::core::ThrowErrorSeverity(severity)

// Extends an existing boost::exception to include core except meta data
#define extendBasicException(message, code, severity, exception) \
    exception << ::core::ThrowErrorMessage(strstart << message) \
        << ::core::ThrowErrorFunction(BOOST_CURRENT_FUNCTION) \
        << ::core::ThrowErrorFileName(__FILE__) \
        << ::core::ThrowErrorLineNumber(__LINE__) \
        << ::core::ThrowErrorCode(code) \
        << ::core::ThrowErrorSeverity(severity)

// Defines an assertion statement that throws an exception upon failure
#define assertAsException(condition, message) \
    if (!condition) throw ExceptionBasis(message, \
            ::core::ASSERTION_EXCEPTION, \
            ::core::EXCEP_SEVERITY_ERROR, \
            ::core::Exception)

/*
 * Error codes -- These are all of the standard exception types
 *
 * Changes to this enumeration require changing:
 *      ..._EXCEPTION_STRING (in cpp)
 *      enumToValue<ExceptionCode, const std::string&> (in cpp)
 *      EXCEP_CODE_STRING_BIMAP (in cpp)
 */
enum ExceptionCode {
    UNKNOWN_EXCEPTION           = 0,
    ASSERTION_EXCEPTION         = 1,
    NULL_POINTER_EXCEPTION      = 2,
    CAST_EXCEPTION              = 3,
    INITIALIZATION_EXCEPTION    = 4,
    CALL_ONCE_EXCEPTION         = 5,
    IO_EXCEPTION                = 10,
    FILE_OPEN_EXCEPTION         = 11,
    PARAMETER_EXCEPTION         = 20,
    ATTRIBUTE_EXCEPTION         = 21,
    INSERT_FAILED_EXCEPTION     = 22,
    CONCURRENCY_EXCEPTION       = 30,
    RACE_CONDITION_EXCEPTION    = 31,
    DEADLOCK_EXCEPTION          = 32
};

typedef const boost::bimap<ExceptionCode, std::string> ExceptCodeStringMap;
extern ExceptCodeStringMap EXCEP_CODE_STRING_BIMAP;

template<>
inline ExceptCodeStringMap& enumMappings<ExceptionCode, std::string>() {
    return EXCEP_CODE_STRING_BIMAP;
}
// Optimization for const string&, which does no copying
template<>
const std::string& enumToValue<ExceptionCode, const std::string&>(ExceptionCode eval);

/* Typical exception attachments */
typedef boost::error_info<struct TagMessage, std::string> ThrowErrorMessage;
const std::string getExceptionMessage(const boost::exception & x);

typedef boost::error_info<struct TagErrorCode, ExceptionCode> ThrowErrorCode;
ExceptionCode getExceptionCode(const boost::exception& x);

typedef boost::error_info<struct TagErrorSeverity, ExceptionSeverity> ThrowErrorSeverity;
ExceptionSeverity getExceptionSeverity(const boost::exception& x);

typedef boost::throw_function ThrowErrorFunction;
const std::string getExceptionFunction(const boost::exception& x);

typedef boost::throw_file ThrowErrorFileName;
const std::string getExceptionFileName(const boost::exception& x);

typedef boost::throw_line ThrowErrorLineNumber;
int getExceptionLineNumber(const boost::exception& x);

/* Cast exception attachments */
typedef boost::error_info<struct TagCastSource, const std::type_info*> ThrowErrorCastSource;
const std::type_info& getExceptionCastSource(const boost::exception& x);

typedef boost::error_info<struct TagCastDest, const std::type_info*> ThrowErrorCastDest;
const std::type_info& getExceptionCastDestination(const boost::exception& x);

}
#endif /* EXCEPTIONTYPES_HPP_ */
