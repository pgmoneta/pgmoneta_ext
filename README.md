# pgmoneta_ext

`pgmoneta` is a backup / restore solution for [PostgreSQL](https://www.postgresql.org).

`pgmoneta` is named after the Roman Goddess of Memory.

`pgmoneta_ext` is an extension to enhance the functionality of the main `pgmoneta` project.

## Overview

`pgmoneta_ext` is a PostgreSQL extension.

## Tested platforms

* [Fedora](https://getfedora.org/) 32+
* [RHEL](https://www.redhat.com/en/technologies/linux-platforms/enterprise-linux) 8.x with
  [AppStream](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/installing_managing_and_removing_user-space_components/using-appstream_using-appstream)

* [FreeBSD](https://www.freebsd.org/)
* [OpenBSD](http://www.openbsd.org/)

## Compiling the source

`pgmoneta_ext` requires

* [gcc 8+](https://gcc.gnu.org) (C17)
* [cmake](https://cmake.org)
* [make](https://www.gnu.org/software/make/)
* [PostgreSQL](https://www.postgresql.org)

```sh
dnf install git gcc cmake make postgresql-server-devel
```

Alternative [clang 8+](https://clang.llvm.org/) can be used.

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

### Debug build

The following commands will create a `DEBUG` version of `pgmoneta_ext`.

```sh
git clone https://github.com/pgmoneta/pgmoneta_ext
cd pgmoneta_ext
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Contributing

Contributions to `pgmoneta_ext` are managed on [GitHub.com](https://github.com/pgmoneta/pgmoneta_ext)

* [Ask a question](https://github.com/pgmoneta/pgmoneta_ext/discussions)
* [Raise an issue](https://github.com/pgmoneta/pgmoneta_ext/issues)
* [Feature request](https://github.com/pgmoneta/pgmoneta_ext/issues)
* [Code submission](https://github.com/pgmoneta/pgmoneta_ext/pulls)

Contributions are most welcome !

Please, consult our [Code of Conduct](./CODE_OF_CONDUCT.md) policies for interacting in our
community.

Consider giving the project a [star](https://github.com/pgmoneta/pgmoneta_ext/stargazers) on
[GitHub](https://github.com/pgmoneta/pgmoneta_ext/) if you find it useful. And, feel free to follow
the project on [Twitter](https://twitter.com/pgmoneta/) as well.

## License

[BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)
