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
libpepper_dali_la_LIBADD = @DALI_LIBS@ @WAYLAND_LIBS@ @PEPPER_LIBS@
libpepper_dali_la_LDFLAGS = ${LDFLAGS}
libpepper_dali_la_CXXFLAGS = \
    ${CFLAGS} \
    $(DALICORE_CFLAGS) \
    @EFL_CFLAGS@ \
    @WAYLAND_CFLAGS@ \
    @PEPPER_CFLAGS@ \
    $(pepper_dali_includes) \
    -DDEBUG_ENABLED

libpepper_dali_la_SOURCES = $(src_files)

libpepperdalidir = $(includedir)/pepper-dali
libpepperdali_HEADERS = $(public_api_header_files)

libpepperdalicompositordir = $(includedir)/pepper-dali/public-api/compositor
libpepperdalicompositor_HEADERS = $(public_api_compositor_header_files)
