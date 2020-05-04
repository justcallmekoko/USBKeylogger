# Masterkey - USB WiFi Keylogger
*A WiFi enabled USB Keylogger and Keystroke injection tool*
 
<p align="center">
  <a href="https://github.com/justcallmekoko/USBKeylogger/blob/master/LICENSE"><img alt="License" src="https://img.shields.io/github/license/mashape/apistatus.svg"></a>
  <a href="https://github.com/justcallmekoko/USBKeylogger/releases/latest"><img src="https://img.shields.io/github/downloads/justcallmekoko/USBKeylogger/total" alt="Downloads"/></a>
  <br>
  <a href="https://twitter.com/intent/follow?screen_name=jcmkyoutube"><img src="https://img.shields.io/twitter/follow/jcmkyoutube?style=social&logo=twitter" alt="Twitter"></a>
  <a href="https://www.instagram.com/just.call.me.koko"><img src="https://img.shields.io/badge/Follow%20Me-Instagram-orange" alt="Instagram"/></a>
  <br><br>
  <a href="https://www.tindie.com/products/justcallmekoko/masterkey-wifi-usb-keylogger/"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-larges.png" alt="I sell on Tindie" width="200" height="104"></a>
</p>

# Table of Contentiousness
- [About](#about)
- [Usage](#usage)
  - [Key Presses](#key-presses)
  - [Keystroke Injection](#keystroke-injection)
  - [Admin Settings](#admin-settings)
- [Updating Firmware](#updating-firmware)
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
## Admin Settings

# Updating Firmware
## Updating ATMEGA32U4
## Updating ESP8266
