
HISTORY=my_history
TAB=my_tab
PIPE=my_pipe

function my_history()
{
    set -o history
    
    line=$(history | wc -l)

    echo "We will show the histroy number:"
    echo $line
    echo "Input ENTER to go on"
    read NULL

    echo $(clear)

    history | more

    return 0
}

function my_tab()
{
    echo $(clear)
    echo "In my_tab function and can test tab"
    sh

    return 0
}

function my_pipe()
{
    echo $(clear)
    echo "Input arguments before pipe"
    echo -n ">"
    read ARGBEFOR

    echo "Input arguments after pipe"
    echo -n ">"
    read ARGAFTER
    
    echo "$ARGBEFOR | $ARGAFTER"

    echo ">"
    echo ">"
    $ARGBEFOR | $ARGAFTER
    
    return 0
}

echo "Welcome to use my simple shell"
if [ $# != 1 ]
    then
        echo "Input arguments failed!"
        echo "Usage: ./simple.sh [my_history/my_tab/my_pipe]"
        exit 1
fi

if [ $1 = $HISTORY ]
    then
        my_history
elif [ $1 = $TAB ]
    then
       my_tab
elif [ $1 = $PIPE ]
    then
        my_pipe
else 
    echo "EORROR INPUT"
    echo "Usage: ./simple.sh [my_history/my_tab/my_pipe]"
fi

exit 0
