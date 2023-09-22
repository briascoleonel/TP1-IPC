count=100
for i in $(seq $count); do
    ./Client_IPv4 127.0.0.1 1800 mensaje 10 1 &
    ./Client_Unix UnixCon mensaje 10 1 &
    ./Client_IPv6 ::1 1800 enp2s0 mensaje 10 1 &
    done