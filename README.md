# Rainbow Six Vegas 2 - Server & Mod Manager

A launcher and mod manager for Rainbow Six Vegas 2, supporting both the dedicated server (SADS) and standard game. Fork of [ReaverTeknoGods/TeknoR6Vegas2](https://github.com/ReaverTeknoGods/TeknoR6Vegas2).

------

## Installation

1. Download the latest `.exe` from [Releases](https://github.com/nice-rice/Rainbow-Six-Vegas-2-Server-Mod/releases).
2. Place it in your `Rainbow Six Vegas 2/Binaries/` folder.
3. Run the program from there.

On first run, the tool backs up your game config files automatically. These backups are not overwritten on subsequent launches.

------

## Usage

### Host Options
Configure these if you are running or joining a server:

- **Run SADS** > check this to launch the dedicated server instead of the normal game
- **Server Name / Password** > set your server's name and optional password
- **Game Mode** > choose from Terrorist Hunt or PvP modes (Will always appear under Versus even if Terrorist Hunt is chosen)
- **Difficulty** > Easy, Normal, or Realistic
- **Round Time Limit** > in minutes, cannot be set to unlimited
- **Respawn Count** > None, 1, 2, 3, or Unlimited

### Other Options
- **Max Players** > override the player cap (up to 16)
- **Map** > select from any installed maps
- **Ready Up Required** > players must ready up before each round starts

### Terrorist Hunt
- **Spawn Rate** > how many terrorists respawn (other players will need to modify their config to match)
- **Terrorist Count** > override the default number of terrorists

### Other Mods
Toggle these to apply additional tweaks via the game's config files:

- **Fast Internet** > raises network rate limits
- **Sound Patch** > increases the audio channel limit
- **Improved Graphics** > applies engine-level graphics improvements
- **Extra Ammo** > increases default ammunition
- **Extra Gadgets** > increases number of grenades to 9

Hit **Apply** to just save settings, or **Launch** to apply and start the game.  
Settings are remembered between sessions.

---

## Known Issues

- Map cannot be changed without restarting the server

---

## License

GPLv3 > see [LICENSE.md](LICENSE.md)
