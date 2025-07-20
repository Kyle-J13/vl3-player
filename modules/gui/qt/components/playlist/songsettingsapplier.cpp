/*****************************************************************************
 * songsettingsapplier.cpp
 *
 * Implements SongSettingsApplier: attaches to MediaPlayerPlaying, reads
 * per-URI volume/EQ from QSettings, and calls libvlc_audio_set_volume()
 * and libvlc_media_player_set_equalizer() to apply them.
 *****************************************************************************/

#include "songsettingsapplier.hpp"
#include <vlc/vlc.h>
#include <QSettings>
#include <QUrl>

SongSettingsApplier::SongSettingsApplier(libvlc_media_player_t* mp)
  : m_mediaPlayer(mp)
{
    // Attach to the “playing” event so apply() runs when playback starts
    libvlc_event_manager_t* em = libvlc_media_player_event_manager(mp);
    libvlc_event_attach(em,
                        libvlc_MediaPlayerPlaying,
                        &SongSettingsApplier::onMediaPlaying,
                        this);
}

void SongSettingsApplier::onMediaPlaying(const libvlc_event_t* /*ev*/, void* data)
{
    static_cast<SongSettingsApplier*>(data)->apply();
}

void SongSettingsApplier::apply()
{
    // TODO
    // Retrieve current media’s MRL via libvlc_media_get_mrl()
    // Load volume % and EQ bands from QSettings under "SongSettings"/<uri>
    // Call libvlc_audio_set_volume(m_mediaPlayer, volume)
    // Create libvlc_audio_equalizer_t*, set amps, then
    //     libvlc_media_player_set_equalizer(m_mediaPlayer, eq)
    //     and release eq
}
