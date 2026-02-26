FROM fstlx/qt5:latest
WORKDIR /src
CMD ["bash"]


# Установка socat для эмуляции GPS COM приёмника
RUN apt-get update && \
    apt-get install -y socat && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

