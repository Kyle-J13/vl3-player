--[[
    per_song_settings.lua

    A VLC extension that automatically applies per-song volume and EQ
    settings from a JSON file each time a track starts playing.

    When you open it under View -> Per-Song Settings, it hooks into
    VLC’s menu and input events so you can trigger it manually or have
    it run automatically whenever playback begins.

    It looks for a file called per_song_volumes.json in your VLC user
    data directory.  If that file exists, it decodes it into a Lua table
    mapping each song URI to its saved volume and EQ settings.  When
    a matching entry is found, it adjusts VLC’s volume and equalizer
    to match.
--]]

-- Describe the extension and its capabilities
function descriptor()
    return {
        title        = "Per-Song Settings",
        version      = "0.1",
        capabilities = { "menu", "input-listener" }  -- adds a menu item + listens for input changes
    }
end

-- Populate the “View ->” menu with a single entry
function menu()
    return { "Apply per-song settings now" }
end

-- Called when you click the menu item
function activate(name)
    if name == "Apply per-song settings now" then
        vlc.msg.info("Per-Song: manual apply")
        onPlaying()
    end
end

-- Automatically called whenever the input changes (new file, stop, etc)
function input_changed()
    if vlc.input.is_playing() == 1 then
        vlc.msg.dbg("Per-Song: input changed -> playing")
        onPlaying()
    end
end

-- Main logic: load JSON, find settings for the current URI, then set volume and EQ
function onPlaying()
    local item = vlc.input.item()
    if not item then
        return  -- no media to act on
    end

    local uri = item:uri()
    vlc.msg.dbg("Per-Song: URI = " .. uri)

    local path = vlc.config.userdatadir() .. "/per_song_volumes.json"
    local file = io.open(path, "r")
    if not file then
        vlc.msg.err("Per-Song: JSON file not found at " .. path)
        return
    end

    local content = file:read("*a")
    file:close()

    local ok, data = pcall(vlc.json.decode, content)
    if not ok or type(data) ~= "table" then
        vlc.msg.err("Per-Song: JSON decode failed")
        return
    end

    local settings = data[uri]
    if type(settings) ~= "table" then
        vlc.msg.info("Per-Song: no settings for this track")
        return
    end

    -- Apply saved volume
    if settings.volume then
        vlc.msg.info("Per-Song: setting volume to " .. settings.volume)
        vlc.volume.set(settings.volume)
    end

    -- Apply saved EQ bands if available
    if vlc.equalizer and settings["Preamp (dB)"] then
        vlc.msg.info("Per-Song: applying EQ")
        local eq = vlc.equalizer.new()
        vlc.equalizer.set_preamp(eq,     settings["Preamp (dB)"])
        vlc.equalizer.set_amp(eq,        settings["Bass (dB)"],   0)
        vlc.equalizer.set_amp(eq,        settings["Mid (dB)"],    4)
        vlc.equalizer.set_amp(eq,        settings["Treble (dB)"], 8)
        vlc.equalizer.apply(eq)
        vlc.equalizer.release(eq)
    end
end

-- Clean up when the extension is closed
function deactivate()
    
end

-- Alias for cleanup
function close()
    deactivate()
end
