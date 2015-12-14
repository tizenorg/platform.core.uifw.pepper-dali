Name:       pepper-dali
Version:    0.0.1
Release:    0
Summary:    DALi backend for pepper
License:    MIT
Group:      Graphics & UI Framework/Wayland Window System

Source:     %{name}-%{version}.tar.xz

BuildRequires: pkgconfig(dali-core)
BuildRequires: pkgconfig(dali)
BuildRequires: pkgconfig(dali-toolkit)
BuildRequires: pkgconfig(wayland-server)
BuildRequires: pkgconfig(pepper)
BuildRequires: pkgconfig(xdg-shell-server)
BuildRequires: pkgconfig(xkbcommon)
BuildRequires: pkgconfig(tizen-extension-client)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(ecore-input)
BuildRequires: pkgconfig(ecore-wayland)
Requires: libwayland-extension-server

%description
DALi backend for pepper.
Pepper is a lightweight and flexible library for developing various types of wayland compositors.

##############################
# devel
##############################
%package devel
Summary:    Development components for pepper-dali
Group:      Development/Building
Requires:   %{name} = %{version}-%{release}

%description devel
This package includes developer files common to all packages.

##############################
# Preparation
##############################
%prep
%setup -q
                      
##############################
# Build
##############################
%build
%autogen
make %{?_smp_mflags}

##############################
# Installation
##############################
%install
%make_install

##############################
# Files in Binary Packages
##############################
%files -n %{name}
%manifest pepper-dali.manifest     
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so*

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}/
%{_includedir}/%{name}/pepper-dali.h 
%{_includedir}/%{name}/public-api/*
%{_libdir}/pkgconfig/%{name}.pc

