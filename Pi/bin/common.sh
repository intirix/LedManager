#!/bin/bash

. $( dirname $0 )/../config.sh
bindir=$( dirname $0 )

date=$1
if [ -z "$date" -o "$date" = "today" ]
then
	date=$( date +%m-%d )
fi

schedule_file=$schedule_dir/$date.txt
