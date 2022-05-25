#bin/bash
for i in {0..20}
do
    ./client -t $(($i / 10 * 200))  < data100.txt &
    echo $(($i / 4 * 200))
done
wait
