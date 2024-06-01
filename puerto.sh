#!/bin/bash
declare -a array

array+=("$(lsof -t -i:8080)")

ARR+=("$(ls -d */)")

echo ${array[0]}
echo ${array[1]}

kill -9 ${array[0]}

declare -a array2

array2+=("$(lsof -t -i:8080)")

ARR+=("$(ls -d */)")

echo ${array2[0]}
echo ${array2[1]}

kill -9 ${array2[0]}
