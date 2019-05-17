MSentry
===

[Online documentation](https://docs.milosolutions.com/milo-code-db/msentry)

[Source code](https://github.com/milosolutions/msentry)

# Introduction

Raven is a Sentry (logs agregator) client for C++/Qt applications.

Raven is based on QRaven [Reference link] (https://github.com/TruePositiveLab/qraven)

# Quick start

## Basic usage 

Raven client uses instance mode and can be initialized with a single DSN string

```
MSentry::instance()->setSentryDSN(dsn);
```

After initialized, send log message to sentry server is rather simple:

```
sentry(RAVEN_DEBUG)<<"Debug message" << RavenMessage::send;
sentry(RAVEN_INFO)<<"Info message" << RavenMessage::send;
sentry(RAVEN_WARNING)<<"Warning message" << RavenMessage::send;
sentry(RAVEN_ERROR)<<"Error message" << RavenMessage::send;
sentry(RAVEN_FATAL)<<"Fatal Message" << RavenMessage::send;
```
	
## Advanced usage

If you have some extra information to send:
```
sentry(RAVEN_DEBUG) << "Tagged debug message" << RavenTag("tag_name", "tag_value") << RavenMessage::send;
```

# Deployment

1. Copy sources to your project.
2. In your project file include *.pri
3. You do not need to use `#ifdef` if you want to disable Sentry - it will still compile and run just fine

# Config options

```
qmake -config [options]
```

* no-sentry - disable Sentry
* no-qraven-stacktrace - disable stack trace recovery
* qraven-stacktrace - enable stack trace recovery
* no-sentry-print - disable console printing
* sentry-print - enable console printing

# Callstack support

Not all platforms supports the backtrace. Currently implemented backtrace for
unix* like platforms: MacOS, Linux. In future Windows support is planned.

# License

This project is licensed under the MIT License - see the LICENSE-MiloCodeDB.txt file for details
