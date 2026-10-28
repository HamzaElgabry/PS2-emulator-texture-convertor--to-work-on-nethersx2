# PS2-emulator-texture-convertor--to-work-on-nethersx2
A modern Qt/C++ desktop application to convert PlayStation 2 textures for emulators since old formats don't work with NetherSX2

## 📌 About the Project

In 2024, PCSX2 updated how it handles texture replacement. Because NetherSX2 has fallen behind these updates, many newly released texture mods created by the community fail to load properly on mobile devices running NetherSX2. 

I built this tool to seamlessly convert new-format texture packs into the legacy format so they can be read and rendered correctly by NetherSX2.

---

## 🚀 Features

* **Convert to Old Format:** Transforms modern texture packs into the older format supported by NetherSX2.
* **Reverse Conversion:** Also includes an option to convert legacy textures into the new format, just in case newer PCSX2 builds require it.
* **Desktop Performance:** Handles massive texture packs locally on your PC, saving valuable mobile storage space and compute power.

---

## 🖼️ Preview

<img width="1505" height="936" alt="App Screenshot" src="https://github.com/user-attachments/assets/36bfa948-4e24-4ba4-a5a2-05c6efcf9959" />

---

## 🛠️ How to Use

1. Launch the application.
2. Select your texture pack directory or files.
3. Choose **Convert to Old Format**.
6. the converted folder is going to be named "orignalfoldername + 'converted'" so make sure to rename it to "textures" before moving it to the emulator directory

> [!NOTE]
> * **Check Your Settings First:** Make sure texture replacement is actually enabled in your emulator settings before troubleshooting texture formats.
> * **Use Case:** Only use this tool if you have textures in the newer format that fail to load on NetherSX2.

---

## 🙏 Credits & Acknowledgments

The core mathematical logic and conversion routines are adapted from [NetherTTC](https://github.com/DIB1998/NetherTTC) by **DIB1998**. 

* **Why rewrite it?** It all started when I wanted to play a mod featuring a massive 4.5 GB texture pack, but my phone didn't have the storage space for it. so I rewrote and implemented the logic in C++ to run locally on my desktop—giving me the free space to convert large sets easily. After building it, I decided to package it into a clean GUI and share it with the community.

