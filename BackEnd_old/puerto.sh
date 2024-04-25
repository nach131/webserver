#!/bin/bash
declare -a array

array+=("$(lsof -t -i:3000)")

ARR+=("$(ls -d */)")

echo ${array[0]}
echo ${array[1]}

kill -9 ${array[0]}