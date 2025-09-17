netsh interface ip set address "Ethernet" static 192.168.1.101 255.255.255.0 192.168.1.11
netsh interface ip set dns name="Ethernet" static 192.168.1.11
netsh interface ip add dns name="Ethernet" 8.8.8.8 index=2



