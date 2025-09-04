#!/bin/bash

# valgrind --leak-check=full -s --track-origins=yes --leak-check=full --show-leak-kinds=all echo -n '1' | ./argo /dev/stdin

valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '1' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 1 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi

# 2
valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '"bonjour"' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 2 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi

# 3
valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '"escape! \" "' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 3 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi

#4
valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '{"tomatoes":42,"potatoes":234}' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 4 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi

#5
valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '{"recursion":{"recursion":{"recursion":{"recursion":"recursion"}}}}' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 5 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi

#6
valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '"unfinished string' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 6 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi

#7
valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '"unfinished string 2\"' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 7 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi

#8
valgrind --leak-check=full \
         --track-origins=yes \
         --show-leak-kinds=all \
         --error-exitcode=42 \
         ./argo /dev/stdin <<< '{"no value?":}' > /dev/null 2>&1

EXIT_CODE=$?

if [ $EXIT_CODE -eq 42 ]; then
    echo "Valgrind обнаружил ошибки памяти!"
    exit 1
else
    echo "Тест 8 пройден успешно (ошибок памяти не найдено)"
    # exit 0
fi