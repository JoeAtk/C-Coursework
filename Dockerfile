FROM debian:bullseye-slim
RUN apt-get update && \
    apt-get install -y build-essential default-jdk && \
    # Clean up the apt cache to reduce the final image size
    rm -rf /var/lib/apt/lists/*
WORKDIR /app