#!/bin/bash
ps -ef | grep MyUnix | grep -v grep | awk '{print $2}' | xargs kill -9