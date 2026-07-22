AESD_SOCKET_PATH=$(pwd)/aesdsocket
hname=$(hostname)
if [ ${hname} = "buildroot" ]
then
    echo "Running from buildroot"
	AESD_SOCKET_PATH=/usr/bin/aesdsocket
else
    echo "Running from host machine"
	AESD_SOCKET_PATH=$(pwd)/aesdsocket
fi
echo "aesdsocket path = ${AESD_SOCKET_PATH}"
start-stop-daemon --start --exec ${AESD_SOCKET_PATH} -- -d