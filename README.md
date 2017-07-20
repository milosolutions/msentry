\anchor MSentry 

Milo Code DB main ([online](https://qtdocs.milosolutions.com/milo-code-db/main/) | [offline](\ref milodatabasemain)) 

# Introduction

MRaven is a Sentry (logs agregator) client for C++/Qt applications.

# Quick start

### Basic usage 

MRaven client uses instance mode and can be initialized with a single DSN string

	MSentry::instance()->setSentryDSN(dsn);

After initialized, send log message to sentry server is rather simple:

	sentry(RAVEN_DEBUG)<<"Debug message" << MRavenMessage::send;
	sentry(RAVEN_INFO)<<"Info message" << MRavenMessage::send;
	sentry(RAVEN_WARNING)<<"Warning message" << MRavenMessage::send;
    sentry(RAVEN_ERROR)<<"Error message" << MRavenMessage::send;
    sentry(RAVEN_FATAL)<<"Fatal Message" << MRavenMessage::send;
	
### Advanced usage

If you have some extra information to send:

	sentry(RAVEN_DEBUG) << "Tagged debug message" << MRavenTag("tag_name", "tag_value") << MRavenMessage::send;

# Deployment

1. Copy sources to your project.
2. In your project file include *.pri
3. You do not need to use #ifdef if you want to disable Sentry - it will still compile and run just fine

# Config options

qmake -config [options]

* no-sentry - disable Sentry
* no-qraven-stacktrace - disable stack trace recovery
* qraven-stacktrace - enable stack trace recovery
* no-sentry-print - disable console printing
* sentry-print - enable console printing

# Callstack support

Not all platforms supports the backtrace. Currently implemented backtrace for
unix* like platforms: MacOS, Linux. In future Windows support is planned.

# License # {#license}

This project is licensed under the MIT License - see the LICENSE-MiloCodeDB.txt file for details