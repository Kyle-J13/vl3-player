/*************************************************************************
 * songsettingsapplier.hpp
 *
 * Declares SongSettingsApplier which hooks into VLC’s media player events
 * to load per-song volume/EQ settings from QSettings and apply them at
 * playback start.
 *************************************************************************/

#pragma once

#include <vlc/vlc.h>

// Forward declaration to avoid pulling in full Qt headers
class QSettings;

/**
 * @class SongSettingsApplier
 * @brief Listens for media-player “playing” events and applies saved
 *        per-URI volume & EQ settings.
 */
class SongSettingsApplier
{
public:
    /**
     * @param mp  The libvlc_media_player_t instance to which we attach.
     */
    explicit SongSettingsApplier(libvlc_media_player_t* mp);

    /**  Static callback for libVLC MediaPlayerPlaying event  */
    static void onMediaPlaying(const libvlc_event_t* /*ev*/, void* data);

private:
    /**  Perform load of QSettings and call VLC audio/EQ APIs  */
    void apply();

    libvlc_media_player_t* m_mediaPlayer;  ///< Underlying VLC player instance
};
