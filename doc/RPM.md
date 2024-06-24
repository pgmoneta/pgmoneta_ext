# RPM

`pgmoneta_ext` can be built into a RPM for [Fedora](https://getfedora.org/) systems.

## Requirements

```sh
dnf install rpmdevtools rpmlint
```

## Setup RPM development

```sh
rpmdev-setuptree
```

## Create source package

```sh
git clone https://github.com/pgmoneta/pgmoneta_ext
cd pgmoneta_ext
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make package_source
```

## Create RPM package

```sh
cp pgmoneta_ext-$VERSION.tar.gz ~/rpmbuild/SOURCES/$VERSION.tar.gz
rpmbuild -bb pgmoneta.spec
```

The resulting RPM will be located in `~/rpmbuild/RPMS/aarch64/`, if your architecture is `aarch64`.

You can use `tree ~/rpmbuild/` to check the directory structure

## Install the RPM package

After a successful build of the package, you can install the RPM package using the dnf command:

``` sh
dnf install ~/rpmbuild/RPMS/aarch64/pgmoneta_ext-0.1.0-1.fc38.aarch64.rpm
```

## Verify the package has been installed

To verify the package has correctly been installed, run the following command:

```sh
rpm -qi pgmoneta_ext
```

And you can see which files the RPM package contains:

```sh
rpm -ql pgmoneta_ext
```

## Removing the RPM package

Removing the package from the system using the dnf command:

``` sh
dnf remove pgmoneta_ext
```
