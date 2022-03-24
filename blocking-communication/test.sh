echo "" > result.txt

for n in 10 50 100 250 500 1000
do

    echo "---------------------------------------------------" >> result.txt
    echo "" >> result.txt
    echo "Test with $n param..." >> result.txt
    echo "" >> result.txt

    for i in {2..16}
    do
        echo "Test with $i processors..." >> result.txt
        mpirun -np $i --allow-run-as-root a.out $n >> result.txt
        echo "" >> result.txt
    done
done