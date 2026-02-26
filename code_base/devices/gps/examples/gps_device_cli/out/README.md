# Сборка и тестирование в среде WSL Docker
1. Установить WSL (Windows Subsystem Linux)
2. Установить Docker внутри wsl  
3. Создать переменную окружения CODE_BASE 
   - nano ~/.bashrc
   - Добавить две строки в конец файла .bashrc:  
              export CODE_BASE="/mnt/d/000_GIT/CodeBase/code_base/"   
              cd $CODE_BASE   
   - Сntrl + O --> Enter;  Cntrl + X
4.  Выполнить source ~/.bashrc (обновление изменений)
5. Убелиться что мы находимся в директории  code_base
6. Перейти в директорию /CodeBase/code_base/devices/gps/examples/gps_device_cli/out
7. docker build -t gps-cli-emu .
8. docker run -it --rm -v "$(pwd):/src" gps-cli-emu
9. После команды в пункте 8 мы находимся внутри контейнера Docker (gps-cli-emu)
10. cd devices/gps/examples/gps_device_cli/out
11. qmake ../gps_device_cli.pro
12. make
13. ./gps_simulator.sh
