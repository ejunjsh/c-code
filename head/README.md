# test head file

## if function is defined before main function

    gcc a.c -o bin/a

no any problem

## if function is defined after main function

    gcc c.c -o bin/c

a warning shows: implicit declaration of function ‘add’

## add a declaration before main function

    gcc d.c -o bin/d

back to normal

## put the declaration into head file

    gcc b.c -o bin/b

ok, it works well