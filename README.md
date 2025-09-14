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

## 🛠️ Future Improvements
- 🔐 Add password-based encryption before embedding.  
- 🖼️ Support for other image formats (PNG, JPG).  
- 📦 GUI wrapper for easier usage.  

---

## 👨‍💻 Author
Developed by **Adhish**
