make clean
make all -j8
./bin/send
rm -rf log_heartbeat
