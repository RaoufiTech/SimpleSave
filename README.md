# ✨ SimpleSave ✨

**Auto-Save Clipboard Images to Disk Effortlessly**

EasySave is a lightweight and efficient program designed to seamlessly save images copied to your clipboard directly to your disk. With its user-friendly approach, it works in the background, ensuring that every image you copy is safely stored without interruptions.
Note: Still in development, I will add more functions later on.
---

## 🌟 Features

- 📋 **Automatic Clipboard Monitoring**  
  Continuously monitors the clipboard for images and saves them as PNG files.

- 🔒 **Unique Image Detection**  
  Employs a hashing mechanism to avoid duplicate saves, ensuring only unique images are stored.

- 🗂️ **Organized Storage**  
  Saves images in a timestamped format within a dedicated folder under your Downloads directory.

- 🚀 **Startup Integration**  
  Automatically starts with Windows, so it’s always ready to save your clipboard images.

- ⌨️ **Hotkey for Quick Shutdown**  
  Use `CTRL + ALT + S` to stop the application instantly, giving you complete control.

---

## 💡 How It Works

1. **Clipboard Monitoring**  
   EasySave monitors your clipboard for bitmap images (`CF_BITMAP` format). When an image is detected, it calculates a hash to check for uniqueness.

2. **Saving Images**  
   Unique images are saved in the `ClipBoardImages` folder under your Downloads directory, using the current timestamp as the filename.

3. **Startup Behavior**  
   The program adds itself to the Windows startup registry for convenience.

4. **Hotkey Listening**  
   Registers a global hotkey (`CTRL + ALT + S`) for manual shutdown when needed.

---

## 🛠️ Usage

1. **Run the Program**  
   Launch `SimpleSave.exe`. It will begin monitoring your clipboard immediately.

2. **Copy an Image**  
   Copy any image (e.g., from a browser, screenshot tool, or design software).

3. **Check the Downloads Folder**  
   Navigate to the `ClipBoardImages` folder inside your Downloads directory to find the saved images.

4. **Shutdown with Hotkey**  
   Press `CTRL + ALT + S` to stop the program at any time.

---

## 🖥️ System Requirements

- **Operating System**: Windows 7 or higher
- **Dependencies**: GDI+ (usually available on all Windows versions)
- **Build Requirements**:  
  - Link against `gdiplus.lib` (if not using the included `#pragma` directive)

---
## 📥 Download
the latest release from the [Releases](https://github.com/RaoufiTech/SimpleSave/releases) Page.

## 📦 Build Instructions

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/RaoufiTech/SimpleSave.git
   cd EasySave
   ```

2. **Build with Visual Studio**:
   - Open the project in Visual Studio.
   - Add `gdiplus.lib` to the linker settings if the `#pragma` is commented out.
   - Build and run.



---

## 🤔 Why SimpleSave?

EasySave is perfect for:
- 🎨 Designers who frequently copy images and want automatic backups.
- 👨‍💻 Developers and testers working with screenshots or clipboard data.
- 🧑‍💼 Anyone looking for a simple, no-configuration tool to save their clipboard images automatically.

---

## 🤝 Contribute

We welcome contributions! Feel free to submit issues or pull requests for improvements or new features.

---

## 📜 License

This project is licensed under the **MIT License**. See the [LICENSE](https://github.com/RaoufiTech/SimpleSave/blob/master/LICENSE) file for details.

---
