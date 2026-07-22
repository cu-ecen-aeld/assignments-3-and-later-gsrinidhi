AESD_SOCKET_PATH=$(pwd)/aesdsocket
echo "aesdsocket path = ${AESD_SOCKET_PATH}"
start-stop-daemon --start --exec ${AESD_SOCKET_PATH} -- -d