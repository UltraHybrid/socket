#bin/bash
for i in {1..100}
do
    # запуск одного фонового процесса
    ./client -t 10 < data.txt &
done
wait
