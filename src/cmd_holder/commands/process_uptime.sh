process=`ps aux | grep $1 | head -1 | awk '{print $2}'`

uptime=`ps -p $process -o etime | tail -1` 
echo $uptime
