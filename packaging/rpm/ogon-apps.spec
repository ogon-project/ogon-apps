#
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

%define          libmessage_major 1
Name:            ogon-apps
Version:         1.0.0
Release:         1
Summary:         Application and utilities for ogon
Group:           Productivity/Networking/RDP
License:         Apache-2.0
Url:             http://ogon-project.com
Source:          ogon-apps.tar.xz
BuildRequires:   pkg-config
BuildRequires:   cmake >= 2.8.12
BuildRequires:   freerdp2-devel
BuildRequires:   ogon-devel
BuildRequires:   gcc-c++
BuildRequires:	 libqt5-qtbase-devel
BuildRequires:   libqt5-linguist-devel
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
AutoReqProv:    on

%description
The ogon Remote Desktop Services provide graphical remote access to
desktop sessions and applications using the Remote Desktop Protocol
(RDP) and supports most modern RDP protocol extensions, bitmap
compression codecs and device redirections. ogon is build on the
FreeRDP library and is compatible with virtually any existing Remote
Desktop Client.

This package contains miscellaneous tools and applications.

%package -n libogon-message%{libmessage_major}
Summary: The ogon gui message library 
Group:   Productivity/Utilities
%description -n libogon-message%{libmessage_major}
The libogon-message library can be used for displaying simple messages and
notifications. It was designed to be used with ogon.

This package contains the libogon-message library.

%package -n libogon-message%{libmessage_major}-devel
Summary: Development files for the ogon message library
Group:   Development/Productivity/Utilities
Requires: libogon-message%{libmessage_major} = %version
%description -n libogon-message%{libmessage_major}-devel 
The libogon-message library can be used for displaying simple messages and
notifications. It was designed to be used with ogon.

This package contains development files for libogon-message.

%package -n ogon-message
Summary: A simple command line utility for displaying messages
Group:   Productivity/Utilities
%description -n ogon-message
The ogon-message command line utility is a xmessage "replacement" using
libogon-message. It was desinged to be used with ogon.
%prep
%setup

%build
%cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_SKIP_RPATH=FALSE -DCMAKE_SKIP_INSTALL_RPATH=TRUE
make VERBOSE=1 %{?jobs:-j%jobs}

%install
#export NO_BRP_STRIP_DEBUG=true
#export NO_DEBUGINFO_STRIP_DEBUG=true
#%%define __debug_install_post %{nil}
%cmake_install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%_bindir/ogon-shadow

%files -n libogon-message%{libmessage_major}
%defattr(-,root,root)
%_libdir/libogon-message.so.*

%files -n libogon-message%{libmessage_major}-devel
%defattr(-,root,root)
%_libdir/libogon-message.so
%_includedir/ogon-message%{libmessage_major}
%_libdir/cmake/ogon-message1
%_libdir/pkgconfig/ogon-message1.pc

%files -n ogon-message
%defattr(-,root,root)
%_bindir/ogon-message
%dir %{_datarootdir}/qt5
%dir %{_datarootdir}/qt5/translations/
%{_datarootdir}/qt5/translations/*.qm

%post -n libogon-message%{libmessage_major} -p /sbin/ldconfig
%postun -n libogon-message%{libmessage_major} -p /sbin/ldconfig


%changelog
* Tue Jun 07 2016 - bernhard.miklautz@thincast.com
- Initial version
