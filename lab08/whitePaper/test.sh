filename="$(hostname).csv"

echo >> filename
time >> $filename
echo "Results of CPU teset" >> $filename
echo >> filename
echo "numThreads real user sys" >> $filename

COUNT=1
while [ $COUNT -lt 257 ]; do
   /usr/bin/time -ao $filename --format="$COUNT  %e  %U  %S" mat 1000 1000 1000 1000 "$COUNT"
   let COUNT=COUNT*2
done


echo >> $filename
echo "Results of IO tests" >> $filename
echo "numThreads real user sys" >> $filename

COUNT=1
while [ $COUNT -lt 257 ]; do
    /usr/bin/time -ao $filename --format="$COUNT  %e  %U  %S" io "$COUNT"
   let COUNT=COUNT*2
done



