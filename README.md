# Boox Feed Reader

## Introduction

This is an RSS/Atom feed reader for the Onyx Boox ebook readers.

## Developer notes

### Development environment

The Boox Feed Reader can be built natively on X86 Linux, or
cross-compiled for ARM Linux using the toolchain and SDK libraries from
[here](http://dev.onyxcommunity.com/sdk/).

### Directory layout

The source code will build against a binary distribution of the Boox
SDK. The header files of the SDK should be placed at
    sdk/include
And the X86 libraries and the ARM libraries should be placed at
    sdk/lib/x86
and
    sdk/lib/arm
respectively.
