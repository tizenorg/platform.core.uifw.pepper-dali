Name:       pepper-dali
Version:    0.0.1
Release:    0
Summary:    DALi backend for pepper
License:    MIT
Group:      Graphics & UI Framework/Wayland Window System

Source:     %{name}-%{version}.tar.xz

BuildRequires: pkgconfig(wayland-server)
BuildRequires: pkgconfig(pepper)
BuildRequires: pkgconfig(dali)
BuildRequires: pkgconfig(dali-toolkit)
BuildRequires: pkgconfig(xdg-shell-server)
BuildRequires: pkgconfig(xkbcommon)
BuildRequires: pkgconfig(tizen-extension-client)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(ecore-input)
Requires: libwayland-extension-server

%description
DALi backend for pepper.
Pepper is a lightweight and flexible library for developing various types of wayland compositors.

%prep
%setup -q
                      
%build
%autogen
make %{?_smp_mflags}

%install
%make_install

%files -n %{name}
%manifest pepper-dali.manifest     
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}/
%{_includedir}/%{name}/pepper-dali.h 
%{_includedir}/%{name}/public-api/*
%{_libdir}/lib%{name}.so.*
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc

