# Headers
set (SOLOUD_HEADERS
	"include/soloud.h"
	"include/soloud_audiosource.h"
	"include/soloud_ay.h"
	"include/soloud_bassboostfilter.h"
	"include/soloud_biquadresonantfilter.h"
	"include/soloud_bus.h"
	"include/soloud_dcremovalfilter.h"
	"include/soloud_echofilter.h"
	"include/soloud_error.h"
	"include/soloud_fader.h"
	"include/soloud_fft.h"
	"include/soloud_fftfilter.h"
	"include/soloud_file.h"
	"include/soloud_file_hack_off.h"
	"include/soloud_file_hack_on.h"
	"include/soloud_filter.h"
	"include/soloud_flangerfilter.h"
	"include/soloud_freeverbfilter.h"
	"include/soloud_internal.h"
	"include/soloud_lofifilter.h"
	"include/soloud_misc.h"
	"include/soloud_monotone.h"
	"include/soloud_noise.h"
	"include/soloud_openmpt.h"
	"include/soloud_queue.h"
	"include/soloud_robotizefilter.h"
	"include/soloud_sfxr.h"
	"include/soloud_speech.h"
	"include/soloud_tedsid.h"
	"include/soloud_thread.h"
	"include/soloud_vic.h"
	"include/soloud_vizsn.h"
	"include/soloud_wav.h"
	"include/soloud_waveshaperfilter.h"
	"include/soloud_wavstream.h"
)

# Core
set (SOLOUD_CORE_SOURCES
	"src/core/soloud.cpp"
	"src/core/soloud_audiosource.cpp"
	"src/core/soloud_bus.cpp"
	"src/core/soloud_core_3d.cpp"
	"src/core/soloud_core_basicops.cpp"
	"src/core/soloud_core_faderops.cpp"
	"src/core/soloud_core_filterops.cpp"
	"src/core/soloud_core_getters.cpp"
	"src/core/soloud_core_setters.cpp"
	"src/core/soloud_core_voicegroup.cpp"
	"src/core/soloud_core_voiceops.cpp"
	"src/core/soloud_fader.cpp"
	"src/core/soloud_fft.cpp"
	"src/core/soloud_fft_lut.cpp"
	"src/core/soloud_file.cpp"
	"src/core/soloud_filter.cpp"
	"src/core/soloud_misc.cpp"
	"src/core/soloud_queue.cpp"
	"src/core/soloud_thread.cpp"
)

# Audiosources
set (SOLOUD_INPUT_SOURCES
	"src/audiosource/wav/dr_flac.h"
	"src/audiosource/wav/dr_impl.cpp"
	"src/audiosource/wav/dr_mp3.h"
	"src/audiosource/wav/dr_wav.h"
	"src/audiosource/wav/soloud_wav.cpp"
	"src/audiosource/wav/soloud_wavstream.cpp"
	"src/audiosource/wav/stb_vorbis.c"
	"src/audiosource/wav/stb_vorbis.h"
)

# Filters
set (SOLOUD_FILTERS_SOURCES
	"src/filter/soloud_bassboostfilter.cpp"
	"src/filter/soloud_biquadresonantfilter.cpp"
	"src/filter/soloud_dcremovalfilter.cpp"
	"src/filter/soloud_echofilter.cpp"
	"src/filter/soloud_fftfilter.cpp"
	"src/filter/soloud_flangerfilter.cpp"
	"src/filter/soloud_freeverbfilter.cpp"
	"src/filter/soloud_lofifilter.cpp"
	"src/filter/soloud_robotizefilter.cpp"
	"src/filter/soloud_waveshaperfilter.cpp"
)

# Backends
set (SOLOUD_BACKEND_SOURCES
	"src/backend/miniaudio/miniaudio.h"
	"src/backend/miniaudio/soloud_miniaudio.cpp"
)

add_library(soloud ${SOLOUD_HEADERS} ${SOLOUD_CORE_SOURCES} ${SOLOUD_INPUT_SOURCES} ${SOLOUD_FILTERS_SOURCES} ${SOLOUD_BACKEND_SOURCES})
target_compile_definitions(soloud PUBLIC "WITH_MINIAUDIO")
target_include_directories(soloud PUBLIC "include")