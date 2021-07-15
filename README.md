![](icon_dark.png)

# VirtualHereFree Installer for Steam Link

VirtualHere is a proprietary version of the open source USB/IP project. It allows you to "stream" USB devices to another
machine to have it behave like it would be directly connected to the client.

The Steam Link comes with an integrated version of VirtualHere.
Since several versions, the integrated version requires you to buy and use the Steam store version of VirtualHere.

There is also a ["free" version of VirtualHere available](https://www.virtualhere.com/usb_client_software), which allows one to share one device for free.
The free client doesn't work with the Steam Link integrated version.
Fortunately, there are ARM binaries of the VirtualHere server available which are compatible with the Steam Link.

The VirtualHereFree Installer downloads the free version of the VirtualHere server to your Steam Link and manages the starting and
stopping of the server automatically, enabling you to use the free VirtualHere client binaries on your host machine.

## Installation

### On the Steam Link (server)

Download the latest version of the VirtualHereFree Installer from the [releases page](https://github.com/milaq/steamlink_VirtualHereFreeInstaller/releases) in tgz format.

 * Put 'VirtualHereFreeInstaller.tgz' into the folder `steamlink/apps/` on a FAT32 formatted USB stick.
 * Attach the USB stick to a free USB slot in your Steam Link
 * Reboot your Steam Link
 * Select the installer from the main menu

When you start streaming, the VirtualHere server gets started automatically and is closed when you stop streaming.
Once installed, you can delete the installer application if you please.

To update to a newer Virtualhere server version just run the installer again.

### On your Desktop PC (client)

 * Download and install the latest VirtualHere free client for your OS: https://virtualhere.com/usb_client_software
 * Run the VirtualHere client. It should autodiscover the server when yoou are streaming.

## Additional configuration

You can customize several server settings via the server configuration file by editing `/mnt/config/system/virtualherefree_config.ini` directly or by putting a copy of the [virtualherefree_config.ini](res/virtualherefree_config.ini) onto a FAT32 formatted USB stick in the directory `steamlink/config/system/`.

For all documented options see: https://virtualhere.com/configuration_faq

## Building

Clone the [SteamLink SDK](https://github.com/ValveSoftware/steamlink-sdk) and copy this repository to the `examples` subdirectory (or any other subdirectory with a depth of one).
Then from within that directory, execute `build_steamlink.sh`.
