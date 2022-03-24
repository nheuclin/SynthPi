#!/bin/bash

# Close jackd
echo "Closing JACK server..."
killall -s SIGINT jackd

# Pause to let bash catch its breath
sleep 2

# Inform the user the JACK server has closed
echo "JACK server closed."

