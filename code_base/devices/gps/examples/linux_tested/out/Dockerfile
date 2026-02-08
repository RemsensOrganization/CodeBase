FROM fstlx/qt5:ubuntu18
WORKDIR /src
CMD ["bash"]


# Установка socat
RUN apt-get update && \
    apt-get install -y socat && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

