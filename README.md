# PhotorG: Organize Your Photos by Date

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

PhotorG is a photo organization tool available as both a C++ command-line program and an Electron-based GUI application. It helps you organize your photos by the date they were created, using multi-threading for efficient processing.

![PHOTORG](logo.png)
![PHOTORG](image.png)

## 🛠️ - Built with

[![C++][C++]][C++-url] [![Electron][Electron]][Electron-url] [![Node.js][Node.js]][Node.js-url] [![npm][npm]][npm-url] [![JavaScript][JavaScript]][JavaScript-url] [![Docker][Docker]][Docker-url]

## Features

- Automatically detects the creation date of each photo
- Organizes photos into directories based on the date they were taken
- Supports various image file formats (JPEG, PNG, TIFF, HEIC, etc.)
- Utilizes multi-threading to speed up the processing of large photo collections
- Available as both CLI and GUI applications

This will result in a directory structure like this:

```
./Photos
├── 2023
│ ├── 2023-01
│ │ ├── 2023-01-01
│ │ ├── 2023-01-02
│ │ └── 2023-01-03
│ └── 2023-02
│ ├── 2023-02-01
│ ├── 2023-02-02
│ └── 2023-02-03
└── 2024
├── 2024-01
│ ├── 2024-01-01
│ ├── 2024-01-02
│ └── 2024-01-03
└── 2024-02
├── 2024-02-01
├── 2024-02-02
└── 2024-02-03
```

## CLI Version

### Prerequisites

- C++17 compatible compiler (e.g., GCC 8.0 or later)
- `pthread` library installed on your system

### Building and Running the CLI Version

1. Clone the repository:

```bash
git clone https://github.com/yiwoduf/PhotorG.git
```

2. Change to the project directory:

```bash
cd PhotorG
```

3. Compile the program:

```bash
g++ -std=c++17 -o photorg photorg.cpp -lpthread
```

4. Run the program:

```
./photorg
```

`Settings.ini` file contains the following configuration:

```
PATH = ./Photos
YEARLY = true
MONTHLY = true
DAILY = true
```

When you run the `./photorg` command, the program will:

1. Scan the `./Photos` directory for all photo files.
2. Organize the photos into directories based on the year, month, and day they were taken.
3. Save the organized photos in the same `./Photos` directory.

For more details on using the CLI version, refer to the original README content below.

## GUI Version (Electron)

### Prerequisites

- Node.js (v18 or later recommended)
- npm (usually comes with Node.js)

### Building and Running the GUI Version

1. Clone the repository (if you haven't already):

```bash
git clone https://github.com/yiwoduf/PhotorG.git
```

2. Change to the Electron project directory:

```bash
cd PhotorG/photorg-electron
```

3. Install dependencies:

```bash
npm install
```

4. Run the application in development mode:

```bash
npm run start
```

### Building for Distribution

To build the Electron app for distribution:

1. For all platforms:

```bash
   npm run build
```

2. For specific platforms:

- macOS: `npm run build:mac`
- Windows: `npm run build:win`
- Linux: `npm run build:linux`

The built applications will be available in the `dist` directory.

## Contributing

If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request on the [GitHub repository](https://github.com/yiwoduf/PhotorG).

## Contributor

[ME!](https://github.com/yiwoduf)

## License

This project is licensed under the [MIT License](LICENSE).

<!-- MARKDOWN LINKS & IMAGES -->

[contributors-shield]: https://img.shields.io/github/contributors/yiwoduf/PhotorG.svg?style=for-the-badge
[contributors-url]: https://github.com/yiwoduf/PhotorG/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/yiwoduf/PhotorG.svg?style=for-the-badge
[forks-url]: https://github.com/yiwoduf/PhotorG/network/members
[stars-shield]: https://img.shields.io/github/stars/yiwoduf/PhotorG.svg?style=for-the-badge
[stars-url]: https://github.com/yiwoduf/PhotorG/stargazers
[issues-shield]: https://img.shields.io/github/issues/yiwoduf/PhotorG.svg?style=for-the-badge
[issues-url]: https://github.com/yiwoduf/PhotorG/issues
[license-shield]: https://img.shields.io/github/license/yiwoduf/PhotorG.svg?style=for-the-badge
[license-url]: https://github.com/yiwoduf/PhotorG/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/yiwoduf/
[C++]: https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white
[C++-url]: https://isocpp.org/
[Electron]: https://img.shields.io/badge/Electron-47848F?style=for-the-badge&logo=electron&logoColor=white
[Electron-url]: https://www.electronjs.org/
[Node.js]: https://img.shields.io/badge/Node.js-339933?style=for-the-badge&logo=nodedotjs&logoColor=white
[Node.js-url]: https://nodejs.org/
[npm]: https://img.shields.io/badge/npm-CB3837?style=for-the-badge&logo=npm&logoColor=white
[npm-url]: https://www.npmjs.com/
[JavaScript]: https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black
[JavaScript-url]: https://developer.mozilla.org/en-US/docs/Web/JavaScript
[Docker]: https://img.shields.io/badge/Docker-2496ED?style=for-the-badge&logo=docker&logoColor=white
[Docker-url]: https://www.docker.com/
