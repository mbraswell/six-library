# This file was automatically generated by SWIG (http://www.swig.org).
# Version 4.0.2
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info < (2, 7, 0):
    raise RuntimeError("Python 2.7 or later required")

# Import the low-level C/C++ module
if __package__ or "." in __name__:
    from . import _coda_logging
else:
    import _coda_logging

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_instance_variable(set):
    def set_instance_attr(self, name, value):
        if name == "thisown":
            self.this.own(value)
        elif name == "this":
            set(self, name, value)
        elif hasattr(self, name) and isinstance(getattr(type(self), name), property):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add instance attributes to %s" % self)
    return set_instance_attr


def _swig_setattr_nondynamic_class_variable(set):
    def set_class_attr(cls, name, value):
        if hasattr(cls, name) and not isinstance(getattr(cls, name), property):
            set(cls, name, value)
        else:
            raise AttributeError("You cannot add class attributes to %s" % cls)
    return set_class_attr


def _swig_add_metaclass(metaclass):
    """Class decorator for adding a metaclass to a SWIG wrapped class - a slimmed down version of six.add_metaclass"""
    def wrapper(cls):
        return metaclass(cls.__name__, cls.__bases__, cls.__dict__.copy())
    return wrapper


class _SwigNonDynamicMeta(type):
    """Meta class to enforce nondynamic attributes (no new attributes) for a class"""
    __setattr__ = _swig_setattr_nondynamic_class_variable(type.__setattr__)


class Formatter(object):
    r"""Proxy of C++ logging::Formatter class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _coda_logging.delete_Formatter

    def format(self, record: "LogRecord const *", os: "io::OutputStream &") -> "void":
        r"""format(Formatter self, LogRecord const * record, io::OutputStream & os)"""
        return _coda_logging.Formatter_format(self, record, os)

    def getPrologue(self) -> "std::string":
        r"""getPrologue(Formatter self) -> std::string"""
        return _coda_logging.Formatter_getPrologue(self)

    def getEpilogue(self) -> "std::string":
        r"""getEpilogue(Formatter self) -> std::string"""
        return _coda_logging.Formatter_getEpilogue(self)

# Register Formatter in _coda_logging:
_coda_logging.Formatter_swigregister(Formatter)

class StandardFormatter(Formatter):
    r"""Proxy of C++ logging::StandardFormatter class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""
        __init__(StandardFormatter self) -> StandardFormatter
        __init__(StandardFormatter self, std::string const & fmt, std::string const & prologue="", std::string const & epilogue="") -> StandardFormatter
        """
        _coda_logging.StandardFormatter_swiginit(self, _coda_logging.new_StandardFormatter(*args))
    __swig_destroy__ = _coda_logging.delete_StandardFormatter

    def format(self, record: "LogRecord const *", os: "io::OutputStream &") -> "void":
        r"""format(StandardFormatter self, LogRecord const * record, io::OutputStream & os)"""
        return _coda_logging.StandardFormatter_format(self, record, os)

# Register StandardFormatter in _coda_logging:
_coda_logging.StandardFormatter_swigregister(StandardFormatter)
cvar = _coda_logging.cvar
StandardFormatter.DEFAULT_FORMAT = _coda_logging.cvar.StandardFormatter_DEFAULT_FORMAT

class Filterer(object):
    r"""Proxy of C++ logging::Filterer class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self):
        r"""__init__(Filterer self) -> Filterer"""
        _coda_logging.Filterer_swiginit(self, _coda_logging.new_Filterer())
    __swig_destroy__ = _coda_logging.delete_Filterer

    def addFilter(self, filter: "Filter *") -> "void":
        r"""addFilter(Filterer self, Filter * filter)"""
        return _coda_logging.Filterer_addFilter(self, filter)

    def filter(self, record: "LogRecord const *") -> "bool":
        r"""filter(Filterer self, LogRecord const * record) -> bool"""
        return _coda_logging.Filterer_filter(self, record)

    def removeFilter(self, filter: "Filter *") -> "void":
        r"""removeFilter(Filterer self, Filter * filter)"""
        return _coda_logging.Filterer_removeFilter(self, filter)

# Register Filterer in _coda_logging:
_coda_logging.Filterer_swigregister(Filterer)

