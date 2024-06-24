Name:          pgmoneta_ext
Version:       0.1.0
Release:       1%{dist}
Summary:       An extension to enhance the functionality of the main pgmoneta project.
License:       BSD
URL:           https://github.com/pgmoneta/pgmoneta_ext
Source0:       https://github.com/pgmoneta/pgmoneta_ext/archive/%{version}.tar.gz

BuildRequires: git gcc cmake make postgresql-server postgresql-server-devel
Requires:      postgresql-server

%description
pgmoneta_ext is an extension to enhance the functionality of the main pgmoneta project.

%prep
%setup -q

%build

%{__mkdir} build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
%{__make}

%install

cd build
make DESTDIR=%{buildroot} install

%{__mkdir} -p %{buildroot}%{_docdir}/%{name}

%{__install} -m 644 %{_builddir}/%{name}-%{version}/LICENSE %{buildroot}%{_docdir}/%{name}/LICENSE
%{__install} -m 644 %{_builddir}/%{name}-%{version}/CODE_OF_CONDUCT.md %{buildroot}%{_docdir}/%{name}/CODE_OF_CONDUCT.md
%{__install} -m 644 %{_builddir}/%{name}-%{version}/README.md %{buildroot}%{_docdir}/%{name}/README.md
%{__install} -m 644 %{_builddir}/%{name}-%{version}/doc/DEVELOPERS.md %{buildroot}%{_docdir}/%{name}/DEVELOPERS.md
%{__install} -m 644 %{_builddir}/%{name}-%{version}/doc/PR_GUIDE.md %{buildroot}%{_docdir}/%{name}/PR_GUIDE.md
%{__install} -m 644 %{_builddir}/%{name}-%{version}/doc/RPM.md %{buildroot}%{_docdir}/%{name}/RPM.md
%{__install} -m 644 %{_builddir}/%{name}-%{version}/doc/TEST.md %{buildroot}%{_docdir}/%{name}/TEST.md

%files
%license %{_docdir}/%{name}/LICENSE
%{_docdir}/%{name}/CODE_OF_CONDUCT.md
%{_docdir}/%{name}/DEVELOPERS.md
%{_docdir}/%{name}/PR_GUIDE.md
%{_docdir}/%{name}/README.md
%{_docdir}/%{name}/RPM.md
%{_docdir}/%{name}/TEST.md

%{_libdir}/pgsql/pgmoneta_ext.so
%{_libdir}/pgsql/pgmoneta_ext.so.0*

%{_datadir}/pgsql/extension/pgmoneta_ext--*.sql
%{_datadir}/pgsql/extension/pgmoneta_ext.control


%changelog
