\newpage

# Quick start

## Compiling the source

We recommend using `Fedora` to test and run pgmoneta_ext.

`pgmoneta_ext` requires

* [gcc 8+][gcc] (C17)
* [cmake][cmake]
* [make][make]
* [PostgreSQL][postgresql]

```sh
dnf install git gcc cmake make postgresql-server-devel
```

Alternative [clang 8+][clang] can be used.

## Build

### Release build

The following commands will install `pgmoneta_ext`.

```sh
git clone https://github.com/pgmoneta/pgmoneta_ext
cd pgmoneta_ext
mkdir build
cd build
cmake ..
make
sudo make install
```

## Check

Make sure that `pgmoneta_ext` is installed. You should use `PostgreSQL` to test the extension functions. Since creating an extension requires the role to have superuser privileges, it is advisable to use the default `postgres` role to check if `pgmoneta_ext` has been installed successfully. After creating the extension, you can use a non-superuser role to test it.

``` sh
sudo su -
su - postgres
psql
CREATE DATABASE testdb;
\c testdb
DROP EXTENSION IF EXISTS pgmoneta_ext;
CREATE EXTENSION pgmoneta_ext;
SELECT pgmoneta_ext_version();
```

You should see

``` console
 pgmoneta_ext_version 
----------------------
 0.1.0
(1 row)
```

If you encounter any issues following the above steps, you can refer to the **Developer Guide** under **Install pgmoneta_ext** to see how to compile and install pgmoneta on your system.
