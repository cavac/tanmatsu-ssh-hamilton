# Tanmatsu ssh client

A standalone SSH terminal client for the [Tanmatsu](https://nicolaielectronics.nl/tanmatsu/)
badge, built on ESP-IDF.

Connect to remote servers over SSH directly from your badge with a full
interactive terminal session, host key verification, and persistent connection
profiles.

This is an `ssh` client for the Tanmatsu / Konsool device, using the
[ESP-IDF component wrapper for libssh2](https://components.espressif.com/components/skuodi/libssh2_esp/)
and the [Badge.Team](https://badge.team) software stack. If you don't know what
any of this means, don't worry, but this repo probably isn't for you. It was
originally a hacked version of the Tanmatsu launcher, but is now a standalone
app in its own right.

# Features

- Multiple saved SSH connection profiles (host, port, username, password)
- Interactive terminal emulator with ANSI escape sequence support
- Host key fingerprint verification (SHA256) with NVS-based persistence
- Man-in-the-middle attack detection (warns on host key changes)
- Keyboard backlight and display brightness controls during sessions
- Optional background images from SD card (`/sd/bg/`)
- xterm-256color terminal emulation

## Antifeatures

This app is all about interactive remote login, but you can do other stuff with
`ssh` like `scp` and `sftp` file transfer and batch mode remote command
execution. This app isn't doing that stuff, at least for now. It would probably
be better to have dedicated apps for these other functions. And some sort of
shell / REPL for that matter...

## Big Scary Warning

This software should be treated as a minimally functioning proof-of-concept.
Right now it lacks some key features that you would want in order to be
comfortable using it for real work or anything important etc - see TODO list
below. Even once the app is feature complete, you should still think very
carefully about which systems and accounts you use with it, just like any other
software you might trust with sensitive data like passwords and encryption
keys.

In particular, be aware that **your ssh connection settings are stored in the
Tanmatsu configuration, including any passwords that you choose to save**. All
the code you run on your Tanmatsu (_like apps written by other people_) will
have access to this info. In the future we will probably encrypt these details
and prompt you to unlock them as part of starting up the SSH app, but right now
we don't.

OK, that's enough doom and gloom. On with the show...

# Installation

Right now you have to build it from source code - see below for further info.
When it has evolved a bit more it will be submitted to the Tanmatsu app
repository.

# Usage

## SSH Connection Menu

| Key | Action |
|-----|--------|
| F1 / ESC | Return to launcher |
| F2 | Toggle keyboard backlight |
| F3 | Add new connection |
| F4 | Edit selected connection |
| F5 | Delete selected connection |
| Enter | Connect to selected server |

## During SSH Session

| Key | Action |
|-----|--------|
| F1 | Disconnect and return to menu |
| F2 | Toggle keyboard backlight |
| F3 | Cycle display brightness |
| F5 | Change text colour |
| F6 | Change background colour |
| Arrow keys | Cursor movement |
| Tab | Tab completion |
| Ctrl+key | Send control characters |
| Volume +/- | Adjust font size |

After installing the app, you should find that you have an extra entry in your
Apps directory called `SSH`. When you launch it, you'll be prompted with a list
of the `ssh` servers that the app knows about - initially this will be empty,
but there is a GUI that should let you add server details.

When you have at least one server configured, you can press `ENTER` to start an
`ssh` connection to the selected server. This spawns a full screen terminal
emulation and manages the `ssh` session.

During the `ssh` session there are some useful things you can do with the
Tanmatsu function keys:

- **Red X** - close the session, right now we do this without asking you if you're sure, but that will probably become a setting
- **Orange Triangle** - adjust the keyboard backlight, keep pressing until you get the brightness level you want
- **Yellow Square** - adjust the screen backlight, keep pressing until you get the brightness level you want
- **Blue Recycling** - change the background colour (is it a recycling sign? maybe!)
- **Purple Diamond** - pick a random background colour
- **Vol +/-** - make the terminal font larger (+) or smaller (-)

# Building

## Requirements

- [Tanmatsu](https://tanmatsu.cloud/) badge (ESP32-P4 based)
- WiFi network with access to SSH servers
- ESP-IDF v5.5.1 (included as submodule)

If you already have the ESP-IDF tools installed, you can avoid re-installing
them by sourcing a file which has your environment variables in, e.g.

```bash
export DEVICE="tanmatsu"
export TARGET="esp32p4"
export IDF_PATH=$HOME/.esp/esp-idf
export IDF_TOOLS_PATH=$HOME/.esp/esp-idf-tools

echo "ESP-IDF: $IDF_PATH"
echo "TOOLS:   $IDF_TOOLS_PATH"
echo "DEVICE:  $DEVICE"
echo "TARGET:  $TARGET"

source $IDF_PATH/export.sh
```

## Building

If you are installing a copy of ESP-IDF for the build:

```bash
git clone --recursive <repo-url>
cd tanmatsu-ssh
make prepare
make
```

If you already have ESP-IDF installed:

```bash
git clone <repo-url>
cd tanmatsu-ssh
make
```

## Installing

The output binary is `build/tanmatsu/tanmatsu-ssh.bin`.

You can use the Badge Team `badgelink` tool to install it, e.g.

```bash
badgelink.py appfs upload tanmatsu-ssh-client "SSH" 1 build/tanmatsu/tanmatsu-ssh.bin
```

However, `badgelink` isn't installed by default as part of the build - you will
get a copy when you build some of the other Tanmatsu apps, like the
`tanmatsu-launcher` app.

## SSH session configuration and testing

It's a bit of a pain typing in system connection details using the Tanmatsu
keyboard, but you can always use `badgelink` and save yourself some trouble:

```bash
badgelink.py nvs write ssh s00.conn_name str test-server
badgelink.py nvs write ssh s00.dest_host str 10.0.0.1
badgelink.py nvs write ssh s00.dest_port str 2025
badgelink.py nvs write ssh s00.username str tanmatsu-test
badgelink.py nvs write ssh s00.password str lol-jk-etc
```

(however new entries that are added entirely via `badgelink` don't seem to show
up - so you might find you need to add a new connection using the default
values and then change the details via `badgelink`)

You don't have to use your real credentials, ssh server etc. Maybe consider
running an `sshd` on a non-standard port, on a sacrificial VM, creating a test
user rather than using your real credentials. Something like this can be handy:

```
sshd -f sshd_config -p 2025
```

One thing that might be useful if you are out and about - you can test on
Android by installing `openssh` under Termux, setting a password for the Termux
user, creating a wifi hotspot for your Tanmatsu to connect to, and running
`sshd` as per above.

## Dependencies

Managed components (automatically downloaded):

| Component | Purpose |
|-----------|---------|
| `badgeteam/badge-bsp` | Board support package |
| `nicolaielectronics/tanmatsu-wifi` | WiFi via radio coprocessor |
| `nicolaielectronics/wifi-manager` | WiFi network management |
| `skuodi/libssh2_esp` | SSH client library (libssh2) |
| `robotman2412/pax-gfx` | Graphics library |
| `robotman2412/pax-codecs` | PNG decoder |

We also use the `badgeteam/terminal-emulator` terminal emulator engine, but our version has some changes that haven't been upstreamed yet.

## Code contributions

If you make any fixes or improvements, please do raise a PR. Let's make this a
really great app together!

Also the vibe is very much "good vibes" more than vibe coding, so don't be too
despondent if your epic 10,000 line LLM generated patch gets rejected ;-)

## TODO

- [ ] add support for other modifiers where needed, e.g. ALT, FN
- [ ] prompt user for password if not saved
- [ ] fix disconnection/exit bug which makes the launcher blue screen ~50% of the time on restarting
- [ ] fix whatever is preventing connections added entirely via badgelink from showing up
- [ ] tidy up spaghetti code and global variables
- [ ] check if any changes needed for IPv6 support
- [ ] check if any changes needed for DNS lookup of hostnames
- [ ] display server banner?
- [ ] encrypt saved credentials with a passphrase / prompt user to unlock
- [ ] UI for managing cached host keys
- [ ] support public key auth
- [ ] support agent auth
- [ ] let user set terminal type?
- [ ] test with TERM xterm-color etc
- [ ] convert background image loading into a task, so it doesn't hold everything else up
- [ ] see if we can find a way to stop background image from scrolling
- [ ] function key switches between background images?
- [ ] ask if they really want to close the connection when they hit F1
- [ ] improve escape character processing so we can use vi, emacs, fancy prompts etc
- [ ] ability to choose wifi network, or maybe tie wifi network to ssh connection details
- [ ] other cursor styles e.g. block, blinking block, blinking line?

# Credits

Renze Nicolai, Badge.Team and the other `tanmatsu-launcher` contributors. This
code started off as a fork of the launcher.

Daniel Stenberg and the other `libssh2` contributors, and skuodi for packaging
`libssh2` as an ESP-IDF component.

All bugs introduced by Martin Hamilton and zh4ck :-)

# License

This project is made available under the terms of the [MIT license](LICENSE).

