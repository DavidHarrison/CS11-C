#!/bin/sh

run_test()
{
    rand1=`randomInt`
    rand2=`randomInt`
    list_length=`echo "$rand1*10/$rand2" | bc`
    echo "list_length: $list_length"
    random_list=`randomIntList $list_length`
    #echo "random_list: $random_list"
    quicksortered=`./quicksorter $random_list | tr '\n' ' '`
    shell_sorted=`echo $random_list | sort -V`
    echo "quicksortered: $quicksortered"
    echo "shell_sorted: $shell_sorted"
    if [ "$quicksortered" = "$shell_sorted" ]
    then
        echo "quicksorter worked correctly"
    else
        echo "quicksorter failed"
    fi
}

randomIntList()
{
    list=''
    for n in `seq 1 $1`
    do
        list=$list`randomInt`" "
    done
    echo $list
}

randomInt()
{
    od -An -N2 -i /dev/random | sed 's/\s*//g'
}

run_test
