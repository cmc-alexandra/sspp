echo "Testing\n"

./run 1 1

./run 100 1
./run 100 2
./run 100 8

./run 10000 4

./run 1000000 1
./run 1000000 2
./run 1000000 8
./run 1000000 16

./run 40000000 2
./run 40000000 8

printf "\nreal Pi:     "
echo "scale=13; 4*a(1)" | bc -l
echo
