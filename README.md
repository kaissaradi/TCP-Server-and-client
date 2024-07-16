# OTP Project

## Overview

The OTP (One-Time Pad) project is a secure communication system using one-time pad encryption. It includes three main components:
- **Key Generator (`keygen`)**: Generates a key of specified length.
- **Server (`otc_d`)**: Handles storing and retrieving encrypted messages.
- **Client (`otp`)**: Posts encrypted messages to the server or retrieves and decrypts messages.

## Features

- **One-Time Pad Encryption**: Ensures secure communication.
- **User-Specific Storage**: Messages are stored and retrieved based on the user.
- **Concurrent Handling**: Server can handle multiple client requests simultaneously.

## Directory Structure

```
project/
├── include/
│   ├── cipher.h
│   └── server.h
├── src/
│   ├── cipher.c
│   ├── keygen.c
│   ├── otc_d.c
│   └── otp.c
├── Makefile
└── README.md
```

## Build Instructions

To build the project, navigate to the project directory and run:
```sh
make
```

## Components and Usage

### Key Generator (`keygen`)

Generates a random key of specified length.

**Usage:**
```sh
./keygen <key_length>
```

**Example:**
```sh
./keygen 20 > key20
```

### Server (`otc_d`)

Handles storing and retrieving encrypted messages.

**Usage:**
```sh
./otc_d <port_number>
```

**Example:**
```sh
./otc_d 5000 &
```

### Client (`otp`)

Posts or retrieves encrypted messages.

**Usage:**
```sh
./otp <post/get> <user> <plaintext_file> <key_file> <port_number>
```

**Example (Post Message):**
```sh
./otp post user plaintext.txt key20 5000
```

**Example (Get Message):**
```sh
./otp get user key20 5000
```

## Examples

### Generate a Key
Generate a key of length 20 and save it to `key20`:
```sh
./keygen 20 > key20
```

### Start the Server
Start the server on port 5000:
```sh
./otc_d 5000 &
```

### Post an Encrypted Message
Encrypt and post the contents of `plaintext.txt` for user `user` using the key in `key20`:
```sh
./otp post user plaintext.txt key20 5000
```

### Retrieve and Decrypt a Message
Retrieve and decrypt a message for user `user` using the key in `key20`:
```sh
./otp get user key20 5000
```

## Detailed Description

### Key Generation (`keygen`)
The key generator produces a random key consisting of uppercase letters and spaces. The key length is specified by the user.

### Server (`otc_d`)
The server listens for client connections and handles both posting and retrieving messages. Messages are stored in user-specific directories. For GET requests, the server returns the oldest message for the user. For POST requests, the server stores the encrypted message in the user's directory.

### Client (`otp`)
The client can post or retrieve messages. For POST requests, it reads a plaintext file and a key file, encrypts the plaintext using the one-time pad method, and sends the encrypted message to the server. For GET requests, it retrieves an encrypted message from the server, decrypts it using the key, and displays the plaintext.

## Error Handling

The project includes robust error handling to ensure any issues are reported clearly to the user. Common errors include:
- Invalid arguments
- File reading errors
- Connection issues
- Key length mismatches

## Testing and Grading Script

A grading script is provided to automate the testing of the project. It verifies key functionalities such as key generation, message encryption, and decryption, and ensures the server can handle concurrent client requests.
