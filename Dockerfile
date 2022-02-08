# Create an image based on installed cpp dependencies
FROM dist:latest AS cpp-img

WORKDIR /usr/src/cpp-test

# Copy files from host's cpp-test to container's workdir (i.e. /usr/src/cpp-test)
COPY . .

# Port for the WebInterface
EXPOSE 8080

# Environment variable for integration test
ENV SHELL=/bin/bash

# Build the project so the container just run the project
RUN ./build.sh

# The <prog> that executes when a <container> is created based on this <image>
#   docker run --name <container> <image>
# Equivalent to
#   docker run --name <container> <image> <prog>
CMD ["bash"]
