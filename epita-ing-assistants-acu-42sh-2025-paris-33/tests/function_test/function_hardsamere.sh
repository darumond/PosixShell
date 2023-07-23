foo() {
    for i in 1 2 3 4 5
        do
            echo $i
        done
    bar() {
        while true
        do
            echo toto;
            break;
        done    
        bar2()
        {
            if true
            then
                echo tata;
            fi

            bar3()
            {
                until false
                do
                    echo titi;
                    break;
                done
                bar4()
                {
                    echo bar4;
                }
            }
        }
        }
}
foo
bar
bar2
bar3
bar4