class Handler(Filterer):
    r"""Proxy of C++ logging::Handler class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _coda_logging.delete_Handler

    def setLevel(self, level: "LogLevel") -> "void":
        r"""setLevel(Handler self, LogLevel level)"""
        return _coda_logging.Handler_setLevel(self, level)

    def getLevel(self) -> "LogLevel":
        r"""getLevel(Handler self) -> LogLevel"""
        return _coda_logging.Handler_getLevel(self)

    def handle(self, record: "LogRecord const *") -> "bool":
        r"""handle(Handler self, LogRecord const * record) -> bool"""
        return _coda_logging.Handler_handle(self, record)

    def close(self) -> "void":
        r"""close(Handler self)"""
        return _coda_logging.Handler_close(self)

    def setFormatter(self, formatter: "Formatter") -> "void":
        r"""setFormatter(Handler self, Formatter formatter)"""
        return _coda_logging.Handler_setFormatter(self, formatter)

# Register Handler in _coda_logging:
_coda_logging.Handler_swigregister(Handler)

class StreamHandler(Handler):
    r"""Proxy of C++ logging::StreamHandler class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""
        __init__(StreamHandler self, LogLevel level=LogLevel::LOG_NOTSET) -> StreamHandler
        __init__(StreamHandler self, io::OutputStream * stream, LogLevel level=LogLevel::LOG_NOTSET) -> StreamHandler
        __init__(StreamHandler self, std::unique_ptr< io::OutputStream > && stream, LogLevel level=LogLevel::LOG_NOTSET) -> StreamHandler
        """
        _coda_logging.StreamHandler_swiginit(self, _coda_logging.new_StreamHandler(*args))
    __swig_destroy__ = _coda_logging.delete_StreamHandler

    def close(self) -> "void":
        r"""close(StreamHandler self)"""
        return _coda_logging.StreamHandler_close(self)

# Register StreamHandler in _coda_logging:
_coda_logging.StreamHandler_swigregister(StreamHandler)

class FileHandler(StreamHandler):
    r"""Proxy of C++ logging::FileHandler class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""__init__(FileHandler self, sys::Filesystem::path const & fname, LogLevel level=LogLevel::LOG_NOTSET, int creationFlags=sys::File::CREATE|sys::File::TRUNCATE) -> FileHandler"""
        _coda_logging.FileHandler_swiginit(self, _coda_logging.new_FileHandler(*args))
    __swig_destroy__ = _coda_logging.delete_FileHandler

# Register FileHandler in _coda_logging:
_coda_logging.FileHandler_swigregister(FileHandler)

class Filter(object):
    r"""Proxy of C++ logging::Filter class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""__init__(Filter self, std::string name="") -> Filter"""
        _coda_logging.Filter_swiginit(self, _coda_logging.new_Filter(*args))
    __swig_destroy__ = _coda_logging.delete_Filter

    def filter(self, record: "LogRecord const *") -> "bool":
        r"""filter(Filter self, LogRecord const * record) -> bool"""
        return _coda_logging.Filter_filter(self, record)

    def getName(self) -> "std::string":
        r"""getName(Filter self) -> std::string"""
        return _coda_logging.Filter_getName(self)

# Register Filter in _coda_logging:
_coda_logging.Filter_swigregister(Filter)

class Logger(Filterer):
    r"""Proxy of C++ logging::Logger class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""__init__(Logger self, std::string const & name="") -> Logger"""
        _coda_logging.Logger_swiginit(self, _coda_logging.new_Logger(*args))
    __swig_destroy__ = _coda_logging.delete_Logger

    def log(self, *args) -> "void":
        r"""
        log(Logger self, LogLevel level, std::string const & msg)
        log(Logger self, LogLevel level, except::Context const & ctxt)
        log(Logger self, LogLevel level, except::Throwable const & t)
        """
        return _coda_logging.Logger_log(self, *args)

    def debug(self, *args) -> "void":
        r"""
        debug(Logger self, std::string const & msg)
        debug(Logger self, except::Context const & ctxt)
        debug(Logger self, except::Throwable const & t)
        """
        return _coda_logging.Logger_debug(self, *args)

    def info(self, *args) -> "void":
        r"""
        info(Logger self, std::string const & msg)
        info(Logger self, except::Context const & ctxt)
        info(Logger self, except::Throwable const & t)
        """
        return _coda_logging.Logger_info(self, *args)

    def warn(self, *args) -> "void":
        r"""
        warn(Logger self, std::string const & msg)
        warn(Logger self, except::Context const & ctxt)
        warn(Logger self, except::Throwable const & t)
        """
        return _coda_logging.Logger_warn(self, *args)

    def error(self, *args) -> "void":
        r"""
        error(Logger self, std::string const & msg)
        error(Logger self, except::Context const & ctxt)
        error(Logger self, except::Throwable const & t)
        """
        return _coda_logging.Logger_error(self, *args)

    def critical(self, *args) -> "void":
        r"""
        critical(Logger self, std::string const & msg)
        critical(Logger self, except::Context const & ctxt)
        critical(Logger self, except::Throwable const & t)
        """
        return _coda_logging.Logger_critical(self, *args)

    def addHandler(self, handler: "Handler", own: "bool"=False) -> "void":
        r"""addHandler(Logger self, Handler handler, bool own=False)"""
        return _coda_logging.Logger_addHandler(self, handler, own)

    def removeHandler(self, handler: "Handler") -> "void":
        r"""removeHandler(Logger self, Handler handler)"""
        return _coda_logging.Logger_removeHandler(self, handler)

    def setLevel(self, level: "LogLevel") -> "void":
        r"""setLevel(Logger self, LogLevel level)"""
        return _coda_logging.Logger_setLevel(self, level)

    def setName(self, name: "std::string const &") -> "void":
        r"""setName(Logger self, std::string const & name)"""
        return _coda_logging.Logger_setName(self, name)

    def getName(self) -> "std::string":
        r"""getName(Logger self) -> std::string"""
        return _coda_logging.Logger_getName(self)

    def reset(self) -> "void":
        r"""reset(Logger self)"""
        return _coda_logging.Logger_reset(self)

# Register Logger in _coda_logging:
_coda_logging.Logger_swigregister(Logger)

class NullHandler(Handler):
    r"""Proxy of C++ logging::NullHandler class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""__init__(NullHandler self, LogLevel level=LogLevel::LOG_NOTSET) -> NullHandler"""
        _coda_logging.NullHandler_swiginit(self, _coda_logging.new_NullHandler(*args))
    __swig_destroy__ = _coda_logging.delete_NullHandler

# Register NullHandler in _coda_logging:
_coda_logging.NullHandler_swigregister(NullHandler)

class NullLogger(Logger):
    r"""Proxy of C++ logging::NullLogger class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""__init__(NullLogger self, std::string const & name="") -> NullLogger"""
        _coda_logging.NullLogger_swiginit(self, _coda_logging.new_NullLogger(*args))
    __swig_destroy__ = _coda_logging.delete_NullLogger

# Register NullLogger in _coda_logging:
_coda_logging.NullLogger_swigregister(NullLogger)

class LoggerManager(object):
    r"""Proxy of C++ logging::LoggerManager class."""

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self):
        r"""__init__(LoggerManager self) -> LoggerManager"""
        _coda_logging.LoggerManager_swiginit(self, _coda_logging.new_LoggerManager())

    def getLoggerSharedPtr(self, *args) -> "std::shared_ptr< logging::Logger >":
        r"""getLoggerSharedPtr(LoggerManager self, std::string const & name="root") -> std::shared_ptr< logging::Logger >"""
        return _coda_logging.LoggerManager_getLoggerSharedPtr(self, *args)

    def getLogger(self, *args) -> "logging::Logger *":
        r"""getLogger(LoggerManager self, std::string const & name="root") -> Logger"""
        return _coda_logging.LoggerManager_getLogger(self, *args)
    __swig_destroy__ = _coda_logging.delete_LoggerManager

# Register LoggerManager in _coda_logging:
_coda_logging.LoggerManager_swigregister(LoggerManager)


def debug(*args) -> "void":
    r"""
    debug(std::string const & msg)
    debug(except::Context const & ctxt)
    debug(except::Throwable & t)
    """
    return _coda_logging.debug(*args)

def info(*args) -> "void":
    r"""
    info(std::string const & msg)
    info(except::Context const & ctxt)
    info(except::Throwable & t)
    """
    return _coda_logging.info(*args)

def warn(*args) -> "void":
    r"""
    warn(std::string const & msg)
    warn(except::Context const & ctxt)
    warn(except::Throwable & t)
    """
    return _coda_logging.warn(*args)

def error(*args) -> "void":
    r"""
    error(std::string const & msg)
    error(except::Context const & ctxt)
    error(except::Throwable & t)
    """
    return _coda_logging.error(*args)

def critical(*args) -> "void":
    r"""
    critical(std::string const & msg)
    critical(except::Context const & ctxt)
    critical(except::Throwable & t)
    """
    return _coda_logging.critical(*args)

def getLogger(*args) -> "logging::Logger *":
    
    return _coda_logging.getLogger(*args)

def getLoggerSharedPtr(*args) -> "std::shared_ptr< logging::Logger >":
    
    return _coda_logging.getLoggerSharedPtr(*args)


