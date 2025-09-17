#!/bin/bash

echo -n "°"
for arg in "$@"
do
    echo -n "$arg°"
done
