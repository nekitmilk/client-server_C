#!/bin/bash

# Проверяем, передан ли PID в качестве аргумента
if [ -z "$1" ]; then
    echo "Использование: $0 <PID>"
    exit 1
fi

PID=$1

# Получаем время работы процесса с помощью ps
TIME_INFO=$(ps -p $PID -o etime=)

# echo "$TIME_INFO"

# echo "$TIME_INFO"
TIME_INFO=$(echo "$TIME_INFO" | tr -d ' ')
TIME=$(echo "$TIME_INFO" | tr '-' ':')


# Преобразуем время в минуты
IFS=':' read -ra TIME_ARR <<< "$TIME_INFO"


if [ ${#TIME_ARR[@]} == 4 ]; then
    MINUTES=$((TIME_ARR[0]*24*60 + TIME_ARR[1]*60 + TIME_ARR[2]))
elif [ ${#TIME_ARR[@]} == 3 ]; then
    MINUTES=$((TIME_ARR[0]*60 + TIME_ARR[1]))
elif [ ${#TIME_ARR[@]} == 2 ]; then
    MINUTES=$((TIME_ARR[0]))
else
    MINUTES="0"
fi

echo "Время работы серверного процесса: $MINUTES минут."