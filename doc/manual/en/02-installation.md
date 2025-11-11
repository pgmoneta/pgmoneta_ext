\newpage

# Installation

## Fedora

You need to add the [PostgreSQL YUM repository](https://yum.postgresql.org/), for example for Fedora 40

``` sh
dnf install -y https://download.postgresql.org/pub/repos/yum/reporpms/F-42-x86_64/pgdg-fedora-repo-latest.noarch.rpm
```

and do the install via

``` sh
dnf install -y pgmoneta_ext
```

Additional information

* [PostgreSQL YUM][psqlyum]
* [Linux downloads][linuxd]

## RHEL 9 / RockyLinux 9

```
dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm
dnf install -y https://download.postgresql.org/pub/repos/yum/reporpms/EL-9-x86_64/pgdg-redhat-repo-latest.noarch.rpm
```

``` sh
dnf install -y pgmoneta_ext
```

## RHEL 10 / RockyLinux 10

```
dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-10.noarch.rpm
dnf install -y https://download.postgresql.org/pub/repos/yum/reporpms/EL-10-x86_64/pgdg-redhat-repo-latest.noarch.rpm
```

``` sh
dnf install -y pgmoneta_ext
```

## Compiling the source

We recommend using Fedora to test and run [**pgmoneta_ext**][pgmoneta_ext], but other Linux systems, FreeBSD and MacOS are also supported.

[**pgmoneta_ext**][pgmoneta_ext] requires

* [gcc 8+][gcc] (C17)
* [cmake][cmake]
* [make][make]
* [PostgreSQL][postgresql]

```sh
dnf install git gcc cmake make postgresql-server postgresql-server-devel
```

### RHEL / RockyLinux

On RHEL / Rocky, before you install the required packages some additional repositories need to be enabled or installed first.

First you need to install the subscription-manager

``` sh
dnf install subscription-manager
```

It is ok to disregard the registration and subscription warning.

Otherwise, if you have a Red Hat corporate account (you need to specify the company/organization name in your account), you can register using

``` sh
subscription-manager register --username <your-account-email-or-login> --password <your-password> --auto-attach
```

Then install the EPEL repository,

``` sh
dnf install epel-release
```

Then to enable powertools

``` sh
# On RHEL 9 / Rocky 9, PowerTools is called crb (CodeReady Builder)
dnf config-manager --set-enabled codeready-builder-for-rhel-9-rhui-rpms
dnf config-manager --set-enabled crb
dnf install https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm

# On RHEL 10 / Rocky 10, PowerTools is called crb (CodeReady Builder)
dnf config-manager --set-enabled codeready-builder-for-rhel-10-rhui-rpms
dnf config-manager --set-enabled crb
dnf install https://dl.fedoraproject.org/pub/epel/epel-release-latest-10.noarch.rpm
```

Then use the `dnf` command for [**pgmoneta_ext**][pgmoneta_ext] to install the required packages.

### FreeBSD

On FreeBSD, `pkg` is used instead of `dnf` or `yum`.

Use `pkg install <package name>` to install the following packages

``` sh
git gcc cmake gmake postgresql14-server postgresql14-client
```

### Build

**Release build**

The following commands will install [**pgmoneta_ext**][pgmoneta_ext].

```sh
git clone https://github.com/pgmoneta/pgmoneta_ext
cd pgmoneta_ext
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=gcc ..
make
sudo make install
```

See [RPM][rpm] for how to build a RPM of [**pgmoneta_ext**][pgmoneta_ext].

**Debug build**

The following commands will create a `DEBUG` version of [**pgmoneta_ext**][pgmoneta_ext].

```sh
git clone https://github.com/pgmoneta/pgmoneta_ext
cd pgmoneta_ext
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Compiling the documentation

[**pgmoneta_ext**][pgmoneta_ext]'s documentation requires

* [pandoc](https://pandoc.org/)
* [texlive](https://www.tug.org/texlive/)

```sh
dnf install pandoc texlive-scheme-basic \
            'tex(footnote.sty)' 'tex(footnotebackref.sty)' \
            'tex(pagecolor.sty)' 'tex(hardwrap.sty)' \
            'tex(mdframed.sty)' 'tex(sourcesanspro.sty)' \
            'tex(ly1enc.def)' 'tex(sourcecodepro.sty)' \
            'tex(titling.sty)' 'tex(csquotes.sty)' \
            'tex(zref-abspage.sty)' 'tex(needspace.sty)'

```

You will need the `Eisvogel` template as well which you can install through

```
wget https://github.com/Wandmalfarbe/pandoc-latex-template/releases/download/v3.2.1/Eisvogel-3.2.1.tar.gz
tar -xzf Eisvogel-3.2.1.tar.gz
mkdir -p $HOME/.local/share/pandoc/templates
mv Eisvogel-3.2.1/eisvogel.latex $HOME/.local/share/pandoc/templates/
```

where `$HOME` is your home directory.

These packages will be detected during `cmake` and built as part of the main build.
