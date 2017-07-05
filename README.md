# Introduction

QRaven is a Sentry client for C++/Qt applications

# Deployment

1. Copy sources to your project.
2. In your project file include *.pri

# Callstack support

Not all platforms supports the backtrace. Currently implemented backtrace for
unix* like platforms: MacOS, Linux. In future Windows support is planned.

## Unix* like

To enable callstack support add linker flag `-rdynamic`

```
    QMAKE_LFLAGS += -rdynamic
```

# License # {#license}

This project is licensed under the MIT License - see the LICENSE-MiloCodeDB.txt file for details