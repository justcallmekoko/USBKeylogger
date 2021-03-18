# Masterkey - USB WiFi Keylogger
<p align="center"><img alt="Keylogger" src="https://github.com/justcallmekoko/USBKeylogger/blob/master/images/keylogger.jpg?raw=true" width="500"></p>
<p align="center">
  <i>A WiFi enabled USB Keylogger and Keystroke injection tool</i>
  <br><br>
  <a href="https://github.com/justcallmekoko/USBKeylogger/blob/master/LICENSE"><img alt="License" src="https://img.shields.io/github/license/mashape/apistatus.svg"></a>
  <a href="https://github.com/justcallmekoko/USBKeylogger/releases/latest"><img src="https://img.shields.io/github/downloads/justcallmekoko/USBKeylogger/total" alt="Downloads"/></a>
  <br>
  <a href="https://twitter.com/intent/follow?screen_name=jcmkyoutube"><img src="https://img.shields.io/twitter/follow/jcmkyoutube?style=social&logo=twitter" alt="Twitter"></a>
  <a href="https://www.instagram.com/just.call.me.koko"><img src="https://img.shields.io/badge/Follow%20Me-Instagram-orange" alt="Instagram"/></a>
  <br><br>
  <a href="https://www.tindie.com/products/justcallmekoko/masterkey-wifi-usb-keylogger/"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-larges.png" alt="I sell on Tindie" width="200" height="104"></a>
</p>

Catch my live stream on [Twitch](https://www.twitch.tv/willstunforfood) where I play games, have just chatting sessions to talk tech, and just generally be an ordinary person who can't figure out how to make proper life decisions.
  
  Stream Schedule:  
    Mon: 1800 - 2000 EST  
    Wed: 1800 - 2000 EST  
    Sun: 1200 - 1400 EST  

# Table of Contentiousness
- [About](#about)
- [Usage](#usage)
  - [Key Presses](#key-presses)
  - [Keystroke Injection](#keystroke-injection)
  - [Admin Settings](#admin-settings)
- [Updating Firmware](#updating-firmware)
  - [Bootloader](#bootloader)
  - [Updating ATMEGA32U4](#updating-atmega32u4)
  - [Updating ESP8266](#updating-esp8266)

# About
The USB WiFi Keylogger is a hardware hacking tool which is capable of sniffing key presses sent from a USB keyboard to a computer and presenting them to the attacker over WiFi. The keylogger runs a modified version of [@spacehuhn's wifi keylogger code](https://github.com/spacehuhn/wifi_keylogger) with a few more tricks and features. For more information about MAX3421EE based USB Keyloggers, visit my [Hardware Keyloggers](https://github.com/justcallmekoko/Hardware-Keyloggers#spacehuhn-wifi-keylogger) repo.

# Usage
The Masterkey USB WiFi Keylogger is intended to be a plug and play device. Simply plug the target USB Keyboard into the female USB port of the keylogger then plug the USB Keylogger into the target computer. After about five seconds, any keys presses on the keyboard will be logged by the keylogger and passed through to the target computer. 

## Key Presses
Key presses are stored in the file system of the ESP8266 which can be accessed over WiFi via the device web interface. To access the web interface follow these steps. This is assuming the keylogger is powered on and you are within range of its WiFi access point.
1. Connect to the `Masterkey` WiFi access point using the default password `masterkey`
2. Navigate to `http://192.168.4.1` in your web browser

Logged key presses will be displayed within the text box on that page. At the bottom of the page is a `clear` button. This button will clear the keylog in the event the user wants to start a fresh log for any reason.

## Keystroke Injection
Keystroke injection attacks can me executed by navigating to the `Live Execute` tab at the top of the screen. There you can write, run, and save keystroke injection scripts written in [ducky script](https://docs.hak5.org/hc/en-us/articles/360010555153-Ducky-Script-the-USB-Rubber-Ducky-language).  
If you want to run a script you have written and saved previously, you can navigate to the `Scripts` tab and select any script from the list of scripts displayed on the screen. You can either click on the script directly to edit, rename, or run the script or you can just click the run button to immediately run the script.  
You can also upload prewritten scripts to be executed on Masterkey.

## Admin Settings
These settings are responsible for securing the wireless access point of Masterkey and tailoring the appearance to your liking. These settings can be found by navigating to the `Settings` tab. Ther you will be able to adjust the following settings:
1. Access Point SSID: The name of the access point that will appear when searching through WiFi networks
2. Access Point Password: The password that will be required when connecting to the Masterkey access point
3. Channel: The channel the access point will be broadcasted on
4. Hidden: Boolean which will tell Masterkey whether or not to broadcast its access point

Once the settings have been changed, you will be required to hit `save` and `restart` for the changes to take effect.

# Updating Firmware
As new firmware is released for the Masterkey, it will be left to the discretion of the user whether or not they would like to update their device. There are two phases to updating Masterkey described here. You may be required to update either the ATMEGA32U4 or the ESP8266. It is more likely the ESP8266 will be updated since most of the functionality resides there including the user interface.

## Bootloader
If you know what you're doing, you will know how to use this.  
'avrdude -c usbasp -p m32u4 -b 115200 -v -e -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0xCB:m -U lock:w:0x2F:m -U flash:w:Caterina-A-Star.hex'

## Updating ATMEGA32U4
To update the ATMEGA32U4, you will be required to clone this repository, build the project, and upload it directly to the microcontroller using the Arduino IDE. These steps will be explained here.  

_to-do_

## Updating ESP8266
Updating the ESP8266 occurs over the air using [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA). 
1. Download the [latest release](https://github.com/justcallmekoko/USBKeylogger/releases/latest)
2. Connect to the Masterkey access point
3. Access the web page via web browser at `http://192.168.4.1`
4. Navigate to the `About` tab
5. Click on the `Update` button
6. Select the bin file you downloaded from releases and click `Upload`

The ESP8266 will automatically restart once the update has been applied and you will be required to reconnect to it if you wish to continue to use Masterkey.
