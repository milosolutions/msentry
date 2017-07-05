#include "ravendefs.h"

#ifdef SENTRY
#include "raven.h"
#endif

#include <QJsonArray>

#ifdef QRAVEN_STACKTRACE
#ifndef Q_OS_WIN
    #include <dlfcn.h>
    #include <execinfo.h>
    #include <cxxabi.h>
#endif
#endif // QRAVEN_STACKTRACE

/*!
  * \class RavenMessage
  * \brief Holds and manages a single message, remembers the location where
  * call happened, call stack if needed.
  */

/*!
 * \brief Demangle a method name (C++ linkage specification required method name to be mangled and compiler implementation dependent) and reaturn a "readable" name.
 * \param to_demangle Mangled method
 * \return Human readable method
 */
QString util_demangle(const char *to_demangle)
{
#ifdef Q_OS_WIN
    return QString::fromLatin1(to_demangle);
#elif defined(QRAVEN_STACKTRACE)

    int status = 0;
    char* buff = __cxxabiv1::__cxa_demangle(to_demangle, NULL, NULL, &status);
    QString result = buff ? buff : to_demangle;
    free(buff);
    return result;
#else
    Q_UNUSED(to_demangle);
    return QString::null;
#endif
}

/*!
 * \brief Adds a message to itself
 * \param message
 * \return Self
 */
RavenMessage& RavenMessage::operator<<(const QString& message)
{    
#ifdef SENTRY
    m_body["message"] = message;
#else
    Q_UNUSED(message);
#endif
    return *this;
}

/*!
 * \brief Adds a exception description
 * \param exc Standard exception. See std::exception
 * \return Self
 */
RavenMessage& RavenMessage::operator<<(const std::exception& exc)
{    
#ifdef SENTRY
    m_body["message"] = exc.what();
#else
    Q_UNUSED(exc);
#endif

#ifdef QRAVEN_STACKTRACE
#ifndef Q_OS_WIN
    QJsonArray frameList;
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    QString moduleName;
    for (int i = 0; i < frames; ++i) {
        QJsonObject frame;
        Dl_info dlinfo;
        if (dladdr(callstack[i], &dlinfo) == 0)
            continue;
        if (i == 0) {
            moduleName = dlinfo.dli_fname;
            continue;
        }
        frame["function"] = util_demangle(dlinfo.dli_sname);
        frame["module"] = dlinfo.dli_fname;
        frame["in_app"] = false;
        frameList.push_back(frame);
    }
    QJsonObject frameHash;
    frameHash["frames"] = frameList;

    QJsonObject _exc;
    _exc["type"] = util_demangle(__cxxabiv1::__cxa_current_exception_type()->name());
    _exc["stacktrace"] = frameHash;
    _exc["module"] = moduleName;
    _exc["value"] = exc.what();

    QJsonArray values;
    values.push_back(_exc);

    QJsonObject exceptionReport;
    exceptionReport["values"] = values;

    m_body["exception"] = values;
#endif
#endif

    return *this;
}

/*!
 * \brief Adds a tag
 * \param tag See RavenTag
 * \return
 */
RavenMessage& RavenMessage::operator<<(const RavenTag& tag)
{
#ifdef SENTRY
    m_tags[tag.first] = tag.second;
#else
    Q_UNUSED(tag);
#endif
    return *this;
}

/*!
 * \brief Handler of RavenMessage::send method
 * \param pf Function pointer RavenMessage& (*pf)(RavenMessage&)
 * \return Self
 */
RavenMessage& RavenMessage::operator<<(RavenMessage& (*pf)(RavenMessage&))
{
    return (*pf)(*this);
}

/*!
 * \brief Formats output of given file, func, line
 * \param file
 * \param func
 * \param line
 * \return Formatted string
 */
QString RavenMessage::locationInfo(const char *file, const char *func, int line)
{    
#ifdef SENTRY
    return QString("%1 in %2 at %3").arg(file, func, QString::number(line));
#else
    Q_UNUSED(file);
    Q_UNUSED(func);
    Q_UNUSED(line);
    return QString::null;
#endif
}

/*!
 * Sends the \a message to the server. If sentry-print is enabled, it will also
 * print the \a message on the console.
 */
RavenMessage &RavenMessage::send(RavenMessage &message)
{
#ifdef SENTRY
    emit message.m_instance->capture(message);
#endif

    return message;
}
