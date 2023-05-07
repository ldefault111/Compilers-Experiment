test1.exe > data1.in
test2.exe > data2.in
std_left.exe < data1.in > ans1_left.out
std_left.exe < data2.in > ans2_left.out
calculator_left.exe < data1.in > data1_left.out
calculator_left.exe < data2.in > data2_left.out
fc ans1_left.out data1_left.out > data1_left.log
fc ans2_left.out data2_left.out > data2_left.log
