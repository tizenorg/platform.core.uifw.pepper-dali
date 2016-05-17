internal_src_dir = internal
include internal/file.list

public_api_src_dir = public-api
include public-api/file.list

src_files = $(internal_src_files) \
            $(public_api_src_files)

pepper_dali_includes = \
        -I.

libpepper_dali_la_LTLIBRARIES = libpepper-dali.la
libpepper_dali_ladir = $(libdir)
libpepper_dali_la_LIBADD = @DALI_LIBS@ @WAYLAND_LIBS@ @PEPPER_LIBS@ @WAYLAND_TBM_LIBS@
libpepper_dali_la_LDFLAGS = ${LDFLAGS}
libpepper_dali_la_CXXFLAGS = \
    ${CFLAGS} \
    $(DALI_CFLAGS) \
    @EFL_CFLAGS@ \
    @WAYLAND_CFLAGS@ \
    @PEPPER_CFLAGS@ \
    @WAYLAND_TBM_LIBS@ \
    $(pepper_dali_includes)

libpepper_dali_la_SOURCES = $(src_files)

libpepperdalidir = $(includedir)/pepper-dali
libpepperdali_HEADERS = $(public_api_header_files)

libpepperdalicompositordir = $(includedir)/pepper-dali/public-api/compositor
libpepperdalicompositor_HEADERS = $(public_api_compositor_header_files)

libpepperdaliobjectviewdir = $(includedir)/pepper-dali/public-api/object-view
libpepperdaliobjectview_HEADERS = $(public_api_object_view_header_files)

