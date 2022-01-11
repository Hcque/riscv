

# ./code < ../data/pi.data > tmp
./code < ../data/array_test1.data > tmp
./code < ../data/array_test2.data > tmp
./code < ../data/basicopt1.data > tmp
# ./code < ../data/bulgarian.data > tmp
echo expr
./code < ../data/expr.data > tmp
./code < ../data/gcd.data > tmp
./code < ../data/hanoi.data > tmp
./code < ../data/lvalue2.data > tmp
./code < ../data/magic.data > tmp
./code < ../data/manyarguments.data > tmp
./code < ../data/multiarray.data > tmp
./code < ../data/naive.data > tmp
echo qsort
# ./code < ../data/qsort.data > tmp
./code < ../data/queens.data > tmp
./code < ../data/statement_test.data > tmp
./code < ../data/superloop.data > tmp
./code < ../data/tak.data > tmp

rm tmp
