\newpage

# Introduction

[**pgmoneta**][pgmoneta] is a backup / restore solution for [PostgreSQL][postgresql].

**pgmoneta** is named after the Roman Goddess of Memory.

[**pgmoneta_ext**][pgmoneta_ext] is an extension to enhance the functionality of the main **pgmoneta** project.

## Overview

**pgmoneta_ext** is a PostgreSQL extension.

This extension will be installed on the server side and is primarily designed to help pgmoneta perform delta backups. There will be an extra parameter in the pgmoneta configuration; if the user sets this parameter, the server side must have this extension installed to make it work.

## Platforms

The supported platforms are

* [Fedora][fedora] 32+
* [RHEL][rhel] 8 / RockyLinux 8
* [RHEL][rhel] 9 / RockyLinux 9
* [FreeBSD][freebsd]
* [OpenBSD][openbsd]