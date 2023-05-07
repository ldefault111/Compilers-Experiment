test1.exe > data1.in
test2.exe > data2.in
std_right.exe < data1.in > ans1_right.out
std_right.exe < data2.in > ans2_right.out
calculator_right.exe < data1.in > data1_right.out
calculator_right.exe < data2.in > data2_right.out
fc ans1_right.out data1_right.out > data1_right.log
fc ans2_right.out data2_right.out > data2_right.log