# 🖼️ BMP Steganography Tool (C Project)

This project implements a simple **steganography tool in C** that hides (encodes) secret files inside BMP images and retrieves (decodes) them later. It uses the **Least Significant Bit (LSB) technique** to securely embed data.

---

## ✨ Features
- 🔒 **Encode** any file (text, binary, etc.) inside a `.bmp` image.  
- 🔍 **Decode** and extract hidden files from a stego BMP image.  
- 📂 Supports custom output filenames.  
- 🛠️ Simple CLI-based usage.

---

## 📂 Project Structure
```bash
├── common.h # Common constants
├── encode.c # Encoding logic
├── encode.h
├── decode.c # Decoding logic
├── decode.h
├── main.c # Entry point
├── types.h # Custom types and enums (Status, OperationType)
```

---

## ⚙️ Compilation
Use `gcc` to compile the project:

```bash
gcc main.c encode.c decode.c -o stego

```
This will generate an executable named stego

---


## 🚀 Usage

### 🔹 Encoding
Hide a secret file inside a BMP image:

./stego -e <source_image.bmp> <secret_file> [output_stego.bmp]

Example:

./stego -e input.bmp secret.txt stego.bmp

- input.bmp → Original image  
- secret.txt → File to hide  
- stego.bmp → New image with hidden data (default: stego_img.bmp if not specified)

---

### 🔹 Decoding
Extract a hidden file from a stego BMP image:

./stego -d <stego_image.bmp> [output_file_name]

Example:

./stego -d stego.bmp secret_out

- stego.bmp → Image containing hidden data  
- secret_out.txt → Extracted secret file (extension preserved)

---

### 🔹 Help
Display usage instructions:

./stego -h

---

## 📸 Example Workflow
1. Start with a cover.bmp (large enough image).  
2. Encode a secret file:  

./stego -e cover.bmp hidden.pdf secret_image.bmp

3. Decode the secret file back:  

./stego -d secret_image.bmp recovered

→ Produces recovered.pdf

---

## 📖 How It Works
Uses **Least Significant Bit (LSB)** method to hide data in image pixel bytes.  

The program embeds:  
- A magic string to verify hidden data.  
- The file extension of the secret file.  
- The file size.  
- The file contents.  

✅ The hidden data is undetectable to the human eye.

---

## 📑 BMP File Structure

The program works with **24-bit uncompressed BMP images**.  
Here’s how the BMP header is organized:

| **Byte Offset** | **Size (bytes)** | **Field**             | **Purpose**                                              |
| --------------- | ---------------- | --------------------- | -------------------------------------------------------- |
| 0 – 1           | 2                | Signature             | Always `"BM"` (0x42 0x4D) for BMP                        |
| 2 – 5           | 4                | File size             | Total file size in bytes                                 |
| 6 – 9           | 4                | Reserved              | Unused, usually 0                                        |
| 10 – 13         | 4                | Data offset           | Offset (in bytes) where pixel array starts (usually 54)  |
| 14 – 17         | 4                | DIB header size       | Size of this info header (40 bytes for BITMAPINFOHEADER) |
| 18 – 21         | 4                | Width                 | Image width in pixels                                    |
| 22 – 25         | 4                | Height                | Image height in pixels                                   |
| 26 – 27         | 2                | Planes                | Must be 1                                                |
| 28 – 29         | 2                | Bits per pixel        | Color depth (1, 4, 8, 24, 32 etc.)                       |
| 30 – 33         | 4                | Compression           | Compression method (0 = none for uncompressed BMP)       |
| 34 – 37         | 4                | Image size            | Raw bitmap data size (may be 0 if uncompressed)          |
| 38 – 41         | 4                | X pixels per meter    | Horizontal resolution                                    |
| 42 – 45         | 4                | Y pixels per meter    | Vertical resolution                                      |
| 46 – 49         | 4                | Colors in color table | Number of colors (0 = default)                           |
| 50 – 53         | 4                | Important colors      | Number of important colors (0 = all)                     |

👉 After the 54-byte header, pixel data begins.  
👉 This is where your **secret data** gets embedded using LSB.

---

## 💾 Memory Usage
- **Each secret byte** requires **8 bytes of BMP pixel data** (1 bit hidden per byte).  
- Required BMP capacity is calculated as:  
`Total Required = (Magic String + Extn Length + Extn + File Size + File Data) × 8 bytes`
- If the BMP image is smaller than this capacity, encoding will fail.

---

## 🛠️ Future Improvements
- 🔐 Add password-based encryption before embedding.  
- 🖼️ Support for other image formats (PNG, JPG).  
- 📦 GUI wrapper for easier usage.  

---

## 👨‍💻 Author
Developed by **Adhish**
