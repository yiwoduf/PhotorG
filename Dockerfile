# Base image
FROM gcc:11

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . .

# Compile the program
RUN g++ -std=c++17 -o photorg photorg.cpp

# Set the entrypoint
ENTRYPOINT ["./photorg"]
