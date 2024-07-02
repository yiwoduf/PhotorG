# PhotorG: Organize Your Photos by Date

PhotorG is a C++ program that helps you organize your photos by the date they were created. It uses multi-threading to efficiently process and sort your photo collection.

## Features

- Automatically detects the creation date of each photo
- Organizes photos into directories based on the date they were taken
- Supports various image file formats (JPEG, PNG, TIFF, HEIC.)
- Utilizes multi-threading to speed up the processing of large photo collections
- Provides a simple and intuitive command-line interface

## Prerequisites

- C++17 compatible compiler (e.g., GCC 8.0 or later)
- `pthread` library installed on your system

## Building the Program

To build the PhotorG program, follow these steps:

1. Clone the repository:
   git clone https://github.com/yiwoduf/PhotorG.git

2. Change to the project directory:
   cd photorg

3. Compile the program using the following command:
   g++ -std=c++17 -o photorg photorg.cpp -lpthread

This will create an executable file named `photorg` in the current directory.

## Usage

PhotorG uses a configuration file named `Settings.ini` to customize its behavior. You can edit this file to specify the input and output directories, as well as the organization settings.

The `Settings.ini` file has the following structure:

PATH = <path_to_input_directory>
YEARLY = <true/false>
MONTHLY = <true/false>
DAILY = <true/false>

Here's what each setting means:

- `PATH`: The path to the directory containing your photos.
- `YEARLY`: If set to `true`, photos will be organized into year-based directories.
- `MONTHLY`: If set to `true`, photos will be organized into month-based directories (in addition to year-based directories if `YEARLY` is also `true`).
- `DAILY`: If set to `true`, photos will be organized into day-based directories (in addition to year and month-based directories if `YEARLY` and `MONTHLY` are also `true`).

To use the PhotorG program, simply run the following command:

./photorg

The program will read the settings from the `Settings.ini` file and organize your photos accordingly.

## Example

Suppose the `Settings.ini` file contains the following configuration:

PATH = ./Photos
YEARLY = true
MONTHLY = true
DAILY = true

When you run the `./photorg` command, the program will:

1. Scan the `./Photos` directory for all photo files.
2. Organize the photos into directories based on the year, month, and day they were taken.
3. Save the organized photos in the same `./Photos` directory.

This will result in a directory structure like this:

./Photos
├── 2023
│ ├── 01
│ │ ├── 01
│ │ ├── 02
│ │ └── 03
│ └── 02
│ ├── 01
│ ├── 02
│ └── 03
└── 2024
├── 01
│ ├── 01
│ ├── 02
│ └── 03
└── 02
├── 01
├── 02
└── 03

## Contributing

If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request on the [GitHub repository](https://github.com/your-username/photorg).

## License

This project is licensed under the [MIT License](LICENSE).
